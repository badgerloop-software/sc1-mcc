#ifndef __CAN_H__
#define __CAN_H__

#include "mbed.h"

///////////////////////////////////////////////////////////////////////
/// SEE README IN "include" FOR HIGHER LEVEL DETAILS
///////////////////////////////////////////////////////////////////////
/// Current CAN Pins
// ---------------------------------
// Name        | Direction   | Pin   
// CAN_TX      | Output      | D2/CAN1_TD             
// CAN_RX      | Input       | D10/CAN1_RD
///////////////////////////////////////////////////////////////////////
/// CAN Logistics
//  CAN output is managed by CANSend function which main turns into 
//  after init calls. To add a signal:
//  1. Define a new offset (#define [NAME]_SLOT X) for your signal
//     Use the next available integer
//  2. Increment TOTAL_SIG by 1
//  3. Append information for your signal in message template arrays 
//     in CANSend 
//  4. When you want to send the message with your signals value, call
//     queueFlags.set([NAME]_SLOT). CANSend will do the rest.
///////////////////////////////////////////////////////////////////////


#define MCC_OFFSET  200 // CAN Address offset for MCC board
#define TOTAL_SIG   4   // Total # unique CAN signals from this board
#define RPM_SLOT    0
#define GPIO_SLOT   1
#define ACC_SLOT    2
#define BRK_SLOT    3

extern EventFlags queueFlags;

// Current state variables
extern float curAcc;
extern float curBrk;
extern float curRPM;
extern uint16_t curGPIO;


/// Initializes output queue id's
//  Data fields and sizes of all signals initialized in signal files
//  Returns 0 on success, -1 on failure
int initCAN(int frequency);


/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend();

#endif