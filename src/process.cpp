#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process :: Process (int pid) {
    Process::pid_= pid;
    Process::user_ = LinuxParser::User(pid);
    Process::command_ = LinuxParser::Command(pid);
    Process::cpuUtil_ = LinuxParser::CpuUtilization(pid);
    Process::ram_ =  LinuxParser::Ram(pid);
    Process::uptime_ = LinuxParser::UpTime(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return Process::pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtil_; }

// TODO: Return the command that generated this process
string Process::Command() { return Process::command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_ ;}

// TODO: Return the user (name) that generated this process
string Process::User() { return Process::user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return Process::uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    if ((this->cpuUtil_) > (a.cpuUtil_)) {return true;}
    return false;
}