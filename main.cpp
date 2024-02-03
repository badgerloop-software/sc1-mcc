#include "mbed.h"
#include "speed.h"


#define PWM_PIN D2
#define STEP_SIZE 10


PwmOut pwmPin(PWM_PIN);

int main()
{
    //BufferedSerial serial(USBTX, USBRX, 115200);
    BufferedSerial serial(USBTX, USBRX);
    serial.set_blocking(true);

    // Fixed 10% duty cycle, the speed depends on frequency
    pwmPin.write(0.1);
    pwmPin.suspend();

    int simRPM = 0;
    char buf[1];

    while(true)
    {
        startSpeedCalculation(20ms);
        printf("\e[1;1H\e[2J");
        printf("Simulating RPM: %i    ", simRPM);
        printf("Press i to increment, d to decrement");
        printf("\nActual RPM Calculation: %f", rpm);

        if (serial.read(buf, 1) > 0) {
            if (buf[0] == 'i') {
                simRPM += STEP_SIZE;
            } else if (buf[0] == 'd') {
                simRPM -= STEP_SIZE;
            }

            if (simRPM < 0) simRPM = 0;
        }

        // Divide by 60 to get rps, multiply by 48 to get ticks per second
        // Period in ms is 1000 / ticks per second
        // Roughly equivalent to 1333.33333 / rpm

        float periodMS;
        if (simRPM > 0) {
            pwmPin.resume();
            periodMS = 1333.333333 / simRPM;
        } else {
            pwmPin.suspend();
        }

        pwmPin.period_ms(periodMS);
    }
}
