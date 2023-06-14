#include "monitor.h"

#include <chrono>

#include "analog.h"
#include "common.h"
#include "drive.h"
#include "mbed.h"
#include "mc_status.h"

static Ticker monitor_timer;

void monitor_private_printMonitor(void)
{
    printf(CLEAR_SCREEN_STR);
    printf("Motor Controller is : %s\n\r",
           READ_BIT_POS(curGPIO, POWER_BIT) ? "ON" : "OFF");
#if CRUISE_CONTROL_ENABLED
    printf("Targeted RPS        : %.2f RPS\n\r", drive_getTargetVelocity());
#endif // CRUISE_CONTROL_ENABLED
    printf("Current Speed RPS   : %.2f RPS\n\r", drive_getRPS());
    printf("Current Speed MPH   : %.2f MPH\n\r", RPS_TO_MPH(drive_getRPS()));
    printf("Eco Telem           : %s\n\r",
           READ_BIT_POS(curGPIO, ECO_BIT) ? "Power" : "ECO");
    printf("Dir Telem           : %s\n\r",
           !READ_BIT_POS(curGPIO, DIRECTON_BIT) ? "FWD" : "REV");
    printf("MC Status           : %d\n\r", READ_BIT_POS(curGPIO, MC_STAT_BIT));
    printf("Pedal Voltage       : %.2fv\n\r", analog_getCurPedal());
    printf("Pedal Percentage    : %.2f\n\r",
           calculate_pedal_press(analog_getCurPedal()));
    printf("MC Status           : %s",
           mc_status_getStatusString(mc_status_getStatus()));
}

int monitor_initMonitor(std::chrono::milliseconds freq_ms)
{
    monitor_timer.attach(&monitor_private_printMonitor, freq_ms);
    return 0;
}