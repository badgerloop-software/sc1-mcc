#include "can.h"
#include "stdio.h"

extern uint16_t curGPIO;
extern float curRPM;
extern float acc_in;
extern float brk_in;


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

void printTable(uint16_t gpio, float rpm, float accel,
                float brake) {
  // Wipe screen
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

  // Table printout
//  printf("  Signal Name  |  Value\n");
  printf("  RPM          |  %4.2f\n", rpm);
//   printf("  Accel V      |  %4.2f\n", accel);
//   printf("  Brake V      |  %4.2f\n", brake);
//   printf("  Power        |  %s\n", (((gpio >> 0) & 0x1) ? "On" : "Off"));
//   printf("  Direction    |  %s\n", (((gpio >> 1) & 0x1) ? "Forward" : "Reverse"));  
//   printf("  Brake        |  %s\n", (((gpio >> 2) & 0x1) ? "On" : "Off"));
//   printf("  Eco          |  %s\n", (((gpio >> 3) & 0x1) ? "Eco" : "Sport"));
//   printf("  Crz Enable   |  %s\n", (((gpio >> 4) & 0x1) ? "On" : "Off"));
//   printf("  Crz Mode     |  %s\n", (((gpio >> 5) & 0x1) ? "B" : "A"));
//   printf("  MC Status    |  %s\n", (((gpio >> 6) & 0x1) ? "Error" : ""));

}

/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend() {
    int curMessage = 0;

    while (1) {
        // Wait for a message. Signaled by any bit in 32 bit flag being set
        // Flag automatically cleared
        //curMessage = queueFlags.wait_any(0xFFFFFFFF);

        // Send it
        //canBus.write(outputQueue[curMessage]);

        // printf("Raw Values: %4.2f\t%x\t%4.2f\t%4.2f\n", curRPM, curGPIO, acc_in, brk_in);
        printTable(curGPIO, curRPM, acc_in, brk_in);
        wait_us(500000);
    }
}

