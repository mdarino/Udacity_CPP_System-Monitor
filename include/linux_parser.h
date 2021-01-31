#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

/*Index of values of the /proc/[PID]/stat */
/* CPU time spent in kernel code, measured in clock ticks */
#define UTIME_INDEX 14
/* CPU time spent in kernel code, measured in clock ticks */
#define STIME_INDEX 15
/* Waited-for children's CPU time spent in user code (in clock ticks) */
#define CUTIME_INDEX 16
/* Waited-for children's CPU time spent in kernel code (in clock ticks) */
#define CSTIME_INDEX 17
/* Time when the process started, measured in clock ticks */
#define STARTTIME_INDEX 22

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU

#define CPU_STATE_NUM 10 /* Max number of CPU states */
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
/* MD: Rename CpuInformation to Cpuinformation because return only the info...*/
std::vector<long> CpuInformation();
/* MD: Rename ActiveJiffies to CpuUseByProcess */
float CpuUseByProcess(int pid);

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif