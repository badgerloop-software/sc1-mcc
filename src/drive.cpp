#include "drive.h"

#include <cstdint>

//#include "PID.h"
#include "analog.h"
#include "common.h"
#include "gpio.h"
#include "math.h"
#include "mbed.h"

static int8_t    counter = 0;
static Ticker    speed_reader;
static Ticker    drive_writer;
InterruptIn      speed_pin(SPEED_PULSE_PIN);
static float     rps = 0;
//static PID       drive_controller(KP, KI, KD, PID_RATE_SEC);
static AnalogOut ACC_OUT(ACC_OUT_PIN);
static float     speed_setpoint = 0;

// P R I V A T E   F U N C T I O N S

/**
 * @return float - Pedal press in percentage [0, 1]
 */
static float drive_private_getPedalPress()
{
    return calculate_pedal_press(analog_getCurPedal());
}

/**
 * Used for interrupt callback - increments counter
 */
static void incCounter() { counter++; }

/**
 * Used for interrupt callback - resets counter
 */
static int resetCounter()
{
    counter = 0;
    return counter;
}

static void rps_moving_average(float newSpeed)
{
#define N (5)
    static float buf[N]  = { 0 };
    static int   counter = 0;
    buf[counter++ % N]   = newSpeed;
    float rps_avg        = 0;
    for(int i = 0; i < N; i++)
    {
        rps_avg += buf[i];
    }

    rps = rps_avg / N;
}

/**
 * Called by ticker - reads counter and calculates RPS
 */
static void readCounter()
{
    float new_rps
        = (counter / TICKS_PER_REV) / WAIT_TIME_MSEC.count() * 1000.0;
    rps_moving_average(new_rps);
    resetCounter();
}

/**
 * Called by ticker - writes to drive controller
 */
static void writeDrive()
{
    //drive_controller.setProcessValue(rps);
    //ACC_OUT.write(drive_controller.compute());
}

/**
 * P U B L I C   F U N C T I O N S
 */

float drive_getRPS() { return rps; }

float drive_getMPH() { return RPS_TO_MPH(rps); }

float drive_getRPM() { return RPS_TO_RPM(rps); }

float drive_getAcc(void) { return ACC_OUT.read(); }

float drive_getTargetVelocity(void) { return speed_setpoint; }

bool drive_setTargetVelocity(float target_vel)
{
    if(target_vel > MAX_RPS || target_vel < MIN_RPS)
    {
        return false;
    }
    speed_setpoint = target_vel;
    //drive_controller.setSetPoint(target_vel);
    return true;
}

void drive_disableAccel() { ACC_OUT.write(0); }

void drive_overrideAccel(float owv) { ACC_OUT.write(owv); }

int drive_initDrive()
{
    calibrate_pedal();
    ACC_OUT.write(0);
    speed_pin.rise(&incCounter);
    speed_reader.attach(&readCounter, WAIT_TIME_MSEC);
#if CRUISE_CONTROL_ENABLED
//    drive_writer.attach(&writeDrive, PID_RATE_MSEC_CHRONO);
    //drive_controller.setInputLimits(MIN_RPS, MAX_RPS);
    //drive_controller.setOutputLimits(0, 1);
    //drive_controller.setBias(VOLT_TO_PERCENT(1.5));
    //drive_controller.setMode(AUTO_MODE);
#endif // CRUISE_CONTROL_ENABLED
    return 1;
}
