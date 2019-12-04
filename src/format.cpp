#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    int hours = (seconds / 3600) % 24;
    int minutes = (seconds % 3600) / 60;
    int second = seconds % 60;

    std::string hour = hours >= 10 ? std::to_string(hours) : "0" + std::to_string(hours);
    std::string minute = minutes >= 10 ? std::to_string(minutes) : "0" + std::to_string(minutes);
    std::string second_str = second >= 10 ? std::to_string(second) : "0" + std::to_string(second);
  
    return hour + ":" +minute + ":" + second_str;
}