#include "linux_parser.h"
#include "format.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float usage;
  string line;
  string key;
  string value;
  std::vector<string> v{};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    for (auto i : {0, 1, 2}) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      v.push_back(value);
    }
  }
  usage = (stof(v[0]) - stof(v[2])) / stof(v[0]);
  return usage;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long seconds;
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  seconds = std::stol(uptime);
  return seconds;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  vector<string> v{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      v.push_back(key);
    }
  }
  return v;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int tot_proc;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          tot_proc = std::stoi(value);
          break;
        }
      }
    }
  }
  return tot_proc;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int proc_value;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          proc_value = std::stoi(value);
          break;
        }
      }
    }
  }
  return proc_value;
}

// DONE: Read and return the command associated with a process

string LinuxParser::Command(int pid) {
  string line;
  string key;
  string cmd_line;
  std::vector<string> v{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      cmd_line += key;
    }
  }
  for (auto i : v) cmd_line += i;

  return cmd_line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  int intRam;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          intRam = std::stoi(value)/1000;
          value = to_string(intRam);
          return value;
          
        }
      }
    }
  }
  return "0.0";
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return "UID not found";
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string uid_number = LinuxParser::Uid(pid);
  string line;
  string key;
  string username_line;
  string username;
  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key.find(":" + uid_number + ":") != std::string::npos) {
          username_line = key;
          break;
        }
      }
    }
  }
  for (auto i : username_line) {
    username += i;
    if (i == ':') {
      break;
    }
  }
  username.pop_back();
  return username;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {

  string line;
  string key;
  string uptime_string;
  long uptime_long;
  std::vector<string> v{};

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      v.push_back(key);
    }
  }
  uptime_string = v[21];
  v.clear();
  uptime_long = std::stol(uptime_string);
  uptime_long /= sysconf(_SC_CLK_TCK);
  return uptime_long;
}

// DONE: Read and return the uptime of a process
float LinuxParser::CpuUtilization(int pid) {

  string line;
  string key;
  float utime;
  float stime;
  float cutime;
  float cstime;
  float starttime;
  float uptime;
  float totaltime;
  float seconds;
  float cpu_usage;
  std::vector<string> v{};

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      v.push_back(key);
    }
  }
  utime = std::stof(v[13]);
  stime = std::stof(v[14]);
  cutime = std::stof(v[15]);
  cstime = std::stof(v[16]);
  starttime = std::stof(v[21]);
  v.clear();

  std::ifstream filestream_(kProcDirectory + kUptimeFilename);
  if (filestream_.is_open()) {
    std::getline(filestream_, line);
    std::istringstream linestream(line);
    linestream >> key;
    uptime = std::stof(key);
  }  
  

  totaltime = utime + stime + cutime + cstime;
  seconds = uptime - (starttime/sysconf(_SC_CLK_TCK));
  cpu_usage = (totaltime/sysconf(_SC_CLK_TCK))/seconds;
  
  return cpu_usage;

}



// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }