#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    activeJiffies = LinuxParser::ActiveJiffies();
    idleJiffies = LinuxParser::IdleJiffies();
    totalJiffies = activeJiffies + idleJiffies;
    
    if (totalJiffies == 0) {
        return 0; // Avoid division by zero
    }
    
    return static_cast<float>(activeJiffies) / totalJiffies;
}