#include <string>
#include "format.h"


#include <sstream>
#include <iostream>
#include <fstream>

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long full_hours = (long)(seconds / 3600);
    long full_minutes = (long)((seconds % 3600) / 60);
    long full_seconds = (long)((seconds % 3600) % 60);

    string full_hours_str = std::to_string(full_hours);
    string full_minutes_str = std::to_string(full_minutes);
    string full_seconds_str = std::to_string(full_seconds);
    string time = string(2 - full_hours_str.length(), '0') + full_hours_str + ':' +
                string(2 - full_minutes_str.length(), '0') + full_minutes_str + ':' +
                string(2 - full_seconds_str.length(), '0') + full_seconds_str;

    return time;
}