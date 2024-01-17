#ifndef _MOTORERROR_H_
#define _MOTORERROR_H_

#include "telemetry.h"

// The minimum break between two consecutive sequences of pulses
#define SEQUENCE_DELAY 1000ms

// The time without pulses after which the error type will be reset as None
#define RESET_DELAY 3000ms


// Start the motor error interpretation by acting on each rising edge of the motor error pin
void startErrorInterpretation();

// Represents the error type as a string
const char * errorString(ErrorType errorType);


#endif