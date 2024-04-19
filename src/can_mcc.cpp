#include "can_mcc.h"

volatile float packOpenVoltage;
volatile float speed_target;

CANMCC::CANMCC(PinName rd, PinName td, int frequency): CANManager(rd, td, frequency) {
    filter(0, 0xFFF);
}

void CANMCC::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
    unsigned char *data_value = NULL;
    switch (messageID) {
        case 0x101:
            data_value = data.get();
            packOpenVoltage = ((data_value[2] << 8) + data_value[3]) * 0.1;
            break;
        case 0x026:
            speed_target = (float)(*(uint16_t*)data.get());
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
