/**
 * Management of SDO Uploads in both expedited and segmented modes.
 */
#pragma once

#include "frames.hpp"

// Trampoline: reinterpret-cast SDOFrameNew -> FrameT and call Method.
template<class FrameT, void (SDO::*Method)(FrameT&)>
static void Thunk(SDO& self, SDOFrameNew& base) noexcept {
    static_assert(std::is_trivially_copyable<FrameT>::value, "FrameT must be trivial");
    static_assert(sizeof(FrameT) == sizeof(SDOFrameNew), "FrameT layout must match SDOFrameNew");
    auto& f = reinterpret_cast<FrameT&>(base);
    (self.*Method)(f);
}

class SDO {
    ODAccessor &odAccessor;
    SDOAbortCode errorCode = SDOAbortCode_OK;

    enum class State {
        Idle,
        InitiateRequest,
        UploadFirstSegment,
        UploadSubsequentSegment,
        AwaitODResponse,
    };

    State state = State::Idle;
    State awaitState = State::Idle;

    DataPromise objectData;

    // Type erasure for SDO Frame handlers.
    using ErasedHandler = void (*)(SDO&, SDOFrameNew&);

    typedef void (SDO::*HandlerFunc)(SDOFrame &request);

    static const HandlerFunc frameDispatcher[static_cast<int>(SDOClientCommandSpecifiers::Reserved) + 1];
    static const HandlerFunc handlers[static_cast<int>(State::AwaitODResponse) + 1];

    SDO(ODAccessor &odAccessor) : odAccessor(odAccessor) {}

    void handleInitiateDownloadRequest(SDOFrameInitiateDownloadRequest &request) {
        uint32_t index = request.getIndex();
        uint8_t subindex = request.getSubindex();

        if ((errorCode = odAccessor.lookup(index, subindex)) != SDOAbortCode_OK) {
            abortOperation(errorCode);
            return;
        }

        if (!odAccessor.isReadable()) {
            abortOperation(SDOAbortCode_AttemptReadOnWriteOnly);
            return;
        }

        errorCode = odAccessor.asyncRead();
        if (errorCode != SDOAbortCode_OK) {
            abortOperation(errorCode);
            return;
        }

        awaitState = State::UploadFirstSegment;
        state = State::AwaitODResponse;
    }

    /**
     * Client Request -> Server Response
     */
    void uploadFirstSegment(SDOFrameInitiateDownloadRequest &request) {
        uint8_t *dataPtr;
        if (sdoObject.metadata.dataType == DataType::DOMAIN) {
            remainingBytes = odData.domain.size;
            dataPtr = domainBuffer;
        } else {
            remainingBytes = sdoObject.size;
            dataPtr = &odData.u8;
        }

        SDOFrameUploadSegmentRequest response(node.nodeId, sdoObject.index, sdoObject.subindex);

        // If data size overflows the frame, segmented transfer is enabled
        // thus, 0 bytes are sent in the first segment, but the size is set
        // to the total size of the object.
        auto dataSize = response.setData(&odData.u8, remainingBytes);
        remainingBytes -= dataSize;

        state = remainingBytes ? State::UploadSubsequentSegment : State::Idle;

        sendFrame(response);
        toggleBit = false;
    }

    void handleUploadSubsequentSegment(SDOFrame &request) {
        SDOSegmentCommand recvCmd(request.getCommandByte());

        // Toggle bit management
        if (toggleBit != recvCmd.getToggleBit()) {
            abortOperation(SDOAbortCode_ToggleBitNotAlternated);
            return;
        }
        toggleBit = !toggleBit;

        SDOSegmentCommand sendCmd(SDO_CCS_DOWNLOAD_SEGMENT_REQUEST);
        sendCmd.setSize(remainingBytes);

        /**
         * Must refetch odData in case of domain
         * bufferSize >= IPCbuffer + PayloadSize
         */

        SDOSegmentFrame response(node.nodeId);
        remainingBytes -= response.setData(&odData.u8, remainingBytes);

        sendCmd.setIsFinalSegment(remainingBytes == 0);
        response.setCommandByte(sendCmd.encode());

        sendFrame(response);

        if (remainingBytes == 0)
            state = State::Idle;
    }

