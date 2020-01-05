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

const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

DateTime TimeWrapper::toUtcTime(uint8_t tzOffset, DateTime tm)
{
  Serial.println("Setting UTC time...");

  uint8_t ss = tm.second();
  uint8_t mm = tm.minute();
  uint8_t hh = tm.hour();
  uint8_t d = tm.day();
  uint8_t m = tm.month();
  uint16_t y = tm.year();

  hh += tzOffset;

  // if hour is 24+
  if (hh > 23)
  {
    hh -= 24;
    d += 1;

    // If days are > days in month
    if (d > daysInMonth[m + 1])
    {
      d = 1;
      m += 1;

      // if month > 12
      if (m > 12)
      {
        m = 1;
        y += 1;
      }
    }
  }

  DateTime time = DateTime(y, m, d, hh, mm, ss);
  return time;
}