#include "processor.h"
#include "linux_parser.h"


// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {

  Processor::v = LinuxParser::CpuUtilization(); 
  prevUser = user;
  prevNice = nice; 
  prevSystem = system;
  prevIdle = idle; 
  prevIowait = iowait;
  prevIrq = irq;
  prevSoft_irq = soft_irq;
  prevSteal = steal;

  user = std::stof(Processor::v[1]); 
  nice = std::stof(Processor::v[2]);
  system = std::stof(Processor::v[3]);
  idle = std::stof(Processor::v[4]);
  iowait = std::stof(Processor::v[5]);
  irq = std::stof(Processor::v[6]);
  soft_irq = std::stof(Processor::v[7]);
  steal = std::stof(Processor::v[8]);

  //CPU_Percentage = (totald - idled)/totald
  Processor::CPU_percentage = ((user + nice + system + irq + soft_irq + steal ) -
                               (prevUser + prevNice + prevSystem + prevIrq + prevSoft_irq + prevSteal )) /
                              ((user + nice + system + irq + soft_irq + steal + idle + iowait) -
                               (prevUser + prevNice + prevSystem + prevIrq + prevSoft_irq + prevSteal + prevIdle + prevIowait )); 



    return CPU_percentage;
}