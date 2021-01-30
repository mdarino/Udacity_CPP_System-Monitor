#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
void Process::Pid(int pid) { Process::pid_ = pid;}
// Return the user (name) that generated this process
void Process::User(std::string user) { Process::user_ = user;}
// Return the command that generated this process
void Process::Command(std::string command) { Process::command_ = command;}
// Return this process's CPU utilization
void Process::CpuUtilization(float cpu) { Process::cpu_ = cpu;}
// Return this process's memory utilization
void Process::Ram(std::string ram) { Process::ram_ = ram;}
// Return the age of this process (in seconds)
void Process::UpTime(long int upTime) { Process::upTime_ = upTime;}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}