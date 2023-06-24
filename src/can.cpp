#include "can.h"

#include "common.h"
#include "stdio.h"

#define TEST_MODE 1

// Variables
EventFlags queueFlags;

// Define CAN pins
CAN canBus(CAN_RD_PIN, CAN_TD_PIN);

/// Initializes CAN on pins and output queue
//  Returns 0 on success, -1 on failure
int initCAN(int frequency)
{
    canBus.frequency(frequency);

    return 0;
}

#if TEST_MODE
void printTable(uint16_t gpio, float rpm, float accel, float brake,
                float pedal, float pedal_percent)
{
    // Wipe screen
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    // Table printout
    printf("  Signal Name  |  Value\n");
    printf("  RPM          |  %4.2f\n", rpm);
    printf("  Accel V      |  %4.2f\n", accel);
    printf("  Brake V      |  %4.2f\n", brake);
    printf("  Pedal V      |  %4.2f\n", pedal);
    printf("  Pedal %      |  %4.2f\n", pedal_percent);
    printf("  Power        |  %s\n", (((gpio >> 0) & 0x1) ? "On" : "Off"));
    printf("  Direction    |  %s\n",
           (((gpio >> 1) & 0x1) ? "Forward" : "Reverse"));
    printf("  Brake        |  %s\n", (((gpio >> 2) & 0x1) ? "OFF" : "ON"));
    printf("  Eco          |  %s\n", (((gpio >> 3) & 0x1) ? "Eco" : "Sport"));
    printf("  Crz Enable   |  %s\n", (((gpio >> 4) & 0x1) ? "On" : "Off"));
    printf("  Crz Mode     |  %s\n", (((gpio >> 5) & 0x1) ? "B" : "A"));
    printf("  MC Status    |  %s\n", (((gpio >> 6) & 0x1) ? "Error" : ""));
}
#endif

/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend(float *dataPtrs[TOTAL_SIG])
{
    uint32_t curMessage = 0;

    // Create message templates
    uint32_t canIDs[TOTAL_SIG] = { 0x200, 0x201, 0x202, 0x203, 0x204, 0x205 };
    // void* dataPtrs[TOTAL_SIG] = data;
    uint8_t lengths[TOTAL_SIG] = { 4, 2, 4, 4, 4, 4 };

    while(1)
    {
        // Wait for a message. Signaled by any bit in 32 bit flag being set
        // Flag automatically cleared
        // Limit for 10ms for canbus error checking, should later match
        // update rate?
        curMessage
            = queueFlags.wait_any_for(0xFFFFFFFF >> (8 - TOTAL_SIG), 10ms);
        if(canBus.tderror() || canBus.rderror())
        {
            canBus.reset();
        }

#if TEST_MODE
        printTable(curGPIO, curRPM, curAcc, curBrk, curPedal,
                   pedal_percent_pressed);
#endif

        // Check if flag was from updated reading and not timeout
        if(curMessage < (0x1 << TOTAL_SIG))
        {
            // Check which readings are new, send those
            for(int i = 0; i < TOTAL_SIG; i++)
            {
                if(curMessage & (0x1 << i))
                {
                    if(!canBus.write(CANMessage(canIDs[i], (char *)dataPtrs[i],
                                                lengths[i])))
                    {
                        printf("Failed to send message\n");
                    }
                }
            }
        }
        curMessage = 0;
    }
}
