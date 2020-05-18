#include "processor.h"

#include "linux_parser.h"

#include <vector>
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpuStates = LinuxParser::CpuUtilization();
    if (cpuStates.size() == 10) {
        userTime = std::stoi(cpuStates[LinuxParser::CPUStates::kUser_]);
        niceTime = std::stoi(cpuStates[LinuxParser::CPUStates::kNice_]);
        systemTime = std::stoi(cpuStates[LinuxParser::CPUStates::kSystem_]);
        idleTime = std::stoi(cpuStates[LinuxParser::CPUStates::kIdle_]);
        ioWaitTime = std::stoi(cpuStates[LinuxParser::CPUStates::kIOwait_]);
        irqTime = std::stoi(cpuStates[LinuxParser::CPUStates::kIRQ_]);
        softIrqTime = std::stoi(cpuStates[LinuxParser::CPUStates::kSoftIRQ_]);
        stealTime = std::stoi(cpuStates[LinuxParser::CPUStates::kSteal_]);
        guestTime = std::stoi(cpuStates[LinuxParser::CPUStates::kGuest_]);
        guestNiceTime = std::stoi(cpuStates[LinuxParser::CPUStates::kGuestNice_]);
    }

    int idle = idleTime + ioWaitTime;
    int nonIdle = userTime + niceTime + systemTime + irqTime + softIrqTime + stealTime;
    return (float) nonIdle / (nonIdle + idle);
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