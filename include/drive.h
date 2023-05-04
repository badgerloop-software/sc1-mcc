#ifndef __DRIVE_H__
#define __DRIVE_H__


#define WHEEL_DIAMETER_IN       (22)
#define WHEEL_CIRCUMFE_IN       (3.14159 * WHEEL_DIAMETER_IN)
#define RPS_TO_IN_PER_SEC(x)    (WHEEL_CIRCUMFE_IN*x)
#define RPS_TO_MPH(x)           (RPS_TO_IN_PER_SEC(x) * 0.0568182)
#define RPS_TO_RPM(x)           (x/60.0)
#define TICKS_PER_REV           (48.0)
#define WAIT_TIME_MSEC          (10ms) // MUST BE IN UNITS OF ms

#define MIN_RPS                 (0)
#define MAX_RPS                 (10)

#define KP                      (8)
#define KI                      (0.00001)
#define KD                      (0.00000001)
#define PID_RATE_SEC            (0.01) // [s] I think?
#define PID_RATE_MSEC_CHRONO    (10ms)


float   drive_getRPS(void);
float   drive_getMPH(void);
float   drive_getRPM(void);
int     drive_initDrive(void);
bool    drive_setTargetVelocity(float target_vel);
void    drive_disableAccel(void);
void    drive_overrideAccel(float owv);
float   drive_getAcc(void);




#endif // __DRIVE_H__