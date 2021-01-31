#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

/* ----------- SYSTEM INFORMATION  ----------- */

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

// Read and return the system memory utilization
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
// Read and return CPU Information
/* Return long instead of string to avoid double loop */
vector<long> LinuxParser::CpuInformation() {
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

/* ----------- PROCESS INFORMATION  ----------- */

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

// Read and return the use of the CPU
// - If Fail return 0
// -This solution is base in the answer of Vilhelm Gray
// link:https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float LinuxParser::CpuUseByProcess(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" +
                       kStatFilename);
  std::string word = "";
  bool errorFlag = false;
  long total = 0;
  float time = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int index = 1;
    while (linestream >> word) {
      switch (++index) {
        case UTIME_INDEX:
        case STIME_INDEX:
        case CUTIME_INDEX:
        case CSTIME_INDEX:
          try {
            total += std::stol(word);
          } catch (const std::invalid_argument& ex) {
            errorFlag = true;
          }
          break;
        case STARTTIME_INDEX:
          try {
            time = (float)LinuxParser::UpTime() -
                   (float)(std::stol(word) / sysconf(_SC_CLK_TCK));
            if (time == 0) errorFlag = true;
          } catch (const std::invalid_argument& ex) {
            errorFlag = true;
          }
          break;
        default:
          break;
      }
    }
  }
  if (errorFlag) {
    return 0;
  } else {
    return ((float)(total / sysconf(_SC_CLK_TCK)) / time);
  }
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line = "";
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" +
                       kCmdlineFilename);
  if (stream.is_open()) std::getline(stream, line);
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" +
                       kStatusFilename);
  std::string word = "";
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "VmSize:") {
        linestream >> word;
        long mem;
        try {
          mem = std::stol(word);
        } catch (const std::invalid_argument& ex) {
          mem = 0;
        }
        return std::to_string(mem >> 10); /* Shift 10 bits to div 1024 */
      }
    }
  }
  return "0";
}

// Read and return the user ID associated with a process
// - If fail return and empty string
string LinuxParser::Uid(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" +
                       kStatusFilename);
  std::string word = "";
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "Uid:") {
        linestream >> word;
        return word;
      }
    }
  }
  return "";
}

// Read and return the user associated with a process
// - IF fail return and empty string
string LinuxParser::User(int pid) {
  std::string uid = LinuxParser::Uid(pid);
  string line;
  std::string nameRead = "";
  std::string uidRead = "";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> nameRead;
      linestream >> uidRead; /* Remove the "x" */
      linestream >> uidRead;
      if (uid == uidRead) return nameRead;
    }
  }
  return "";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/" +
                       kStatFilename);
  std::string word = "";
  int index = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> word) {
      index++;
      if (index == STARTTIME_INDEX) {
        return ((long)(std::stol(word) / sysconf(_SC_CLK_TCK)));
      }
    }
  }
  return 0;
}
