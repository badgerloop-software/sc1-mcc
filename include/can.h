#ifndef __CAN_H__
#define __CAN_H__

#include "mbed.h"


/// Initializes CAN on pins and output queue
//  Returns 0 on success, -1 on failure
int initCAN(int frequency);


/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend();

#endif