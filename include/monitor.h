#ifndef __MONITOR_H__
#define __MONITOR_H__

#include "common.h"
#include "mcc.h"
#include "mccstate.h"
#include "mccstatelist.h"
int monitor_initMonitor(std::chrono::milliseconds freq_ms, Mcc* mcc);

#endif // __MONITOR_H__
