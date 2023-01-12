#include "can.h"
#include "stdio.h"

#define TEST_MODE 0

// Variables
EventFlags queueFlags;


// Define CAN pins
CAN canBus(PA_11, PA_12);


/// Initializes CAN on pins and output queue
//  Returns 0 on success, -1 on failure
int initCAN(int frequency) {
    canBus.frequency(frequency);
    
    return 0;
}

#if TEST_MODE
void printTable(uint16_t gpio, float rpm, float accel,
                float brake) {
  // Wipe screen
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

  // Table printout
  printf("  Signal Name  |  Value\n");
  printf("  RPM          |  %4.2f\n", rpm);
  printf("  Accel V      |  %4.2f\n", accel);
  printf("  Brake V      |  %4.2f\n", brake);
  printf("  Power        |  %s\n", (((gpio >> 0) & 0x1) ? "On" : "Off"));
  printf("  Direction    |  %s\n", (((gpio >> 1) & 0x1) ? "Forward" : "Reverse"));  
  printf("  Brake        |  %s\n", (((gpio >> 2) & 0x1) ? "On" : "Off"));
  printf("  Eco          |  %s\n", (((gpio >> 3) & 0x1) ? "Eco" : "Sport"));
  printf("  Crz Enable   |  %s\n", (((gpio >> 4) & 0x1) ? "On" : "Off"));
  printf("  Crz Mode     |  %s\n", (((gpio >> 5) & 0x1) ? "B" : "A"));
  printf("  MC Status    |  %s\n", (((gpio >> 6) & 0x1) ? "Error" : ""));
}
#endif

/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend() {
    int curMessage = 0;

    // Create message templates
    uint16_t canIDs[TOTAL_SIG] = {200, 201, 202, 203};
    void* dataPtrs[TOTAL_SIG] = {&curRPM, &curGPIO, &curAcc, &curBrk};
    uint8_t lengths[TOTAL_SIG] = {4, 2, 4, 4};

    while (1) {
        // Wait for a message. Signaled by any bit in 32 bit flag being set
        // Flag automatically cleared
        curMessage = queueFlags.wait_any(0xFFFFFFFF);


        #if TEST_MODE
        printTable(curGPIO, curRPM, curAcc, curBrk);
        char mes = 1;
        canBus.write(CANMessage(200, &mes, 1));
        wait_us(2500000);
        #else
        // Send it
        canBus.write(CANMessage(canIDs[curMessage], (char*)dataPtrs[curMessage], lengths[curMessage]));
        #endif

        
    }
}

