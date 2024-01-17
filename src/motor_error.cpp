#include "motor_error.h"

Timeout evaluationTimeout;
Timeout resetTimeout;

InterruptIn motorErrorPin(D0);

// The error type, determined based on input pulses
volatile ErrorType errorType = None;

// The pulse count, incremented with every rising edge of the motor error pin
static volatile int pulses = 0;

// Resets error type to None if no pulses have been recorded for the last RESET_DELAY
void reset()
{
    if (pulses == 0)
    {
        errorType = None;
    }
}

// Evaluates the motor error type based on the current number of pulses, resets pulse count
void evaluatePulses()
{
    switch (pulses)
    {
        case 0: errorType = None; break;
        case 1: errorType = OverCurrent; break;
        case 3: errorType = HallSensorFault; break;
        case 4: errorType = MotorLocked; break;
        case 5: errorType = SensorFault1; break;
        case 6: errorType = SensorFault2; break;
        case 8: errorType = HighBatteryVoltage; break;
        case 9: errorType = ControllerOverheat; break;
        default: errorType = Unknown; break;
    }
    pulses = 0;

    // reset() will be called after delay RESET_DELAY
    resetTimeout.attach(&reset, RESET_DELAY);

    // Note: when evaluatePulses() is called again before a prior delay has elapsed, the
    // prior delay is disregarded; reset() will only be called at the end of the new delay
}

// Increments the pulse count and calls the evaluation method with delay SEQUENCE_DELAY
void newPulse()
{
    pulses ++;

    // evaluatePulses() will be called after delay SEQUENCE_DELAY
    evaluationTimeout.attach(&evaluatePulses, SEQUENCE_DELAY);

    // Note: when newPulse() is called again before a prior delay has elapsed, the prior
    // delay is disregarded; evaluatePulses() will only be called at the end of the new delay
}

// Start motor error interpretation
void startErrorInterpretation()
{
    // newPulse() is called with every rising edge of the motor error pin
    motorErrorPin.rise(&newPulse);
}

// Represents the error type as a string
const char * errorString(ErrorType errorType)
{
    switch (errorType)
    {
        case None: return "None";
        case OverCurrent: return "Over Current";
        case HallSensorFault: return "Hall Sensor Fault";
        case MotorLocked: return "Motor Locked";
        case SensorFault1: return "Sensor Fault 1";
        case SensorFault2: return "Sensor Fault 2";
        case HighBatteryVoltage: return "High Battery Voltage";
        case ControllerOverheat: return "Controller Overheat";
        case Unknown: return "Unknown";
    }
}