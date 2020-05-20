#include "format.h"

#include <string>

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long time = seconds;
  long hour = time / 3600;
  time = time % 3600;
  long min = time / 60;
  long sec = time % 60;

  string strHour, strMin, strSec;

  if (hour < 10) {
    strHour = "0" + to_string(hour);
  } else {
    strHour = to_string(hour);
  }

  if (min < 10) {
    strMin = "0" + to_string(min);
  } else {
    strMin = to_string(min);
  }

  if (sec < 10) {
    strSec = "0" + to_string(sec);
  } else {
    strSec = to_string(sec);
  }

  return strHour + ":" + strMin + ":" + strSec;
}
