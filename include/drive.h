#ifndef __DRIVE_H__
#define __DRIVE_H__

#define WHEEL_DIAMETER_IN       (22)
#define WHEEL_CIRCUMFE_IN       (M_PI * WHEEL_DIAMETER_IN)
#define RPS_TO_IN_PER_SEC(x)    (WHEEL_CIRCUMFE_IN*x)
#define RPS_TO_MPH(x)           (x * 0.0568182)
#define RPS_TO_RPM(x)           (x/60.0)
#define TICKS_PER_REV           (48.0)
#define WAIT_TIME_MSEC          (100ms) // MUST BE IN UNITS OF ms


float   drive_getRPS(void);
float   drive_getMPH(void);
float   drive_getRPM(void);
int     drive_initDrive(void);


#endif // __DRIVE_H__