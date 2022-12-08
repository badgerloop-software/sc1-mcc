#include "can.h"
#include "stdio.h"

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

void printTable(uint16_t curGPIO, float curRPM, float curAccel,
                float curBrake) {
  // Wipe screen
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

  // Table printout
  printf("  Signal Name  |  Value\n");
  printf("  RPM          |  %4.2f\n", curRPM);
  printf("  Accel V      |  %4.2f\n", curAccel);
  printf("  Brake V      |  %4.2f\n", curBrake);
  printf("  Power        |  %s\n", (((curGPIO >> 0) & 0x1) ? "On" : "Off"));
  printf("  Direction    |  %s\n", (((curGPIO >> 1) & 0x1) ? "Forward" : "Reverse"));  
  printf("  Brake        |  %s\n", (((curGPIO >> 2) & 0x1) ? "On" : "Off"));
  printf("  Eco          |  %s\n", (((curGPIO >> 3) & 0x1) ? "Eco" : "Sport"));
  printf("  Crz Enable   |  %s\n", (((curGPIO >> 4) & 0x1) ? "On" : "Off"));
  printf("  Crz Mode     |  %s\n", (((curGPIO >> 5) & 0x1) ? "B" : "A"));
  printf("  MC Status    |  %s\n", (((curGPIO >> 6) & 0x1) ? "Error" : ""));

}

/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend() {
    int curMessage;
    int curGPIO;
    float curRPM;
    float acc_in;
    float brk_in;

    while (1) {
        // Wait for a message. Signaled by any bit in 32 bit flag being set
        // Flag automatically cleared
        //curMessage = queueFlags.wait_any(0xFFFFFFFF);

        // Send it
        //canBus.write(outputQueue[curMessage]);
        curRPM = *(float*)outputQueue[0].data;
        curGPIO = *(uint16_t*)outputQueue[1].data;
        acc_in = *(float*)outputQueue[2].data;
        brk_in = *(float*)outputQueue[3].data;

        printf("Raw Values: %4.2f\t%u\t%4.2f\t%4.2f\n", curRPM, curGPIO, acc_in, brk_in);
        // printTable(curGPIO, curRPM, acc_in, brk_in);
        wait_us(5000000);
    }
}

