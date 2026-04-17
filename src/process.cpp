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

int Process::Pid() const{ 
     return pid; 
}

float Process::CpuUtilization() const{ 
    float activeSeconds = LinuxParser::ActiveJiffies(pid)/sysconf(_SC_CLK_TCK);
    long seconds = LinuxParser::UpTime()-UpTime();
    if (seconds > 0) {
        return activeSeconds / seconds;
    }
    return 0;
}

string Process::Command() const {
    return LinuxParser::Command(pid);
}

string Process::Ram() const { 
    return LinuxParser::Ram(pid); 
}

string Process::User() const {
    return LinuxParser::User(pid);
}

long int Process::UpTime() const {
    return LinuxParser::UpTime(pid);
}

bool Process::operator<(Process const& a) const { 
    if (this->CpuUtilization() < a.CpuUtilization()) {
        return true;
    }
    return false;
}