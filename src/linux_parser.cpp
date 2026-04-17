#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
    return value;
  }
  return "Operating system not found";
  
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    return kernel;
  }
  return "Kernel not found";
}

// BONUS: Update this to use std::filesystem
/*vector<int> LinuxParser::Pids() {
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
}*/

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  std::filesystem::path proc_path(kProcDirectory);
  for (const auto& entry : std::filesystem::directory_iterator(proc_path)) {
    if (entry.is_directory()) {
      string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}


float LinuxParser::MemoryUtilization() { 
  float memTotal, memFree;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = stof(value);
        } else if (key == "MemFree:") {
          memFree = stof(value);
        }
      }
    }
    return (memTotal - memFree) / memTotal;
  }
  return 0.0;
  
}

long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    long uptime_seconds = stol(uptime);
    return uptime_seconds;
  }
  return 0;
  

}

long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  int count = 0;
  float utime{0}, stime{0}, cutime{0}, cstime{0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::stringstream ss(line);
    while(ss >> value){
      if(count == 13){
        utime = stof(value);
      } else if(count == 14){
        stime = stof(value);
      } else if(count == 15){
        cutime = stof(value);
      } else if(count == 16){
        cstime = stof(value);
        break;
      } 

      count++;
    }
      
  }
  return utime + stime + cutime + cstime;
}

long LinuxParser::ActiveJiffies() { 
  string line, key;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        return user + nice + system + irq + softirq + steal;
      }
    }
  }
  return 0;
}

long LinuxParser::IdleJiffies() { 
  string line, key;
  long idle, iowait;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        long user, nice, system, irq, softirq, steal;
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        return idle + iowait;
      }
    }
  }
  return 0;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuUtilization;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      key = key.substr(0, 3); 
      if (key == "cpu") {
        while (linestream >> value) {
          cpuUtilization.push_back(value);
        }
        return cpuUtilization;
      }
    }
  }
  return {};
}

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return "Command not found";
}

string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmData:") {
          long ram_kB = stol(value);
          long ram_MB = ram_kB / 1024;
          return std::to_string(ram_MB);
        }
      }
    }
  }
  return "Zombie";
}

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

string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  string username;
  string x;
  string userid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> userid;
      if (userid == uid) {
        return username;
      }
    }
  }
  return "User not found";
}

long LinuxParser::UpTime(int pid) {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
   if (stream.is_open()) {
     for (int i = 0; i < 22; i++) {
       stream >> uptime;
     }
     long uptime_seconds = stol(uptime) / sysconf(_SC_CLK_TCK);
     return uptime_seconds;
   }
   return 0;
}

