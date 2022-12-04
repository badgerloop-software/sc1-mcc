#include "can.h"


// Queue for outgoing CAN messages
// 0     |      RPM
// 1     |      GPIO
// 2     |      Acc
// 3     |      Brake

Semaphore queueSem(1);
CANMessage outputQueue[4];

// 32 bit flag used to indicate if any messages are to be sent
// Updating functions will set flag once new value updated in queue
// CANSend will loop through queue cyclically to prevent starvation
// Use same value for flag as in queue (ex RPM avail to send is indicated by bit 0)
EventFlags queueFlags;

