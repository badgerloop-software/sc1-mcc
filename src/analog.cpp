#include "analog.h"

AnalogIn ACC_SIG(A0);
AnalogIn BRK_SIG(A1);
AnalogOut ACC_OUT(A4);



Ticker ACC_TIMER;

void analogUpdate() {
    float acc_in = ACC_SIG.read_voltage();
    float brk_in = BRK_SIG.read_voltage();
    *(outputQueue[2].data) = acc_in;
    ACC_OUT.write(acc_in);
    queueFlags.set(1UL << 2);

    *(outputQueue[3].data) = brk_in;
    queueFlags.set(1UL << 3);
}

int initAnalog(std::chrono::milliseconds pollRateMS) {
    ACC_SIG.set_reference_voltage(5);
    BRK_SIG.set_reference_voltage(5);
    ACC_TIMER.attach(analogUpdate, pollRateMS);
    return 0;
}

int disableAnalog() {
    ACC_TIMER.detach();
    return 0;    
}