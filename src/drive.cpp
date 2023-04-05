#include "drive.h"

#include "analog.h"
#include "gpio.h"

#include "mbed.h"
#include "math.h"

static int8_t counter = 0;
static Ticker speed_reader;
InterruptIn speed_pin(SPEED_PULSE_PIN);
static float rps = 0;


static void incCounter()
{
    counter++;
}

static int resetCounter()
{
    counter = 0;
    return counter;
}

static void readCounter()
{
    rps = (counter/TICKS_PER_REV) / WAIT_TIME_MSEC.count()*1000.0;
    resetCounter();
}


/**
* P U B L I C   F U N C T I O N S
*/

float drive_getRPS()
{
    return rps;
}

float drive_getMPH()
{
    return RPS_TO_MPH(rps);
}

float drive_getRPM()
{
    return RPS_TO_RPM(rps);
}

int drive_initDrive()
{
    speed_pin.rise(&incCounter);
    speed_reader.attach(&readCounter, WAIT_TIME_MSEC);
    return 1;
}
