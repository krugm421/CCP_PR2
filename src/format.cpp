#include <string>

#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long full_hours = (long)(seconds / 3600);
    long full_minutes = (long)((seconds % 3600) / 60);
    long full_seconds = (long)((seconds % 3600) % 60);
    
    
    return to_string(full_hours) + ":" + to_string(full_minutes) + ":" + to_string(full_seconds); 
}

