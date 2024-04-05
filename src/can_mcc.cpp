#include "can_mcc.h"

volatile float dischargeCurrentLimit;

CANMCC::CANMCC(PinName rd, PinName td, int frequency): CANManager(rd, td, frequency) {
    filter(0, 0xFFF);
}

void CANMCC::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
    switch (messageID) {
        case 0x103:
            dischargeCurrentLimit = (float)(*(uint16_t*)data.get()) * CONST_CURR_SAFETY_MULT;
            break;
        default:
            break;
    }
}

void CANMCC::send_mcc_data() {
    // Digital signals
    this->sendMessage(0x200, (void*)&digital_data, 1, 1ms);
    this->sendMessage(0x201, (void*)&cruzMode, 1, 1ms);
    this->sendMessage(0x202, (void*)&mccState, 1, 1ms);
    
    // send floats
    float float_helper = acceleratorPedal;
    this->sendMessage(0x203, &float_helper, 4, 1ms);
    float_helper = regenerativeBraking;
    this->sendMessage(0x204, &float_helper, 4, 1ms);
    float_helper = motorSpeedSetpoint;
    this->sendMessage(0x205, &float_helper, 4, 1ms);
    float_helper = mph;
    this->sendMessage(0x206, &float_helper, 4, 1ms);
    float_helper = brakeSensor;
    this->sendMessage(0x207, &float_helper, 4, 1ms);
}
