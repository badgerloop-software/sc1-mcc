#include "mc_status.h"

#include "common.h"
#include "mbed.h"

#define MC_STATUS_TIMEOUT_MS (5000ms) // This will need some fine tuning

static InterruptIn mc_status(MC_STATUS_PIN, PullNone);
static Ticker      mc_status_ticker;
static int         flash_counter  = 0;
static mc_status_e current_status = MC_STATUS_OK;

void mc_status_private_incrementCounter(void) { flash_counter++; }

void mc_status_private_resetCounter(void) { flash_counter = 0; }

void mc_status_private_computeStatus(void)
{
    if(flash_counter >= MC_STATUS_MAX_ERROR_CODE)
    {
        current_status = MC_STATUS_MAX_ERROR_CODE;
    }
    else
    {
        current_status = (mc_status_e)flash_counter;
    }
    mc_status_private_resetCounter();
}

bool mc_status_init()
{
    mc_status.rise(&mc_status_private_incrementCounter);
    mc_status_ticker.attach(&mc_status_private_computeStatus,
                            MC_STATUS_TIMEOUT_MS);
    return true;
}

mc_status_e mc_status_getStatus() { return current_status; }

char *mc_status_getStatusString(mc_status_e status)
{
    switch(status)
    {
    case MC_STATUS_OK:
        return "OK";
    case MC_STATUS_OVERCURRENT:
        return "Overcurrent";
    case MC_STATUS_NOT_SET:
        return "Not set";
    case MC_STATUS_HOLE_SENSOR_ERROR:
        return "Hole sensor error";
    case MC_STATUS_MOTOR_LOCK_ERROR:
        return "Motor lock error";
    case MC_STATUS_SENSOR_1_ERROR:
        return "Sensor 1 error";
    case MC_STATUS_SENSOR_2_ERROR:
        return "Sensor 2 error";
    case MC_STATUS_NOT_SET_2:
        return "Not set 2";
    case MC_STATUS_OVERVOLTAGE:
        return "Overvoltage";
    case MC_STATUS_OVERHEATING:
        return "Overheating";
    case MC_STATUS_MAX_ERROR_CODE:
        return "Max error code";
    default:
        return "Unknown error";
    }
}