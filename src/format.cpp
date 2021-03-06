#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>
using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
#define SECOND_IN_HOUR (1 * 60 * 60)
#define SECOND_IN_MIN (1 * 60)
/* MAX time supported is: 99 Hours + 59 Min + 59 Sec*/
#define MAX_TIME_SUPPORTED (99 * SECOND_IN_HOUR + 59 * SECOND_IN_MIN + 59)

  string time = "";
  if (seconds == 0) {
    time = "00:00:00";
  } else if (seconds < MAX_TIME_SUPPORTED) {
    int hours = 0;
    int min = 0;
    int sec = 0;

    while (seconds > 0) {
      if (seconds >= SECOND_IN_HOUR) {
        hours++;
        seconds -= SECOND_IN_HOUR;
      } else if (seconds >= SECOND_IN_MIN) {
        min++;
        seconds -= SECOND_IN_MIN;
      } else {
        sec = (int)seconds;
        seconds = 0; /* Exit condition */
      }
    }

    std::ostringstream str;
    str << std::setw(2) << std::setfill('0') << hours;
    str << ":";
    str << std::setw(2) << std::setfill('0') << min;
    str << ":";
    str << std::setw(2) << std::setfill('0') << sec;
    time = str.str();
  } else {
    time = "99:59:59";
  }
  return time;
}