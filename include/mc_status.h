#ifndef __MC_STATUS_H__
#define __MC_STATUS_H__

#include "common.h"

typedef enum
{
    MC_STATUS_OK                = 0,
    MC_STATUS_OVERCURRENT       = 1,
    MC_STATUS_NOT_SET           = 2,
    MC_STATUS_HOLE_SENSOR_ERROR = 3,
    MC_STATUS_MOTOR_LOCK_ERROR  = 4,
    MC_STATUS_SENSOR_1_ERROR    = 5,
    MC_STATUS_SENSOR_2_ERROR    = 6,
    MC_STATUS_NOT_SET_2         = 7,
    MC_STATUS_OVERVOLTAGE       = 8,
    MC_STATUS_OVERHEATING       = 9,
    MC_STATUS_MAX_ERROR_CODE    = 10 // There is no error code greater than 9
} mc_status_e;

bool        mc_status_init();
mc_status_e mc_status_getStatus();
char *      mc_status_getStatusString(mc_status_e status);

#endif // __MC_STATUS_H__