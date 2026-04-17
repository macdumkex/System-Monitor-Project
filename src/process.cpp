#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <linux_parser.h>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    this->pid = pid;
}
// TODO: Return this process's ID
int Process::Pid() const{ 
     return pid; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{ 
    long activeJiffies = LinuxParser::ActiveJiffies(pid);
    std::cout<<"Uptime: "<<UpTime()<<std::endl;
    std::cout<<"StartTime: "<<StartTime()<<std::endl;
    long seconds = UpTime()-StartTime();
    if (seconds > 0) {
        return static_cast<float>(activeJiffies) / seconds;
    }
    return 0;
}

// TODO: Return the command that generated this process
string Process::Command() const { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() const {
    return LinuxParser::User(pid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const {
    return LinuxParser::UpTime(pid);
}

long int Process::StartTime() const {
    return LinuxParser::StartTime(pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    if (this->CpuUtilization() < a.CpuUtilization()) {
        return true;
    }
    return false;
}