#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    pid_ = pid;
    cpuUtilization_ = CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long activeJiffies = LinuxParser::ActiveJiffies(pid_);      // in jiffies/clock ticks
    long uptimeSys = LinuxParser::UpTime();                        // in sec
    long uptimeProcess = LinuxParser::UpTime(pid_);             // in jiffies/clock ticks

    float secsActive = uptimeSys - (uptimeProcess / sysconf(_SC_CLK_TCK));
    float cpuUtilization = (activeJiffies / sysconf(_SC_CLK_TCK))/secsActive;
    
    return cpuUtilization;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_); 
    }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_)/sysconf(_SC_CLK_TCK);
}

// TODO: Overload the "less than" comparison operator for Process objects
// Compare by CPU utilization
bool Process::operator<(Process const& a) const { 
    return (cpuUtilization_)  <  a.cpuUtilization_;
}