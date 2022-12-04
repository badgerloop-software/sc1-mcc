#ifndef __CAN_H__
#define __CAN_H__

#include "mbed.h"

// Queue for outgoing CAN messages
extern volatile Semaphore queueSem;
extern volatile uint8_t queueRead;
extern volatile uint8_t queueWrite;
extern volatile CANMessage outputQueue[10];
extern volatile EventFlags queueFlags;

/// Initializes CAN on pins and output queue
//  Returns 0 on success, -1 on failure
int CANInit(int frequency);

/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend();

#endif