    void handleDownloadInitiateRequest(SDOFrame &request) {}
    void downloadFirstSegment(SDOFrame &request) {}
    void handleDownloadSubsequentSegment(SDOFrame &request) {}

    void handleBlockDownloadInitiateRequest(SDOFrame &request) {}
    void blockDownloadReceive(SDOFrame &request) {}
    void blockDownloadEnd(SDOFrame &request) {}

    // Upload Block (client request, server response)
    void handleBlockUploadInitiateRequest(SDOFrame &request) {}
    void blockUploadReceive(SDOFrame &request) {}
    void blockUploadEn(SDOFrame &request) {}




    void awaitODResponse(SDOFrame &request) {
        if (odAccessor.errorOccurred())
            abortOperation(odAccessor.getErrorCode());
        else if (odAccessor.operationComplete()) {
            odData = odAccessor.getData();
            state = awaitState;
            executeNextAction();
        }
    }

    void abortOperation(SDOAbortCode code) {
        sendAbortFrame(sdoObject.index, sdoObject.subindex, code);
        odAccessor.clear();
        state = State::Idle;
    }

public:
    /**
     * Called upon frame reception related to SDO services.
     */
    void receiveFrame(SDOFrameNew &frame, uint32_t timestamp_us) {
        auto ccs = frame.getCCS();
        frameDispatcher[ccs](frame);
    }

    /**
     * Server async updates.
     */
    void update() {
        switch(state) {
            case State::AwaitODResponse:
                executeNextAction();
        };
        return false;
    }
};


/**
 * Frame dispatcher based on the command specifier.
 * Upon receiving a frame, the command specifier is checked
 * and the corresponding handler is called.
 */
static constexpr ErasedHandler frameDispatcher[] = {
    /* ccs=0 */ &Thunk<SDOFrameUploadSegmentRequest,       &SDO::handleUploadSubsequentSegment>,
    /* ccs=1 */ &Thunk<SDOFrameDownloadInitiateRequest,    &SDO::handleDownloadInitiateRequest>,
    /* ccs=2 */ &Thunk<SDOFrameUploadInitiateRequest,      &SDO::handleUploadInitiateRequest>,
    /* ccs=3 */ &Thunk<SDOFrameDownloadSegmentRequest,     &SDO::handleDownloadSubsequentSegment>,
    /* ccs=4 */ &Thunk<SDOFrameAbort,                      &SDO::handleAbort>,
    /* ccs=5 */ &Thunk<SDOFrameBlockUploadInitiateRequest, &SDO::handleBlockUploadInitiateRequest>,
    /* ccs=6 */ &Thunk<SDOFrameBlockDownloadDispatch,      &SDO::handleBlockDownloadInitiateRequest>,
    /* ccs=7 */ &Thunk<SDOFrameBlockUploadDispatch,        &SDO::handleBlockUploadDispatch>,
};

/**
 * Handlers for each state of the SDO Server process.
 */
static const HandlerFunc handlers[] = {
    nullptr, // Idle

    // Download Object (client request, server response)
    &SDOUpload::handleDownloadInitiateRequest,
    &SDOUpload::downloadFirstSegment,
    &SDOUpload::handleDownloadSubsequentSegment,

    // Upload Object (client request, server response)
    &SDOUpload::handleUploadInitiateRequest,
    &SDOUpload::uploadFirstSegment,
    &SDOUpload::handleUploadSubsequentSegment,

    // Download Block (client request, server response)
    &SDOUpload::handleBlockDownloadInitiateRequest,
    &SDOUpload::blockDownloadReceive,
    &SDOUpload::blockDownloadEnd,

    // Upload Block (client request, server response)
    &SDOUpload::handleBlockUploadInitiateRequest,
    &SDOUpload::blockUploadReceive,
    &SDOUpload::blockUploadEnd

    // Other States
    &SDOUpload::handleAbort
};
