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

int main()
{
    // Initializes devices
    initGPIO(250ms);
    initAnalog(250ms);
    drive_initDrive();
    initCAN(125000);
    monitor_initMonitor(300ms);
    Mcc* mcc = new Mcc();
    
    while(1) {
        // current state of Mcc
        MccState* currentState = mcc->getCurrentState();

        // checks if in Off state
        if(currentState == &MccOff::getInstance()) {
            // checks to see if power is on and parking brake is enabled. If so, switches to Park state
            if(READ_BIT_POS(curGPIO, POWER_BIT) == POWER_ON && READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_ON) {
                mcc->setState(MccPark::getInstance());
            }
        }

        // checks if in Park state
        else if(currentState == &MccPark::getInstance()) {
            
            // checks to see if Forward/Reverse switch is in Forward and parking brake is disabled. If so, switches to Forward state
            if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_FORWARD && READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_OFF) {
                mcc->setState(MccForward::getInstance());
            }

            // checks to see if Forward/Reverse switch is in Reverse and parking brake is disabled. If so, switches to Reverse state
            else if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_REVERSE && READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_OFF) {
                mcc->setState(MccReverse::getInstance());
            }
        }

        // checks if in Forward State
        else if(currentState == &MccForward::getInstance()) {
            if(READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_ON) { // if parking break is on, switch to park state
               mcc->setState(MccPark::getInstance());
            }

            // change to reverse state if wanted direction is reverse and speed is equal to 0
            else if (READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_REVERSE && drive_getMPH() == 0) {
                mcc->setState(MccReverse::getInstance());
            }

            // set acceleration if cruise control is set to on
            else if(READ_BIT_POS(curGPIO, CRZ_EN_BIT) == CRZ_ON) {
                mcc->setState(MccCruise::getInstance());
            }

             // if wanted direction is reverse but speed is not equal to 0, stay in forward state
             // until speed is equal to 0, display error message
            else if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_REVERSE && drive_getMPH() != 0) {
                mcc->setState(MccForward::getInstance());
            }
        }

        // checks if in Reverse State
        else if(currentState == &MccReverse::getInstance()) {
            if(READ_BIT_POS(curGPIO, BRAKE_BIT) == PARK_ON) { // if parking break is on, switch to park state
                mcc->setState(MccPark::getInstance());
            }

            // change to forward state if wanted direction is forward and speed is equal to 0
            else if (READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_FORWARD && drive_getMPH() == 0) {
                mcc->setState(MccForward::getInstance());
            }

             // if wanted direction is forward but speed is not equal to 0, stay in reverse state
             // until speed is equal to 0, display error message
            else if(READ_BIT_POS(curGPIO, DIRECTON_BIT) == DIRECTION_FORWARD && drive_getMPH() != 0) {
                mcc->setState(MccReverse::getInstance());
            }
        }

        // checks if in Cruise State (not sure what to do here)
        else {

        }
        CANSend();

        // maybe add delay?
    }
}
