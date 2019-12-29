#include "Time/TimeLib.h"

class TimeWrapper
{
public:
  static tmElements_t buildTime(const char *str, tmElements_t tm);

  static tmElements_t buildDate(const char *str, tmElements_t tm);

  static tmElements_t getSystemTime();
};