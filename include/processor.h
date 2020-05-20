#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  int userTime_;
  int niceTime_;
  int systemTime_;
  int idleTime_;
  int ioWaitTime_;
  int irqTime_;
  int softIrqTime_;
  int stealTime_;
  int guestTime_;
  int guestNiceTime_;

  long prevNonIdle_ = 0;
  long prevTotal_ = 0;
};

#endif