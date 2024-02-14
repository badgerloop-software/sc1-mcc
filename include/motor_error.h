#ifndef _MOTORERROR_H_
#define _MOTORERROR_H_

#include "telemetry.h"

// Start the motor error interpretation by acting on each rising edge of the motor error pin
void startErrorInterpretation();

// Represents the error type as a string
const char * errorString(ErrorType errorType);


#endif