#include "can.h"
#include <cstring>


// Queue for outgoing CAN messages
// 0     |      RPM
// 1     |      GPIO
// 2     |      Acc
// 3     |      Brake

CANMessage outputQueue[4];

// 32 bit flag used to indicate if any messages are to be sent
// Updating functions will set flag once new value updated in queue
// CANSend will loop through queue cyclically to prevent starvation
// Use same value for flag as in queue (ex RPM avail to send is indicated by bit 0)
EventFlags queueFlags;

// Define CAN pins
CAN canBus(PA_11, PA_12);


/// Initializes CAN on pins and output queue
//  Returns 0 on success, -1 on failure
int initCAN(int frequency) {
    canBus.frequency(frequency);

    // RPM CAN Frame
    char* RPMData = (char*)malloc(4);
    *(int*)RPMData = 0;
    CANMessage rpmTemplate(200, RPMData, 32, CANData, CANStandard);
    outputQueue[0] = rpmTemplate;

    // GPIO CAN Frame
    char* GPIOData = (char*)malloc(2);
    *(short*)GPIOData = 0;
    CANMessage GPIOTemplate(201, GPIOData, 16, CANData, CANStandard);
    outputQueue[1] = GPIOTemplate;

    // Acc CAN Frame
    char* AccData = (char*)malloc(4);
    *(int*)AccData = 0;
    CANMessage AccTemplate(202, AccData, 32, CANData, CANStandard);
    outputQueue[2] = AccTemplate;

    // Brk CAN Frame
    char* BrkData = (char*)malloc(4);
    *(int*)BrkData = 0;
    CANMessage BrkTemplate(203, BrkData, 32, CANData, CANStandard);
    outputQueue[3] = BrkTemplate;

    return 0;
}


/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend() {
    uint8_t index = 0;

    while (1) {
        if (queueFlags.get() & (0x1 << index)) {
            canBus.write(outputQueue[index]);
            queueFlags.clear(0x1 << index);
        }

        index = (index + 1) % 4;
    }
}

