#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {}
  /* Gets */
  int Pid() { return pid_; };
  std::string User() { return user_; };
  std::string Command() { return command_; };
  float CpuUtilization() { return cpu_; };
  std::string Ram() { return ram_; };
  long int UpTime() { return upTime_; };
  /* Sets */
  void Pid(int pid);
  void User(std::string user);
  void Command(std::string command);
  void CpuUtilization(float cpu);
  void Ram(std::string ram);
  void UpTime(long int upTime);

 bool operator<(Process const& a)const;
 bool operator>(Process const& a)const;

 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpu_;
  std::string ram_;
  long int upTime_;
};

#endif