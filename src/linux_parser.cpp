#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "process.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // Total used memory = memTotal - memFree
  unsigned long memTotal = 0;
  unsigned long memFree = 0;
  unsigned long value = 0;
  string line, key;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = value;
        } else if (key == "MemFree:") {
          memFree = value;
          return (float)(memTotal - memFree) / memTotal;
        }
      }
    }
  }
  return value;
}

// Read and return the system uptime in seconds
long LinuxParser::UpTime() {
  long sysUpTime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    filestream >> sysUpTime;
  }

  return sysUpTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
/*
Source: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
/proc/[PID]/stat
#14 utime - CPU time spent in user code, measured in clock ticks
#15 stime - CPU time spent in kernel code, measured in clock ticks
#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
*/
long LinuxParser::ActiveJiffies(int pid) {
  long totalTime = 0;
  string value;
  string utime, stime, cutime, cstime; // tokens 14, 15, 16, 17
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  for (int i = 1; i <= 13; i++) {
    stream >> value;
  }
  stream >> utime >> stime >> cutime >> cstime;
  totalTime = std::stol(utime) + std::stol(stime) + std::stol(cutime) + std::stol(cstime);
  return totalTime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> cpuStates = CpuUtilization();

  /*
  long activeJiffies = 0;
  for (string jiffy : cpuStates) {
    activeJiffies += std::stoi(jiffy);
  }
  */
  // if you don't include idle and iowait
  long activeJiffies = std::stoi(cpuStates[LinuxParser::CPUStates::kUser_])
                     + std::stoi(cpuStates[LinuxParser::CPUStates::kNice_])
                     + std::stoi(cpuStates[LinuxParser::CPUStates::kSystem_])
                     + std::stoi(cpuStates[LinuxParser::CPUStates::kIRQ_])
                     + std::stoi(cpuStates[LinuxParser::CPUStates::kSoftIRQ_])
                     + std::stoi(cpuStates[LinuxParser::CPUStates::kSteal_]);
                     /* guest is already included in user and nice?
                     + std::stoi(cpuStates[LinuxParser::CPUStates::kGuest_])
                     + std::stoi(cpuStates[LinuxParser::CPUStates::kGuestNice_]); */

  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpuStates = CpuUtilization();
  long idleJiffies = std::stoi(cpuStates[LinuxParser::CPUStates::kIdle_]) 
                   + std::stoi(cpuStates[LinuxParser::CPUStates::kIOwait_]);
  
  return idleJiffies;
}

// Read and return CPU utilization
// extract all ten values associated with the key cpu
// return vector of string, call in Processor
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuStates;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        for (int i = 0; i < 10; i++) {
          linestream >> value;
          cpuStates.push_back(value);
        }
        return cpuStates;
      }
    }
  }
  return cpuStates;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    stream >> command;
  }
  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
        if (key == "VmSize:") {
          linestream >> value;
          long ramMB = std::stol(value) / 1000;
          return to_string(ramMB);
      }
    }
  }
  return "";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, user, uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      // user:x:uid:
      std::getline(linestream, user, ':');
      std::getline(linestream, uid, ':');
      std::getline(linestream, uid, ':');
      if (uid == LinuxParser::Uid(pid)) {
        return user;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process in seconds
long LinuxParser::UpTime(int pid) {
  long procStartTime = 0;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    // Token 22 starttime - Time when the process started, in clock ticks
    for (int i = 1; i <= 22; i++) {
      stream >> value;
    }
    procStartTime = std::stol(value) / sysconf(_SC_CLK_TCK);
  }

  return UpTime() - procStartTime;
}
