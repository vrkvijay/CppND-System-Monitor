#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "linux_parser.h"

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
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> os >> kernel;
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
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{  
  std::string line{};
  std::ifstream file_name(kProcDirectory + kMeminfoFilename);

  std::getline(file_name, line);
  std::istringstream current_line(line);
  std::string mem_total;
  long mem_total_value;

  current_line >> mem_total >> mem_total_value;

  std::string mem_free{};
  long mem_free_value{};

  std::getline(file_name, line);
  current_line.str("");
  current_line.str(line);

  current_line >> mem_free >> mem_free_value;

  return (mem_total_value - mem_free_value) / static_cast<float>(mem_total_value);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string time_spent_in_suspend, time_spent_in_idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream current_line(line);
    current_line >> time_spent_in_suspend >> time_spent_in_idle;
  }

  return static_cast<long int>(std::stol(time_spent_in_suspend));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line{};
  std::string process{};
  std::string process_number{};

  std::ifstream process_file(kProcDirectory + kStatFilename);

  if (process_file.is_open()) {
    while (std::getline(process_file, line)) {
      std::istringstream current_line(line);
      current_line >> process >> process_number;
      if (process == "processes") {
        return std::stoi(process_number);
      }
    }
  }

  return 0;
}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line{};
  std::string process{};
  std::string process_number{};

  std::ifstream process_file(kProcDirectory + kStatFilename);

  if (process_file.is_open()) {
    while (std::getline(process_file, line)) {
      std::istringstream current_line(line);
      current_line >> process >> process_number;
      if (process == "procs_running") {
        return std::stoi(process_number);
      }
    }
  }

  return 0;
}
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  std::string line{};
  std::ifstream file_name(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if(file_name.is_open())
  {
      std::getline(file_name, line);
      return line;
  }

  return std::string();
}
// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  std::string line{};
  std::string vm_size{};
  std::string ram_kbytes{};

  std::ifstream process_file(kProcDirectory + to_string(pid) + kStatusFilename);

  if (process_file.is_open()) {
    while (std::getline(process_file, line)) {
      std::istringstream current_line(line);
      current_line >> vm_size >> ram_kbytes;
      if (vm_size == "VmSize:") 
      {
        auto ram_in_kb = stol(ram_kbytes);
        constexpr int convertible_multiplier_to_megabytes{1000};

        return std::to_string(ram_in_kb/ convertible_multiplier_to_megabytes);
      }
    }
  }

  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  std::string line{};
  std::ifstream file_name(kProcDirectory + to_string(pid) + "/" + kStatusFilename );

  if(file_name.is_open())
  {
    while(std::getline(file_name, line))
    {
      std::istringstream current_line(line);
      std::string u_id{};
      std::string u_id_number{};
      current_line >> u_id >> u_id_number;

      if(u_id == "Uid:")
      {
        return u_id_number;
      }
    }
  }

  return string(); 
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  std::string line{};
  std::ifstream file_name(kPasswordPath);

  std::string uid = LinuxParser::Uid(pid);

  if(file_name.is_open())
  {
    while(std::getline(file_name, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream current_line(line);
      std::string user_name{};
      std::string char_x{};
      std::string third_string{};
      std::string forth_string{};

      current_line >> user_name >> char_x >> third_string >> forth_string;

      if((char_x == "x") && (third_string == uid) && (forth_string == uid))
      {
        return user_name;
      }
    }
  }
  return string(); 
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  std::string line{};
  std::ifstream file_name(kProcDirectory + to_string(pid) + kStatFilename);
  auto hertz = sysconf(_SC_CLK_TCK);

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

    return std::stol(all_elemnts.at(21)) / hertz;
  }

  return 0; 
}