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


    string hours_str = std::to_string(full_hours);
    string minutes_str = std::to_string(full_minutes);
    string seconds_str = std::to_string(full_seconds);
    string time = string(2 - hours_str.length(), '0') + hours_str + ':' +
                string(2 - minutes_str.length(), '0') + minutes_str + ':' +
                string(2 - seconds_str.length(), '0') + seconds_str;

    // return to_string(full_hours) + ":" + to_string(full_minutes) + ":" + to_string(full_seconds) + " :: " + to_string(seconds);
    return time;
}

