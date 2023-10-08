#include "speed.h"
#include "vector"

Ticker rpmTicker;

InterruptIn speedPin(D1);


volatile float rpm = 0;
volatile float mph = 0;

// Track past pulses for precision
static vector<unsigned int> previousPulses;
// How often speed computation happens
static std::chrono::milliseconds speedInterval = 0ms;
// Number of PWM pulses since last calculation
static volatile unsigned int speedPulses = 0;


// Increments the speedPulses counter
void increment()
{
    speedPulses++;
}

// Calculates the sum of the elements of a vector
int sum(vector<unsigned int> values)
{
    int total = 0;

    for (int i = 0; i < values.size(); i++)
    {
        total += values.at(i);
    }
    return total;
}

// Calculates RPM, call frequency depends on interval parameter of startRpmCalculation
void calculateSpeed()
{
    // Variable to track array update position
    static int calculationCounter = 0;

    // Replace the oldest remembered pulse count in the vector with a new one
    previousPulses.at(calculationCounter % previousPulses.size()) = speedPulses;
    speedPulses = 0;
    calculationCounter ++;

    // Calculate rpm based the total number of pulses in the previousPulses vector
    float revolutions = sum(previousPulses) / 48.0f;
    rpm = revolutions / (speedInterval.count() * previousPulses.size() / 1000.0f / 60.0f);
    mph = rpm * WHEEL_CIRCUMFERENCE / 12.0f / 5280.0f * 60;


    /*
    Note: since we remember 1250 ms of previous pulses, every additional pulse in our
    previousPulses vector causes an rpm increase of 1, which is our accuracy boundary.

    Note: based on testing, it appears that the actual sample rate is slightly slower than
    every interval ms (as assumed for calculations), leading to an rpm reading that is
    slightly higher than what it actually is.
    */
}

// Starts repeating rpm calculation every interval milliseconds
void startSpeedCalculation(std::chrono::milliseconds interval)
{
    speedInterval = interval;

    // Attach the address of the increment function to the rising edge of the speed pin
    speedPin.rise(&increment);

    // Call calculateRpm() every interval ms
    rpmTicker.attach(calculateSpeed, interval);

    // Set size of previousPulses such that we remember appox 1250 ms of previous pulses
    previousPulses.resize(1250 / interval.count(), 0);
}
