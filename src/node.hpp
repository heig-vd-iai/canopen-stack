/******************************************************************************
 * [Filename]:      node.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the declaration of the Node class.
 *****************************************************************************/
#pragma once
#include "nmt.hpp"
#include "heartbeat.hpp"
#include "sdo.hpp"
#include "od.hpp"
#include "pdo.hpp"
#include "sync.hpp"
#include "emergency.hpp"
#include <cstdint>

namespace CANopen
{
    /**
     * @brief Represents a CANopen slave node.
     * This is the main class of this library.
     */
    class Node
    {
    private:
        ObjectDictionnary _od;
        NMT _nmt;
        HB _hb;
        SDO _sdo;
        PDO _pdo;
        SYNC _sync;
        EMCY _emcy;

        /**
         * @brief Send a CANopen frame to the CAN network.
         * @param frame The CANopen frame to send.
         */
        void sendFrame(class Frame &frame);

        /**
         * @brief Get the relative clock time.
         * This value is used internally and doesn't have to be absolute.
         * The counter MUST count up to 0xFFFFFFFF in order to avoid clocking issues.
         * @return Current clock time in microseconds.
         */
        uint32_t getTime_us();

    public:
        friend NMT;
        friend HB;
        friend SDO;
        friend PDO;
        friend SYNC;
        friend EMCY;
        const uint8_t nodeId;

        /**
         * @brief Constructor for the Node class.
         * @param id The ID of the node.
         */
        Node(uint8_t id);

        /**
         * @brief Get the %Object Dictionnary object.
         * @return Reference to the ObjectDictionnary instance.
         */
        ObjectDictionnary &od();

        /**
         * @brief Get the %NMT object.
         * @return Reference to the NMT instance.
         */
        NMT &nmt();

        /**
         * @brief Get the Heartbeat object.
         * @return Reference to the HB instance.
         */
        HB &hb();

        /**
         * @brief Get the %SDO object.
         * @return Reference to the SDO instance.
         */
        SDO &sdo();

        /**
         * @brief Get the %PDO object.
         * @return Reference to the PDO instance.
         */
        PDO &pdo();

        /**
         * @brief Get the Sync object.
         * @return Reference to the SYNC instance.
         */
        SYNC &sync();

        /**
         * @brief Get the Emergency object.
         * @return Reference to the EMCY instance.
         */
        EMCY &emcy();

        /**
         * @brief Initialize the CANopen node.
         * This method will initialize the %NMT state machine, so it should be called first.
         */
        void init();

        /**
         * @brief Receive and process a CANopen frame.
         * This method should be called when a CAN frame is received from the bus.
         * @param frame The CANopen frame to process.
         */
        void receiveFrame(class Frame frame);

        /**
         * @brief Update the node by updating its CANopen objects.
         * This method should be called periodically.
         */
        void update();
    };
}
