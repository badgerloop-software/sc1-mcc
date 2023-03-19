#include "can.h"
#include "stdio.h"

// Disables CAN. Gives direct console control over outputs. Printout of current signals
#define TEST_MODE 0

// Signaled for when to send CAN message 
EventFlags queueFlags;


// Define CAN pins
CAN canBus(PA_11, PA_12);


/// Initializes CAN on pins and output queue
void initCAN(int frequency) {
    canBus.frequency(frequency);
    // TODO: ADD READ CODE
}


#if TEST_MODE
// Serial console setup
static BufferedSerial pc(USBTX, USBRX);

// Print current variables
void printTable(uint16_t gpio, float rpm, float accel,
                float brake) {
  // Wipe screen
  printf("\e[1;1H\e[2J");

  // Table printout
  printf("  Signal Name  |  Value\n");
  printf("  Cur State    |  %d\n", curState);
  printf("  RPM          |  %4.2f\n", rpm);
  printf("  Accel V      |  %4.2f\n", accel);
  printf("  Power        |  %s\n", (((gpio >> 0) & 0x1) ? "On" : "Off"));
  printf("  Direction    |  %s\n", (((gpio >> 1) & 0x1) ? "Reverse" : "Forward"));  
}

// Rudimentary frontend to give serial console control over MCC outputs
// Essentially replaces purpose of inbound CAN messages
void command_loop() {
    char buf[1];
    
    printf("Press any key to change settings\n");
    if (pc.read(buf, 1) > 0) {
        pc.set_blocking(true);
        printf("Press a to set Acc_out, p to set Power, d to set direction, any other to cancel\n");
        pc.read(buf, 1);
        switch (buf[0]) {
            case 'a':
                printf("Use u to increase voltage, d to decrease in increments of 0.1V, x to exit, e for emergency stop. Any other key to see current voltage\n");
                while (1) {
                    pc.read(buf, 1);
                    if (buf[0] == 'x') {
                        break;
                    }
                    switch (buf[0]) {
                        case 'e':
                            curAcc = 0;
                            break;
                        case 'u':
                            curAcc += 0.1;
                            if (curAcc > 3.3) curAcc = 3.3; // Saturate
                            break;
                        case 'd':
                            curAcc -= 0.1;
                            if (curAcc < 0) curAcc = 0;
                            break;
                        default:
                            printf("%f V\n", curAcc);
                            break;
                    }
                }
                break;
            case 'p':
                printf("Press 0 to shut off, 1 to power on\n");
                pc.read(buf, 1);
                if (buf[0] == '0') {
                    curGPIO &= ~(0x1 << 0);
                    printf("Shutting down motor");
                } else if (buf[0] == '1') {
                    curGPIO |= (0x1 << 0);
                    printf("Powering on motor");
                } else {
                    printf("Unrecognized command. Exiting\n");
                }
                break;
            case 'd':
                printf("Press 0 to set forward, 1 to set reverse\n");
                pc.read(buf, 1);
                if (buf[0] == '0') {
                    curGPIO &= ~(0x1 << 1);
                    printf("Set desired direction to forward\n");
                } else if (buf[0] == '1') {
                    curGPIO |= (0x1 << 1);
                    printf("Set desired direction to reverse\n");
                } else {
                    printf("Unrecognized command. Exiting\n");
                }
                break;
            default:
                printf("Cancelling\n");
                break;
        }

        pc.set_blocking(false);
    }
}
#endif


/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend() {
    // Test mode replaces CAN system with serial console
    #if TEST_MODE
    char buf[1] = {0};
    pc.set_blocking(false);
    pc.set_baud(9600);
    pc.set_format(8, BufferedSerial::None, 1);
    while (1) {
        printTable(curGPIO, curRPM, curAcc, curBrk);
        command_loop();
        wait_us(1500000);
    }

    // CAN Send system
    #else
    uint32_t curMessage = 0;

    // Collection of message metadata
    uint32_t canIDs[TOTAL_SIG] = {0x200, 0x201, 0x202, 0x203};
    void* dataPtrs[TOTAL_SIG] = {&curRPM, &curGPIO, &curAcc, &curBrk};
    uint8_t lengths[TOTAL_SIG] = {4, 2, 4, 4};

    while (1) {
        // Wait for a message. Signaled by any bit in 32 bit flag being set
        // Flag automatically cleared
        // Limit for 10ms for canbus error checking, should later match update rate?
        // On timeout, all but MSB is set to 1 by function
        curMessage = queueFlags.wait_any_for(0xFFFFFFFF >> (32 - TOTAL_SIG), 10ms);
        if (canBus.tderror() || canBus.rderror()) {
            canBus.reset();
        }

        // Check if flag was from updated reading and not timeout
        if (curMessage < (0x1 << TOTAL_SIG)) {

            // Check which readings are new, send those
            for (int i = 0; i < TOTAL_SIG; i++) {
                if (curMessage & (0x1 << i)) {
                    if (!canBus.write(CANMessage(canIDs[i], (char*)dataPtrs[i], lengths[i]))) {
                        printf("Failed to send message\n");
                    }
                }
            }
        }
        curMessage = 0;
    }
    #endif
}

