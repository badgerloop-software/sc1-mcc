#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include "mbed.h"
#include "telemetry.h"
#include "digital.h"
#include "analog.h"

#define MIN_MOVING_SPEED 3.0        // speed threshold for idle state


enum class MCCStates{
    OFF,
    PARK,
    IDLE,
    FORWARD,
    REVERSE,
    CRUISE
};

class MCCState {
    private:
        MCCStates state;
        Ticker state_updater;
        void transition();

    public:
        /*
            Sets up state machine to update at specified interval
        */
        MCCState(std::chrono::microseconds ticker_delay);
        ~MCCState();
        MCCStates get_state();
};

#endif