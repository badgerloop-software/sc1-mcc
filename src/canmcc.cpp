#include "canmcc.h"

volatile bool parkBrake = false;

CANMCC::CANMCC(PinName rd, PinName td, MCCState* stateMachine, int frequency): CANManager(rd, td, frequency) {
    this->stateMachine = stateMachine;
}

void CANMCC::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
    // Variable to track number of received messages
    static unsigned long messageCount = 0;
    messageCount++;

    // Park brake from main
    if (messageID == 0x100) {
        parkBrake = *data;
    }
}


void CANMCC::send_mcc_data() {
    // Digital signals
    this->sendMessage(0x200, (void*)&digital_data, 1, 1ms);
    this->sendMessage(0x201, (void*)&cruzMode, 1, 1ms);
    char charHelper = (char)this->stateMachine->get_state();
    this->sendMessage(0x202, &charHelper, 1, 1ms);
    
    //wait_us(8000);
    // send floats
    float float_helper = acceleratorPedal;
    this->sendMessage(0x203, &float_helper, 4, 1ms);
    float_helper = brakeStatus;
    this->sendMessage(0x204, &float_helper, 4, 1ms);
    float_helper = regenerativeBraking;
    this->sendMessage(0x205, &float_helper, 4, 1ms);
    //wait_us(8000);
    float_helper = motorSpeedSetpoint;
    this->sendMessage(0x206, &float_helper, 4, 1ms);
    float_helper = rpm;
    this->sendMessage(0x207, &float_helper, 4, 1ms);
    float_helper = mph;
    this->sendMessage(0x208, &float_helper, 4, 1ms);
}
