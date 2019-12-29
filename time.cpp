#include <string.h>
#include <stdio.h>
#include "time.hpp"

const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

tmElements_t TimeWrapper::buildTime(const char *str, tmElements_t tm)
{

  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
    return tm;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return tm;
}

tmElements_t TimeWrapper::buildDate(const char *str, tmElements_t tm)
{

  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3)
    return tm;

  // Finds current month index by matching system month string with monthName list
  for (monthIndex = 0; monthIndex < 12; monthIndex++)
  {
    if (strcmp(Month, monthName[monthIndex]) == 0)
      break;
  }
  if (monthIndex >= 12)
    return tm;

  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);

  return tm;
}

tmElements_t TimeWrapper::getSystemTime()
{
  tmElements_t tm;

  tm = buildDate(__DATE__, tm);
  tm = buildTime(__TIME__, tm);

  return tm;
}
