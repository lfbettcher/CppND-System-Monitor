#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    /*
    float idle = idleTime + ioWaitTime;
    float nonIdle = userTime + niceTime + systemTime + irqTime + softIrqTime + stealTime;
    float cpuUtilization = nonIdle / (nonIdle + idle);
    return cpuUtilization;
    */
   return 0.0;
}

/* TODO: use this to calculate more updated usage
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

PrevIdle = prev_idle + prev_iowait
Idle = idle + iowait

PrevNonIdle = prev_user + prev_nice + prev_system + prev_irq + prev_softIrq + prev_steal
NonIdle = user + nice + system + irq + softIrq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Utilization = (totald - idled) / totald;
*/