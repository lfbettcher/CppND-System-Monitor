#ifndef PROCESS_H
#define PROCESS_H

#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) { CpuUtilization(); }
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_ = 0;
  float cpuUtilization_ = 0.0;
};

#endif