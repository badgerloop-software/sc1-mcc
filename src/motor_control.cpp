#include "motor_control.h"

// TODO: Update with Serial signal
static bool parkBrake = 1;


// set default state to OFF
MCCState::MCCState(std::chrono::microseconds ticker_delay) : state(MCCStates::OFF) {
    // initialize Ticker to run the transition method every ticker_delay seconds
    state_updater.attach(callback(this, &MCCState::transition), ticker_delay);
}

MCCState::~MCCState() {
    state_updater.detach();
}

void MCCState::transition() {
    switch (state) {
        case MCCStates::PARK:
            if (parkBrake) {
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
            }
            if (cruzMode != CRUZ_MODE::OFF) {
                state = MCCStates::CRUISE;
                break;
            }
            // OUTPUT: set acc_out pin based on acc_in from the pedal
            setAccOut(acceleratorPedal);
            // OUTPUT: set fr_out to forward.
            setDirectionOutput(FORWARD_VALUE);
            break;

        case MCCStates::CRUISE:
            if (cruzMode == CRUZ_MODE::OFF) {
                state = MCCStates::FORWARD;
                break;
            }
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

