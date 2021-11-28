#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

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
  string os, kernel, version;
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
  string mem_total, mem_free, kb, key;
  string line;

  std::ifstream stream(kProcDirectory  + kMeminfoFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> mem_total >> kb;

    std::getline(stream, line);
    linestream.str(line);
    linestream.clear();
    linestream >> key >> mem_free >> kb;
  }

  return (std::stof(mem_total) - std::stof(mem_free)) / std::stof(mem_total);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long int uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

// This helper function parese all jiffies
std::vector<long int> LinuxParser::JiffieHelper(){
  string cpu_name;
  string line;
  vector<long int> jiffie_vector;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu_name;
    
    while (linestream)
    {
      long value = 0;
      linestream >> value;
      jiffie_vector.push_back(value);
    }
  }

  return jiffie_vector;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<long int> jiffie_vector = LinuxParser::JiffieHelper();

  long sum = 0;
  for(jiffie : jiffie_vector){
    sum += jiffie;
  }
  return sum; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string temp_var;
  string utime, stime, cutime, cstime;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      for(int i = 1; i < 14; i++) {
          linestream >> temp_var;
      }
      linestream >> utime >> stime >> cutime >> cstime;
      return stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
    }
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<long int> jiffie_vector = LinuxParser::JiffieHelper();

  // TODO: Is there a better definition of active jiffies besides sum - idle jiffies?
  return LinuxParser::Jiffies() - jiffie_vector[3]; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long int> jiffie_vector = LinuxParser::JiffieHelper();
  return jiffie_vector[3]; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Parses file till it finds key, will return the value after sperator is exists, if not empty string
std::string LinuxParser::KeyValueHelper(const std::string &key, const std::string &file){
  string key_parsed, value;
  string line;

  std::ifstream stream(file);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key_parsed >> value) {
        if (key_parsed == key) {
          return value;
        }
      }
    }
  }
  return "0";
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return stoi(LinuxParser::KeyValueHelper("processes", kProcDirectory + kStatFilename));
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return stoi(LinuxParser::KeyValueHelper("procs_running", kProcDirectory + kStatFilename)); 
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      return line;
    }
  }
  return string();
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  return KeyValueHelper("VmSize:", kProcDirectory + to_string(pid) + kStatusFilename);
  }

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  return KeyValueHelper("Uid:", kProcDirectory + to_string(pid) + kStatusFilename);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid[[maybe_unused]]) {
  string line;
  string user, x, uid;
  string uid_parser{LinuxParser::Uid(pid)};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == uid_parser) {
          return user;
        }
      }
    }
  }
  return "User not found";
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string uptime;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      for(int i = 1; i < 23; i++) {
          linestream >> uptime;
      }
      return stol(uptime);
    }
  }
  return 0;
}

