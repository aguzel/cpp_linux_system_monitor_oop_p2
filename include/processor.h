#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
  std::vector<std::string> v{}; 
  float prevUser{0};
  float prevNice{0};
  float prevSystem{0};
  float prevIdle{0};
  float prevIowait{0};
  float prevIrq{0};
  float prevSoft_irq{0};
  float prevSteal{0};

  
  float user{0};
  float nice{0};
  float system{0};
  float idle{0};
  float iowait{0};
  float irq{0};
  float soft_irq{0};
  float steal{0};
  

  float CPU_percentage;
};

#endif