#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
    cpu_.Utilization();
    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> currentPids = LinuxParser::Pids();
    for (int pid : currentPids) {
        if (std::find_if(processes_.begin(), processes_.end(), [pid](const Process& p) { return p.Pid() == pid; }) == processes_.end()) {
            Process process(pid);
            processes_.emplace_back(process);
        }
    }
    
    //find if processes_ vector has a process that is not in the current list of PIDs and remove it from processes_ vector
    processes_.erase(std::remove_if(processes_.begin(), processes_.end(), [](const Process& p) {
        return std::find(currentPids.begin(), currentPids.end(), p.Pid()) == currentPids.end();
    }), processes_.end());
    return processes_;
}

std::string System::Kernel() { 
    return LinuxParser::Kernel();
}

float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
 }

std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() {
    return LinuxParser::UpTime();
 }