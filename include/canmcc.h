#ifndef __CAN_MCC_H__
#define __CAN_MCC_H__

#include "canmanager.h"
#include "telemetry.h"
#include "motor_control.h"


#define MCC_CAN_TICKER_INTERVAL 10000us


/*
    Class managing CAN bus for MCC
    See CANManager class for usage documentation
*/
class CANMCC : public CANManager {
    public:
        CANMCC(PinName rd, PinName td, MCCState* stateMachine, int frequency = DEFAULT_CAN_FREQ);
        void readHandler(int messageID, SharedPtr<unsigned char> data, int length);
        void send_mcc_data();

    private:
        MCCState* stateMachine;
        void send_helper(int messageID, void *data, int length);
};

#endif