#include "can.h"

// Variables
CANMessage outputQueue[TOTAL_SIG];
EventFlags queueFlags;


// Define CAN pins
CAN canBus(PA_11, PA_12);


/// Initializes CAN on pins and output queue
//  Returns 0 on success, -1 on failure
int initCAN(int frequency) {
    canBus.frequency(frequency);

    // Initialize id's in CANMessage frames
    for (int i = 0; i < TOTAL_SIG; i++) {
        outputQueue[i].id = MCC_OFFSET + i;
    }
    
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

