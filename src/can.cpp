#include "can.h"
#include <cstring>


// 32 bit flag used to indicate if any messages are to be sent
// Updating functions will set flag once new value updated in queue
// CANSend will loop through queue cyclically to prevent starvation
// Use same value for flag as in queue (ex RPM avail to send is indicated by bit 0)
CANMessage outputQueue[TOTAL_SIG];
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
    int curMessage;

    while (1) {
        // Wait for a message. Signaled by any bit in 32 bit flag being set
        // Flag automatically cleared
        curMessage = queueFlags.wait_any(0xFFFFFFFF);

        // Send it
        canBus.write(outputQueue[curMessage]);
    }
}

