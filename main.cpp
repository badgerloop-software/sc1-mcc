#include "mbed.h"
#include "analog.h"
#include "can_mcc.h"
#include "digital.h"
#include "motor_control.h"
#include "motor_error.h"
#include "speed.h"
#include "telemetry.h"

#define DEBUG_PRINT 1

#define ANALOG_CALC_INTERVAL 50ms
#define DIGITAL_CALC_INTERVAL 50ms
#define CAN_SEND_INTERVAL   50ms

// time between checking for transitions in state machine in us
#define SM_TRANSITION_INTERVAL 50ms

#define CAN_RX PA_11
#define CAN_TX PA_12

#if DEBUG_PRINT
void debugPrint() {
    printf("\e[1;1H\e[2J");
    //state printout
    switch (mccState) {
        case MCCStates::OFF :
            printf("State: OFF\n");
            break;
        case MCCStates::PARK :
            printf("State: PARK\n");
            break;
        case MCCStates::IDLE :
            printf("State: IDLE\n");
            break;
        case MCCStates::FORWARD :
            printf("State: FORWARD\n");
            break;
        case MCCStates::REVERSE :
            printf("State: REVERSE\n");
            break;
        case MCCStates::CRUISE_POWER :
            printf("State: CRUISE\n");
            break;
        case MCCStates::CRUISE_SPEED:
            printf("State: CRUISE\n");
            break;
        default:
            printf("Unknown state?\n");
            break;
    }

    // inputs
    printf("INPUTS-------------------------------\n");
    printf("Motor Power: %s\n", digital_data.motorPower ? "On" : "Off");
    printf("Forward and Reverse: %s\n", digital_data.forwardAndReverse ? "Reverse" : "Forward");
    printf("RPM: %f\n", rpm);
    printf("MPH: %f\n", mph);
    printf("Foot Brake: %f\n", brakeSensor);
    printf("Park Brake: %s\n", digital_data.parkBrake ? "On" : "Off");
    printf("acceleratorPedal: %f\n", acceleratorPedal);
    switch(cruzMode) {
        case CRUZ_MODE::OFF :
            printf("cruzMode: OFF\n");
            break;
        case CRUZ_MODE::POWER :
            printf("cruzMode: POWER\n");
            break;
        case CRUZ_MODE::SPEED :
            printf("cruzMode: SPEED\n");
            break;
    }
    printf("motorSpeedSetpoint: %f\n", motorSpeedSetpoint);
    printf("Motor Error: %s\n", errorString(errorType));
    
    // debug
    printf("ecomode %s\n", digital_data.ecoMode ? "On" : "Off");
    printf("regenbraking: %f\n", regenerativeBraking);
    printf("speed_pid_compute: %f\n", speed_pid_compute);
}
#endif

int main()
{
    // Set terminal to fast print
    BufferedSerial terminal(USBTX, USBRX, 115200);

    // read RPM and MPH every 0.01 second
    startSpeedCalculation();

    // read all analog inputs every 0.01 second
    initAnalog(ANALOG_CALC_INTERVAL);

    // read all digital inputs every 0.01 second
    initDigital(DIGITAL_CALC_INTERVAL);

    // initialize state machine
    MCCState state_machine(SM_TRANSITION_INTERVAL);

    // error LED reading
    //startErrorInterpretation();

    CANMCC canBus(CAN_RX, CAN_TX);

#if DEBUG_PRINT
    int print_counter = 0;
#endif

    while(true){
#if DEBUG_PRINT
        // Print every second
        print_counter++;
        if (print_counter > 1000 / CAN_SEND_INTERVAL.count()) debugPrint();
#endif
    
        canBus.send_mcc_data();
        canBus.runQueue(CAN_SEND_INTERVAL);
    }
}
