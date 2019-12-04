#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
  std::string line{};
  std::ifstream file_name(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

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

    if(!is_previous_value_set_)
    {
        SetCpuValues(all_elemnts);
        is_previous_value_set_ = true;
        return 0.0;
    }

    cpu_percentage_ = ComputeCpuUtilization(all_elemnts);
    SetCpuValues(all_elemnts);
  }
  
  return cpu_percentage_; 
}

float Processor::ComputeCpuUtilization(const std::vector<std::string>& all_elemnts)
{
    auto previdle = prev_idle_ + prev_iowait;
    auto idle = std::stol(all_elemnts[4]) + std::stol(all_elemnts[5]);
    auto prevnonidle = prev_user_ + prev_nice_ + prev_system_ + prev_irq_ + prev_soft_irq_ + prev_steal_;
    auto nonidle = std::stol(all_elemnts[1]) +std::stol(all_elemnts[2]) + std::stol(all_elemnts[3]) +
                    std::stol(all_elemnts[6]) + std::stol(all_elemnts[7]) + std::stol(all_elemnts[8]);
    auto prevtotal = previdle + prevnonidle;
    auto total = idle + nonidle;

    auto totalid = total - prevtotal;
    auto idled = idle - previdle;

    return (totalid - idled) / static_cast<float>(totalid);
}

void Processor::SetCpuValues(const std::vector<std::string>& current_cpu_values)
{
    prev_user_ = std::stol(current_cpu_values[1]);
    prev_nice_ = std::stol(current_cpu_values[2]);
    prev_system_ = std::stol(current_cpu_values[3]);
    prev_idle_ = std::stol(current_cpu_values[4]);
    prev_iowait = std::stol(current_cpu_values[5]);
    prev_irq_ = std::stol(current_cpu_values[6]);
    prev_soft_irq_ = std::stol(current_cpu_values[7]);
    prev_steal_ = std::stol(current_cpu_values[8]);
}