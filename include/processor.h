#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 int userTime;
 int niceTime;
 int systemTime;
 int idleTime;
 int ioWaitTime;
 int irqTime;
 int softIrqTime;
 int stealTime;
 int guestTime;
 int guestNiceTime;

};

#endif