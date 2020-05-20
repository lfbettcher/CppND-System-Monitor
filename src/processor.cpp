#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using std::stoi;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // get current values
  std::vector<std::string> cpuStates = LinuxParser::CpuUtilization();
  userTime_ = stoi(cpuStates[LinuxParser::CPUStates::kUser_]);
  niceTime_ = stoi(cpuStates[LinuxParser::CPUStates::kNice_]);
  systemTime_ = stoi(cpuStates[LinuxParser::CPUStates::kSystem_]);
  idleTime_ = stoi(cpuStates[LinuxParser::CPUStates::kIdle_]);
  ioWaitTime_ = stoi(cpuStates[LinuxParser::CPUStates::kIOwait_]);
  irqTime_ = stoi(cpuStates[LinuxParser::CPUStates::kIRQ_]);
  softIrqTime_ = stoi(cpuStates[LinuxParser::CPUStates::kSoftIRQ_]);
  stealTime_ = stoi(cpuStates[LinuxParser::CPUStates::kSteal_]);
  guestTime_ = stoi(cpuStates[LinuxParser::CPUStates::kGuest_]);
  guestNiceTime_ = stoi(cpuStates[LinuxParser::CPUStates::kGuestNice_]);

  // userTime and niceTime already include guestTime and guestNiceTime
  long idle = idleTime_ + ioWaitTime_;
  long nonIdle = userTime_ + niceTime_ + systemTime_ + irqTime_ + softIrqTime_ +
                 stealTime_;
  long total = nonIdle + idle;

  // calculate difference
  long totald = total - prevTotal_;
  long nonIdled = nonIdle - prevNonIdle_;

  float utilization = (float)nonIdled / totald;

  // set previous values
  this->prevNonIdle_ = nonIdle;
  this->prevTotal_ = total;

  return utilization;
}
