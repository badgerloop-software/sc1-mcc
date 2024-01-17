#ifndef __CAN_MCC_H__
#define __CAN_MCC_H__

#include "canmanager.h"
#include "telemetry.h"
#include "motor_control.h"


/*
    Class managing CAN bus for MCC
    See CANManager class for usage documentation
*/
class CANMCC : public CANManager {
    public:
        CANMCC(PinName rd, PinName td, int frequency = DEFAULT_CAN_FREQ);
        void readHandler(int messageID, SharedPtr<unsigned char> data, int length);
        void send_mcc_data();

    private:
        void send_helper(int messageID, void *data, int length);
};

#endif