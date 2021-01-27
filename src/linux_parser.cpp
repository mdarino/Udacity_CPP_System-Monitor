#include "linux_parser.h"

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
/*
24/JAN/2021 - MD - The function kernel return the word "version" instead of
version number. Added and extra string to recive the word "version" and copy to
the kernel variable the real version info.
*/
string LinuxParser::Kernel() {
  string os, verString, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> verString >> kernel;
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
        int pid;
        try {
          pid = stoi(filename);
          pids.push_back(pid);
        } catch (const std::invalid_argument& ex) {
          /* try continue with the next... */
        }
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float total = 1; /* 1 to avoid div 0 if it doesn't read the file */
  float free = 1;

  if (stream.is_open()) {
    std::string word;
    std::string value;
    std::getline(stream, line);
    std::istringstream linestream1(line);
    linestream1 >> word >> value;
    try {
      total = std::stof(value);
      if (total == 0) total = 1; /* Safe value */
    } catch (const std::invalid_argument& ex) {
      total = 1; /* Safe value */
    }
    total = std::stof(value);
    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> word >> value;
    try {
      free = std::stof(value);
    } catch (const std::invalid_argument& ex) {
      free = 1; /* Safe value */
    }
  }
  return ((total - free) / total);
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::string word;
  long upTime = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> word;
    try {
      upTime = std::stol(word);
    } catch (const std::invalid_argument& ex) {
      upTime = 0;
    }
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
/* Return long instead of string to avoid double loop */
vector<long> LinuxParser::CpuUtilization() {
  vector<long> myCpuValues = {};
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string word;
    linestream >> word; /* Discart the first word "cpu" */
    while (linestream >> word) {
      try {
        myCpuValues.push_back(std::stol(word));
      } catch (const std::invalid_argument& ex) {
        /* Next... and the error will be catch when the function is called,
         * check the size */
      }
    }
  }
  return myCpuValues;
}

// Read and return the total number of processes
/* If find -> return the number of process
   If not (Cannot find, not open the file, etc) return 0 */
int LinuxParser::TotalProcesses() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string word;
  int process = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "processes") {
        linestream >> word;
        try {
          process = std::stoi(word);
        } catch (const std::invalid_argument& ex) {
          process = 0;
        }
      }
    }
  }
  return process;
}

// Read and return the number of running processes
/* If find -> return the number of running process
   If not (Cannot find, not open the file, etc) return 0 */
int LinuxParser::RunningProcesses() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string word;
  int running_process = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "procs_running") {
        linestream >> word;
        try {
          running_process = std::stoi(word);
        } catch (const std::invalid_argument& ex) {
          running_process = 0;
        }
      }
    }
  }
  return running_process;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
