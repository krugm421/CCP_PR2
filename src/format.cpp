#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long full_seconds = seconds % 60;
    long full_minutes = (long)(seconds / 60) % 60;
    long full_hours = (long)(seconds / 3600);
    return to_string(full_hours) + ":" + to_string(full_minutes) + ":" + to_string(full_seconds); 
}