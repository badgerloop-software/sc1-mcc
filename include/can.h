#ifndef __CAN_H__
#define __CAN_H__

#include "mbed.h"


/// *** CAN OUTPUT LOGISTICS ***
//  
//  CANLoop continuously checks queue for messages to send
//  Signals have slot in queue, as specified below
//  To request an output message, corresponding bit of the queueFlag should be set
//  Data field of message is mapped to the internal variables in init functions
//  
//  Ex: GPIO sets flag (1UL << 1). Data field of outputQueue[1] is linked to curGPIO variable
//
//  When sent, will be given offset of 200 to indicate source from MCC (ex rpm sent at id 201)

#define MCC_OFFSET  200
#define RPM_SLOT    0
#define GPIO_SLOT   1
#define ACC_SLOT    2
#define BRK_SLOT    3

extern CANMessage outputQueue[4];
extern EventFlags queueFlags;


/// Initializes output queue id's
//  Data fields and sizes of all signals initialized in signal files
//  Returns 0 on success, -1 on failure
int initCAN(int frequency);


/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend();

#endif