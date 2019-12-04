#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
  std::string line{};
  std::ifstream file_name(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);

  up_time_ = LinuxParser::UpTime();

  if(file_name.is_open())
  {
    std::getline(file_name, line);
    std::istringstream current_line(line);
    std::string element{};
    std::vector<std::string> all_elemnts{};

    while(!current_line.fail())
    {
        current_line >> element;

        if(current_line.fail())
        {
            break;
        }

        all_elemnts.push_back(element);
    }

    long total_time = std::stol(all_elemnts[13]) + std::stol(all_elemnts[14]) + std::stol(all_elemnts[15]) + std::stol(all_elemnts[16]);
    long start_time = std::stol(all_elemnts[21]);
    auto Hertz = sysconf(_SC_CLK_TCK);
    float seconds = LinuxParser::UpTime() - (start_time / Hertz);

    cpu_utilization_ =  100 * ((total_time / Hertz) / seconds);

    return cpu_utilization_;
  } 

  return 0; 
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
    return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    return this->GetCpuUtilization() > a.GetCpuUtilization(); 
}