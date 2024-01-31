#include "can_mcc.h"

CANMCC::CANMCC(PinName rd, PinName td, int frequency): CANManager(rd, td, frequency) {}

void CANMCC::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {

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
}
