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

Processor& System::Cpu() {
    cpu_.Utilization();
    return cpu_;
}

vector<Process>& System::Processes() { 
    vector<int> currentPids = LinuxParser::Pids();
    for (int pid : currentPids) {
        if (std::find_if(processes_.begin(), processes_.end(), [pid](const Process& p) { return p.Pid() == pid; }) == processes_.end()) {
            Process process(pid);
            if (process.Ram() != "Zombie") {
                processes_.push_back(process);
            }
    }
}
    
    processes_.erase(std::remove_if(processes_.begin(), processes_.end(), [&currentPids](const Process& p) {
        return std::find(currentPids.begin(), currentPids.end(), p.Pid()) == currentPids.end();
    }), processes_.end());
    std::sort(processes_.rbegin(), processes_.rend());
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

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() {
    return LinuxParser::UpTime();
}