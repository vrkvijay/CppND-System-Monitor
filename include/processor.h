#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   void SetCpuValues(const std::vector<std::string>& current_cpu_values);
   float ComputeCpuUtilization(const std::vector<std::string>& );

   float cpu_percentage_{0.0};
   bool is_previous_value_set_{false};
   long prev_idle_{};
   long prev_iowait{};
   long prev_user_{};
   long prev_nice_{};
   long prev_system_{};
   long prev_irq_{};
   long prev_soft_irq_{};
   long prev_steal_{};
};

#endif