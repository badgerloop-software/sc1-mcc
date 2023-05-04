#ifndef __COMMON_H__
#define __COMMON_H__

// FEATURE FLAGS
#define CRUISE_CONTROL_PEDAL    (0)

#define SEC_TO_USEC(x)    (x*1e6)
#define MSEC_TO_USEC(x)   (x*1000)
#define USEC_TO_MSEC(x)   (x*0.001)
#define USEC_TO_SEC(x)    (x*1e-6)
#define VOLT_TO_PERCENT(x)  ((x/1.5) / 3.3)

#endif // __COMMON_H__