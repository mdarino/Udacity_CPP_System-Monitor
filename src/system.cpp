#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  vector<int> myreadPids = LinuxParser::Pids();
  for (int onePid : myreadPids) {
    Process aux_process(
        onePid); /* We have a empty process object (we only have the pid_) */
    /* Update values (The idea is to get all the values to have the option to
     * sort for different things)*/

    aux_process.User(LinuxParser::User(onePid));
    aux_process.Command(LinuxParser::Command(onePid));
    aux_process.CpuUtilization(LinuxParser::CpuUseByProcess(onePid));
    aux_process.Ram(LinuxParser::Ram(onePid));
    aux_process.UpTime(LinuxParser::UpTime(onePid));

    /* Push the processes object to the internal list */
    processes_.emplace_back(aux_process);
  }

  if(sortCPU)
  {
    std::sort(processes_.begin(), processes_.end(), 
      [](Process const& a, Process const& b) {return a > b;});
  }
  else
  { 
    std::sort(processes_.begin(), processes_.end(), 
      [](Process const& a, Process const& b) {return a < b;});
  }
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }