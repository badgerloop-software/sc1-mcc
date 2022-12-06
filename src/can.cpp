#include "can.h"
#include <cstring>


// 32 bit flag used to indicate if any messages are to be sent
// Updating functions will set flag once new value updated in queue
// CANSend will loop through queue cyclically to prevent starvation
// Use same value for flag as in queue (ex RPM avail to send is indicated by bit 0)
CANMessage outputQueue[4];
EventFlags queueFlags;


// Define CAN pins
CAN canBus(PA_11, PA_12);


/// Initializes CAN on pins and output queue
//  Returns 0 on success, -1 on failure
int initCAN(int frequency) {
    canBus.frequency(frequency);

    outputQueue[RPM_SLOT].id = MCC_OFFSET + RPM_SLOT;
    outputQueue[GPIO_SLOT].id = MCC_OFFSET + GPIO_SLOT;
    outputQueue[ACC_SLOT].id = MCC_OFFSET + ACC_SLOT;
    outputQueue[BRK_SLOT].id = MCC_OFFSET + BRK_SLOT;
    
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

