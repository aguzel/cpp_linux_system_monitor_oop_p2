#include "format.h"
#include <string>
#include <iomanip>
using std::string;

// DONE: Complete this helper function
string Format::ElapsedTime(long seconds) {
  long minutes;
  long hours;
  std::string min;
  std::string sec;
  std::string hr; 
  
  
  std::string upTime;

  hours = seconds / 3600;
  minutes = (seconds % 3600) / 60;
  seconds = seconds % 60;

  if (hours < 10) { hr = "0" + std::to_string(hours);} else { hr = std::to_string(hours);}
  if (minutes < 10) { min = "0" + std::to_string(minutes);} else { min = std::to_string(minutes);}
  if (seconds < 10) { sec = "0" + std::to_string(seconds);} else { sec = std::to_string(seconds);}


  upTime = hr + ":" + min + ":" + sec;

  return upTime;
}
