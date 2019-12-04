#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process() = default;
  Process(int pid) : pid_(pid) {}
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  float GetCpuUtilization() const
  {
      return cpu_utilization_;
  }

  // TODO: Declare any necessary private members
 private:
  int pid_{0};
  float cpu_utilization_{0.0};
  long int up_time_{1};
};

#endif