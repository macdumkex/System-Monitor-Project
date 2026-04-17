#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long activeJiffies{0};
    long idleJiffies{0};
    long totalJiffies{0};
};

#endif