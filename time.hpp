#include <Time.h>
#include <TimeLib.h>
#include <RTClib.h>

class TimeWrapper
{
public:
  static tmElements_t buildTime(const char *str, tmElements_t tm);

  static tmElements_t buildDate(const char *str, tmElements_t tm);

  static tmElements_t getSystemTime();

  static DateTime setTimeOffset(DateTime tm, uint8_t hourOffset, uint16_t dayOffset = 0);
};