#include "mbed.h"
#include "main.h"


#if MONITOR_MODE
static CAN canBus(PA_11, PA_12);
static uint16_t curGPIO = 0;
static float curRPM = 0;
static float curAccel = 0;
static float curBrake = 0;
static DigitalIn eco_telem(D7);
static DigitalIn main_telem(D9);
static AnalogOut ACCEL_OUT(A4);
static uint64_t counter = 0;

void printTable() {
  uint16_t gpio = curGPIO;
  float rpm = curRPM;
  float accel = curAccel;
  float brake = curBrake;
  // Wipe screen
  printf("%c", char(27));
  printf("[2J");
  printf("%c", char(27));
  printf("[H");

  // Table printout
//   printf("  Signal Name  |  Value\n");
//   printf("  RPM          |  %4.2f\n", rpm);
//   printf("  Accel V      |  %4.2f\n", accel);
//   printf("  Brake V      |  %4.2f\n", brake);
//   printf("  Power        |  %s\n", (((gpio >> 0) & 0x1) ? "On" : "Off"));
//   printf("  Direction    |  %s\n", (((gpio >> 1) & 0x1) ? "Forward" : "Reverse"));  
//   printf("  Brake        |  %s\n", (((gpio >> 2) & 0x1) ? "On" : "Off"));
//   printf("  Eco          |  %s\n", (((gpio >> 3) & 0x1) ? "Eco" : "Sport"));
//   printf("  Crz Enable   |  %s\n", (((gpio >> 4) & 0x1) ? "On" : "Off"));
//   printf("  Crz Mode     |  %s\n", (((gpio >> 5) & 0x1) ? "B" : "A"));
//   printf("  MC Status    |  %s\n", (((gpio >> 6) & 0x1) ? "Error" : ""));
  printf("  Main_Telem   |  %d\n",  main_telem.read());
  printf("  Eco_Telem    |  %d\n",  eco_telem.read());
  printf("  Accel_out    |  %.2f\n", ACCEL_OUT.read());
}


void rampup(void)
{
    
}

// Monitor CAN Bus for messages from the MCC
// Update current on screen values on arrival
int runMonitorApp() {
printf("starting...");
  // Initialize bus
  canBus.frequency(115200);
  Ticker t;
  t.attach(printTable, 500ms);
//   t.attach(rampup, 100ms);


  // Poll forever, updating printout on new data
  CANMessage msg;


  while (1) {

    ACCEL_OUT.write((counter++ % 100) / 100.0f);
    // Check if from MCC
    if(canBus.read(msg)) {
        if (msg.id & 0x200) {
            switch (msg.id & 0x7) {
                case 0:
                    curRPM = *(float*)msg.data;
                    break;
                case 1:
                    curGPIO = *(uint16_t*)msg.data;
                    break;
                case 2:
                    curAccel = *(float*)msg.data;
                    break;
                case 3:
                    curBrake = *(float*)msg.data;
                    break;
                default:
                    break;
            }
        }
        printf("Got message id: %x\n", msg.id);
    }
    
    if (canBus.tderror() || canBus.rderror()) {
        canBus.reset();
    } 
  }

  return 0;
}
#endif // MONITOR_MODE