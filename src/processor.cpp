#include "processor.h"

#include <vector>

#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  float cpuPorcentage = 0.0;
  /*
    Based on Vangelis's answer:
    https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   */

  cpuValues = LinuxParser::CpuUtilization();
  if (cpuValues.size() ==
      CPU_STATE_NUM) /* For some reason We miss one value, discart this update
                        and print 0.0 (Another option is print the previous)*/
  {
    long idle =
        cpuValues[LinuxParser::kIdle_] + cpuValues[LinuxParser::kIOwait_];

    long total =
        cpuValues[LinuxParser::kUser_] + cpuValues[LinuxParser::kNice_] +
        cpuValues[LinuxParser::kSystem_] + cpuValues[LinuxParser::kIRQ_] +
        cpuValues[LinuxParser::kSoftIRQ_] + cpuValues[LinuxParser::kSteal_] +
        idle;

    if (firstRun == true) {
      firstRun =
          false; /* Avoid the first calculation to fill the previos value */
    } else {
      long totald = total - prevTotal;
      long idled = idle - prevIdle;
      cpuPorcentage = (float)(totald - idled) / totald;
    }
    prevTotal = total;
    prevIdle = idle;
  }
  return cpuPorcentage;
}
