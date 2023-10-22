#include "motor_control.h"


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
                break;
            }
            // OUTPUT: make sure the motor isn't spinning when it's in PARK
            setAccOut(0.0);
            break;

        case MCCStates::IDLE:
            // OUTPUT: set acc_out pin based on acc_in from the pedal
            // this allows us to accelerate into REVERSE or FORWARD states
            setAccOut(acceleratorPedal);
            setDirectionOutput(digital_data.forwardAndReverse);

            // park if brake is pressed and parking brake is set
            if (parkBrake) {
                state = MCCStates::PARK;
                break;
            }
            // motor is spinning
            if (rpm >= MIN_MOVING_SPEED) {
                // true = forward, false = reverse
                if (digital_data.forwardAndReverse) {
                    state = MCCStates::FORWARD;
                    break;
                } else {
                    state = MCCStates::REVERSE;
                    break;
                }
            }
            break;

        case MCCStates::REVERSE:
            if (rpm < MIN_MOVING_SPEED) {
                state = MCCStates::IDLE;
                break;
            }
            // OUTPUT: set acc_out pin based on acc_in from the pedal
            setAccOut(acceleratorPedal);
            // OUTPUT: set fr_out to reverse
            setDirectionOutput(REVERSE_VALUE);
            break;

        case MCCStates::FORWARD:
            // make sure we are stopped and pressing brake before transitioning to IDLE
            if (rpm < MIN_MOVING_SPEED) {
                state = MCCStates::IDLE;
                break;
            } else if (cruzMode == CRUZ_MODE::POWER && digital_data.cruiseEnabled) {
                curr_PID = &power_PID;
                state = MCCStates::CRUISE_POWER;
                break;
            } else if (cruzMode == CRUZ_MODE::SPEED && digital_data.cruiseEnabled) {
                curr_PID = &speed_PID;
                state = MCCStates::CRUISE_SPEED;
                break;
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
            // set accelerator 
            setAccOut(speed_PID.compute());
            break;

        // OFF state as our default
        default:
            if (digital_data.motorPower) {
                state = MCCStates::PARK;
                break;
            }
            // OUTPUT: make sure the motor isn't spinning when it's OFF
            setAccOut(0.0);
            break;
    }

    // use if, else if because we should only switch to 1 state.
    // put higher priority checks up top.
    if (!digital_data.motorPower) {
        // the motor is off, so go into OFF state
        state = MCCStates::OFF;
    } else if (parkBrake) {
        state = MCCStates::PARK;
        setAccOut(0.0);
    }

    // set motor output to 0 if both brake and pedal are being pressed
    // use 0.1 as the threshold to give it a bit of leeway
    if (brakeStatus > 0.1 && acceleratorPedal > 0.1) {
        setAccOut(0.0);
    }
}

// returns the current state of the state machine
MCCStates MCCState::get_state() {
    return state;
}

