#include "analog.h"
#include "can.h"
#include "common.h"
#include "drive.h"
#include "gpio.h"
#include "mbed.h"
#include "monitor.h"
#include "include/mccstate.h"
#include "include/mccstatelist.h"
#include "include/mcc.h"


// Main code. Should initialize devices and then transform into the CAN loop
 MccState* nextState;

int main()
{
    // AnalogOut BRK_OUT(A4);
    // BRK_OUT.write(0);
    // Initializes devices
    initGPIO(250ms);
    initAnalog(250ms);
    drive_initDrive();
    initCAN(125000);
    Mcc* mcc = new Mcc();
 //   monitor_initMonitor(30000ms, mcc);
    while(1) {

        // current state of Mcc
        MccState* currentState = mcc->getCurrentState();

        // checks if in Off state
        if(currentState == &OFF::getInstance()) {
            printf("In OFF state");
            // checks to see if power is on and parking brake is enabled. If so, switches to Park state
            if(READ_BIT_POS(curGPIO, POWER_BIT) == POWER_ON && READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_ON) {
                nextState = &PARK::getInstance();
                mcc->setState(PARK::getInstance());
            }
        }

        // checks if in Park state
        else if(currentState == &PARK::getInstance()) {
            printf("In PARK state");
            // checks to see if Forward/Reverse switch is in Forward and parking brake is disabled. If so, switches to Forward state
            if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_FORWARD && READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_OFF) {
                nextState = &FORWARD::getInstance();
                mcc->setState(FORWARD::getInstance());
            }

            // checks to see if Forward/Reverse switch is in Reverse and parking brake is disabled. If so, switches to Reverse state
            else if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_REVERSE && READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_OFF) {
                nextState = &REVERSE::getInstance();
                mcc->setState(REVERSE::getInstance());
            }

            // checks to see if Main_Telem goes low. If so, switches to Off state
            else if(READ_BIT_POS(curGPIO, POWER_BIT) == POWER_OFF) {
                nextState = &OFF::getInstance();
                mcc->setState(OFF::getInstance());
            }
        }

        // checks if in Forward State
        else if(currentState == &FORWARD::getInstance()) {
            printf("In FORWARD state");
            if(READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_ON) { // if parking break is on, switch to park state
               nextState = &PARK::getInstance();
               mcc->setState(PARK::getInstance());
            }

            // change to reverse state if wanted direction is reverse and speed is equal to 0
            // TODO: Will need to modify the 0, it will never always just be 0. Need to set a range. 
            else if (READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_REVERSE && drive_getMPH() == 0) {
                nextState = &REVERSE::getInstance();
                mcc->setState(REVERSE::getInstance());
            }

             // if wanted direction is reverse but speed is not equal to 0, stay in forward state
             // until speed is equal to 0, display error message
            else if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_REVERSE && drive_getMPH() != 0) {
                nextState = &FORWARD::getInstance();
                mcc->setState(FORWARD::getInstance());
            }

            // checks to see if Main_Telem goes low. If so, switches to Off state
            else if(READ_BIT_POS(curGPIO, POWER_BIT) == POWER_OFF) {
                nextState = &OFF::getInstance();
                mcc->setState(OFF::getInstance());
            }

            else {
                currentState->execute();
            }
        }

        // checks if in Reverse State
        else if(currentState == &REVERSE::getInstance()) {
            printf("In REVERSE state");
            if(READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_ON) { // if parking break is on, switch to park state
                nextState = &PARK::getInstance();
                mcc->setState(PARK::getInstance());
            }

            // change to forward state if wanted direction is forward and speed is equal to 0
            else if (READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_FORWARD && drive_getMPH() == 0) {
                nextState = &FORWARD::getInstance();
                mcc->setState(FORWARD::getInstance());
            }

             // if wanted direction is forward but speed is not equal to 0, stay in reverse state
             // until speed is equal to 0, display error message
            else if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_FORWARD && drive_getMPH() != 0) {
                nextState = &REVERSE::getInstance();
                mcc->setState(REVERSE::getInstance());
            }

            // checks to see if Main_Telem goes low. If so, switches to Off state
            else if(READ_BIT_POS(curGPIO, POWER_BIT) == POWER_OFF) {
                nextState = &OFF::getInstance();
                mcc->setState(OFF::getInstance());
            }

            else {
                currentState->execute();
            }
        }
        float curGPIO_float = static_cast<float>(curGPIO);
        float* data[TOTAL_SIG] = {&curGPIO_float, &curRPM, &curAcc, &curBrk, &curPedal, &pedal_percent_pressed};
        printf("\nCalculated Analog Signal: %4.2f\n", calculate_pedal_press(analog_getCurPedal()));
        printTable(curGPIO, curRPM, curAcc, curBrk, curPedal, pedal_percent_pressed);
  //      CANSend(data);
     //   wait_us(6000000);
    }
}
