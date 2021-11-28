#include "processor.h"
#include <iostream>
#include <ctime>
#include "linux_parser.h"
#include <unistd.h>

using std::string;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  string line;
  string cpu;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;

  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }

  long idle_time = idle + iowait;        
  float time_total = user + nice + system + irq + softirq + steal + idle + iowait;
  
  return (time_total - idle_time)/time_total;
}