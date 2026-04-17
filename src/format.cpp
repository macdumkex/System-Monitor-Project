#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hours = seconds / 3600;
    long minutes = (seconds % 3600) / 60;
    long remainingSeconds = seconds % 3600 % 60;
    std::string hoursString = std::to_string(hours);
    if (hoursString.length() < 2) {
        hoursString = "0" + hoursString;
    }
    std::string minutesString = std::to_string(minutes);
    if (minutesString.length() < 2) {
        minutesString = "0" + minutesString;
    }
    std::string remainingSecondsString = std::to_string(remainingSeconds);
    if (remainingSecondsString.length() < 2) {
        remainingSecondsString = "0" + remainingSecondsString;
    }
    std::string time =  hoursString + ":" + minutesString + ":" + remainingSecondsString;
    return time;
}