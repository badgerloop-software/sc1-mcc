#include "speed.h"
#include "vector"

#define SPEED_CALC_INTERVAL 50ms
#define SPEED_INTERVAL 50 
#define ARRAY_SIZE 1250 / 50 // 1250ms/50ms

Ticker rpmTicker;

InterruptIn speedPin(PA_1);


volatile float rpm = 0;
volatile float mph = 0;

// Track past pulses for precision
static unsigned int previousPulses[ARRAY_SIZE];
// Number of PWM pulses since last calculation
static volatile unsigned int speedPulses = 0;


// Increments the speedPulses counter
void increment()
{
    speedPulses++;
}

// Calculates RPM, call frequency depends on interval parameter of startRpmCalculation
void calculateSpeed()
{
    // Variable to track array update position
    static int calculationCounter = 0;
    static unsigned int runningSum = 0; // running sum

    // Replace the oldest remembered pulse count in the vector with a new one
    calculationCounter = (calculationCounter + 1) % ARRAY_SIZE;
    runningSum = runningSum - previousPulses[calculationCounter] + speedPulses;
    previousPulses[calculationCounter] = speedPulses;
    speedPulses = 0;

    // Calculate rpm based the total number of pulses in the previousPulses vector
    float revolutions = runningSum * 0.020833333; // 1 / 48
    rpm = revolutions / (0.0000166666666 * SPEED_INTERVAL * ARRAY_SIZE); // 1 / 1000 / 60
    mph = rpm * WHEEL_CIRCUMFERENCE * 0.00094696; // 1 / 12 / 5280 * 60


    /*
    Note: since we remember 1250 ms of previous pulses, every additional pulse in our
    previousPulses vector causes an rpm increase of 1, which is our accuracy boundary.

    Note: based on testing, it appears that the actual sample rate is slightly slower than
    every interval ms (as assumed for calculations), leading to an rpm reading that is
    slightly higher than what it actually is.
    */
}

// Starts repeating rpm calculation every interval milliseconds
void startSpeedCalculation()
{
    // Attach the address of the increment function to the rising edge of the speed pin
    speedPin.rise(&increment);

    // Call calculateRpm() every interval ms
    rpmTicker.attach(calculateSpeed, SPEED_CALC_INTERVAL);
}
