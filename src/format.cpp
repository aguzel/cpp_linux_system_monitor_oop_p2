#include "format.h"
#include <string>
using std::string;

// DONE: Complete this helper function
string Format::ElapsedTime(long seconds) {
  long minutes;
  long hours;
  std::string upTime;

  hours = seconds / 3600;
  minutes = (seconds % 3600) / 60;
  seconds = seconds % 60;

  upTime = std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
           std::to_string(seconds);

  return upTime;
}
