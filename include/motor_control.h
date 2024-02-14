#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include "mbed.h"
#include "telemetry.h"
#include "digital.h"
#include "analog.h"
#include "PID.h"

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

        // cruise control variables
        PID *curr_PID;
        PID power_PID, speed_PID;
};

#endif