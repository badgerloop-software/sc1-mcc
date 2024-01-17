#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include "mbed.h"
#include "telemetry.h"
#include "digital.h"
#include "analog.h"
#include "PID.h"

#define MIN_MOVING_SPEED 3.0        // speed threshold for idle state


// PID macros
#define POWER_P_PARAM 0.0
#define POWER_I_PARAM 0.0
#define POWER_D_PARAM 0.0
#define SPEED_P_PARAM 1.0/720
#define SPEED_I_PARAM 0.0
#define SPEED_D_PARAM 0.0

// limits for power and speed PIDs.
#define MIN_POWER 0.0
#define MAX_POWER 1.0
#define MIN_RPM 0.0
#define MAX_RPM 48.0

// limits for outputs of PID
// is 0.0 to 1.0 due to how AnalogOut pins work.
#define MIN_OUT 0.0
#define MAX_OUT 1.0

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