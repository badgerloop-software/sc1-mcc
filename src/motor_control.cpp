#include "motor_control.h"

volatile MCCStates mccState = MCCStates::OFF;
volatile float speed_pid_compute = 0.0;

// set default state to OFF
MCCState::MCCState(std::chrono::microseconds ticker_delay) : state(MCCStates::OFF), 
        power_PID(POWER_D_PARAM, POWER_I_PARAM, POWER_P_PARAM, std::chrono::duration<float>(ticker_delay).count()), 
        speed_PID(SPEED_P_PARAM, SPEED_I_PARAM, SPEED_D_PARAM, std::chrono::duration<float>(ticker_delay).count()) {
    // initialize Ticker to run the transition method every ticker_delay seconds
    state_updater.attach(callback(this, &MCCState::transition), ticker_delay);
    // set limits of inputs and outputs
    power_PID.setInputLimits(MIN_POWER, MAX_POWER);
    power_PID.setOutputLimits(MIN_OUT, MAX_OUT);
    speed_PID.setInputLimits(MIN_RPM, MAX_RPM);
    speed_PID.setOutputLimits(MIN_OUT, MAX_OUT);
    speed_PID.setMode(AUTO_MODE);
    
    curr_PID = &power_PID;
}

MCCState::~MCCState() {
    state_updater.detach();
}

void MCCState::transition() {
    switch (state) {
        case MCCStates::PARK:
            if (!parkBrake) {
                state = MCCStates::IDLE;
            }
            // OUTPUT: make sure the motor isn't spinning when it's in PARK
            setAccOut(0.0);
            break;

        case MCCStates::IDLE:
            // OUTPUT: set acc_out pin based on acc_in from the pedal
            // this allows us to accelerate into REVERSE or FORWARD states
            setAccOut(acceleratorPedal);
            setDirectionOutput(digital_data.forwardAndReverse);

            // car moving fast, transition to locked direction state
            if (rpm >= MIN_MOVING_SPEED) {
                if (digital_data.forwardAndReverse == FORWARD_VALUE) {
                    state = MCCStates::FORWARD;
                } else {
                    state = MCCStates::REVERSE;
                }
            } 
            break;

        case MCCStates::REVERSE:
            if (rpm < MIN_MOVING_SPEED) {
                state = MCCStates::IDLE;
            }
            // OUTPUT: set acc_out pin based on acc_in from the pedal
            setAccOut(acceleratorPedal);
            // OUTPUT: set fr_out to reverse
            setDirectionOutput(REVERSE_VALUE);
            break;

        case MCCStates::FORWARD:
            if (rpm < MIN_MOVING_SPEED) {
                state = MCCStates::IDLE;

            } else if (digital_data.cruiseEnabled) {
                if (cruzMode == CRUZ_MODE::POWER) {
                    curr_PID = &power_PID;
                    state = MCCStates::CRUISE_POWER;
                } else if (cruzMode == CRUZ_MODE::SPEED) {
                    curr_PID = &speed_PID;
                    state = MCCStates::CRUISE_SPEED;
                }
            }
            
            // OUTPUT: set acc_out pin based on acc_in from the pedal
            setAccOut(acceleratorPedal);
            // OUTPUT: set fr_out to forward.
            setDirectionOutput(FORWARD_VALUE);
            break;

        case MCCStates::CRUISE_POWER:
            if (cruzMode == CRUZ_MODE::OFF) {
                state = MCCStates::FORWARD;
                break;
            } else if (cruzMode == CRUZ_MODE::SPEED) {
                curr_PID = &speed_PID;
                state = MCCStates::CRUISE_SPEED;
                break;
            }
            // TODO: not enough stuff for power right now (10/22)
            // get current motor power
            // set setPoint to target power
            // setAccOut(compute());
            break;

        case MCCStates::CRUISE_SPEED:
            if (cruzMode == CRUZ_MODE::OFF) {
                state = MCCStates::FORWARD;
                break;
            } else if (cruzMode == CRUZ_MODE::POWER) {
                curr_PID = &power_PID;
                state = MCCStates::CRUISE_POWER;
                break;
            }
            // set current rpm for speed_PID
            speed_PID.setProcessValue(rpm);
            // set target for speed_PID
            speed_PID.setSetPoint(motorSpeedSetpoint);
            // 
            speed_pid_compute = speed_PID.compute();
            // set accelerator 
            setAccOut(speed_pid_compute);
            break;

        // OFF state as our default
        default:
            if (digital_data.motorPower) {
                state = MCCStates::PARK;
                break;
            }
            // OUTPUT: make sure the motor isn't spinning when it's OFF
            setAccOut(0.0);
            // Set to known state since it is our default
            state = MCCStates::OFF;
            break;
    }

    // STATE MACHINE OVERRIDES AND SAFETY CHECKS

    // use if, else if because we should only switch to 1 state.
    // put higher priority checks up top.
    if (!digital_data.motorPower) {
        // the motor is off, so go into OFF state
        state = MCCStates::OFF;
        setAccOut(0.0);
    } else if (parkBrake) {
        state = MCCStates::PARK;
        setAccOut(0.0);
    }

    // set motor output to 0 if either park or foot brake are being pressed
    if (digital_data.brakeStatus) {
        setAccOut(0.0);
    }

    // set brakeLED based on brakeStatus(foot and park brake) and regenerativeBraking
    setBrakeLEDOutput(digital_data.brakeStatus || regenerativeBraking > REGEN_ON_THRESHOLD);

    mccState = this->state; // used to send state over CAN
}

// returns the current state of the state machine
MCCStates MCCState::get_state() {
    return state;
}

