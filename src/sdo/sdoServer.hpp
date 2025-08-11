/**
 * Management of SDO Uploads in both expedited and segmented modes.
 */
#pragma once

#include "frames.hpp"
#include "odAccessor.hpp"

#define SDO_STATE_TABLE(X)                                    \
    X(Idle, nullptr)                                          \
    X(InitiateUploadRequest, handleInitiateUploadRequest)     \
    X(UploadFirstSegment, uploadFirstSegment)                 \
    X(UploadSubsequentSegment, handleUploadSubsequentSegment) \
    X(handleAbort, handleAbort)

// clang-format off
#define SDO_FRAME_DISPATCH_TABLE(X)                                                    \
    X(0, SDOFrameUploadSegmentRequest,       &SDO::handleUploadSubsequentSegment)      \
    X(1, SDOFrameDownloadInitiateRequest,    &SDO::handleDownloadInitiateRequest)      \
    X(2, SDOFrameUploadInitiateRequest,      &SDO::handleUploadInitiateRequest)        \
    X(3, SDOFrameDownloadSegmentRequest,     &SDO::handleDownloadSubsequentSegment)    \
    X(4, SDOFrameAbort,                      &SDO::handleAbort)                        \
    X(5, SDOFrameBlockUploadInitiateRequest, &SDO::handleBlockUploadInitiateRequest)   \
    X(6, SDOFrameBlockDownloadDispatch,      &SDO::handleBlockDownloadInitiateRequest) \
    X(7, SDOFrameBlockUploadDispatch,        &SDO::handleBlockUploadDispatch)
// clang-format on

/**
 * SDO Server class that manages SDO operations.
 */
class SDO {
    using CCS = std::underlying_type<SDOClientCommandSpecifiers>::type;

    ODAccessor &odAccessor;
    SDOAbortCode errorCode = SDOAbortCode_OK;

    enum class State {
#define X(name, handler) name,
        SDO_STATE_TABLE(X)
#undef X
            COUNT
    };

    State state = State::Idle;
    State awaitState = State::Idle;

    Data objectData;

    // Type erasure for SDO Frame handlers.
    using ErasedHandler = void (*)(SDO &, SDOFrame &);

    typedef void (SDO::*HandlerFunc)(SDOFrame &request);

    static const HandlerFunc
        dispatch[static_cast<int>(SDOClientCommandSpecifiers::Reserved) + 1];
    static const HandlerFunc
        handlers[static_cast<int>(State::AwaitODResponse) + 1];

    // Trampoline: reinterpret-cast SDOFrame -> FrameT and call Method.
    template <class FrameT, void (SDO::*Method)(FrameT &)>
    static void Thunk(SDO &self, SDOFrame &base) noexcept {
        static_assert(std::is_trivially_copyable<FrameT>::value,
                      "FrameT must be trivial");
        static_assert(sizeof(FrameT) == sizeof(SDOFrame),
                      "FrameT layout must match SDOFrame");
        auto &f = reinterpret_cast<FrameT &>(base);
        (self.*Method)(f);
    }

    SDO(ODAccessor &odAccessor) : odAccessor(odAccessor) {}

    /**
     * Client wants this server to read an object in the object dictionary.
     */
    void handleInitiateUploadRequest(SDOFrameInitiateUploadRequest &request);
    void uploadFirstSegment(SDOFrameInitiateUploadRequest &request);
    void handleUploadSubsequentSegment(SDOFrameUploadSegmentRequest &request);


    /**
     * Client wants this server to write an object in the object dictionary.
     */
    void handleInitiateDownloadRequest(SDOFrameInitiateDownloadRequest &request);
    void handleDownloadSubsequentSegment(SDOFrameDownloadSegmentRequest &request);
    void downloadInitiateEnd();
    void downloadSegmentEnd();

    void asyncRead(State nextState = State::Idle) {
        errorCode = odAccessor.asyncRead();
        if (errorCode != SDOAbortCode_OK) {
            abortOperation(errorCode);
            return;
        }
        awaitState = nextState;
        state = State::AwaitODResponse;
    }

    void asyncWrite(State nextState = State::Idle) {
        errorCode = odAccessor.asyncWrite();
        if (errorCode != SDOAbortCode_OK) {
            abortOperation(errorCode);
            return;
        }
        awaitState = nextState;
        state = State::AwaitODResponse;
    }

    void awaitODResponse() {
        if (odAccessor.readComplete()) {
            state = awaitState;
            executeNextAction();
        }
    }

    void abortOperation(SDOAbortCode code) {
        sendAbortFrame(sdoObject.index, sdoObject.subindex, code);
        odAccessor.clear();
        state = State::Idle;
    }

    void executeNextAction() {
        if (state == State::Idle) return;
        auto handler = handlers[static_cast<std::size_t>(state)];
        (this->*handler)();
    }

    void sendFrame(SDOFrame &frame) {
        frame.nodeId = node.nodeId;
        frame.functionCode = 0b1011;
    }
   public:
    /**
     * Called upon frame reception related to SDO services.
     */
    void receiveFrame(SDOFrame &frame, uint32_t timestamp_us) {
        frameDispatchTable[static_cast<CCS>(frame.getCCS())](frame);
    }

    /**
     * Server async updates.
     */
    void update() { executeNextAction(); }
};

/**
 * Handlers for each state of the SDO Server process.
 */
static constexpr std::array<HandlerFunc, static_cast<std::size_t>(State::COUNT)>
    handlers = {{
#define X(name, handler) handler,
        SDO_STATE_TABLE(X)
#undef X
    }};

/**
 * Frame dispatcher based on the command specifier.
 * Upon receiving a frame, the command specifier is checked
 * and the corresponding handler is called.
 */
static constexpr ErasedHandler dispatch[] = {
#define X(ccs, FrameT, Method) &Thunk<FrameT, Method>,
    SDO_FRAME_DISPATCH_TABLE(X)
#undef X
};

namespace {
static_assert(handlers.size() == static_cast<std::size_t>(State::COUNT),
              "handlers table mismatch");

static_assert(
    sizeof(dispatch) / sizeof(ErasedHandler) ==
        static_cast<std::size_t>(SDOClientCommandSpecifiers::Reserved) + 1,
    "Mismatch between dispatch table and CCS enum range");
}  // namespace