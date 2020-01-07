#include "Calendar.cpp"
#include "Ephemeris.cpp"
#include <iostream>
#include <math.h>

struct TimeObject
{
  uint16_t Year;
  uint8_t Month; // 1 - 12
  uint8_t Day;
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;
};

int main()
{

  TimeObject t;
  t.Year = 2020;
  t.Month = 1;
  t.Day = 6;
  t.Hour = 23;
  t.Minute = 40;
  t.Second = 14;

  // Ephemeris::setLocationOnEarth(40.71305, -74.66034); // NYC -- not needed for heliocentric coords

  LunarPhaseMeasures lpm = Ephemeris::getLunarPhaseMeasures(t.Day, t.Month, t.Year, t.Hour, t.Minute, t.Second);

  std::cout << "\n******\n";
  std::cout << "IF: " << std::to_string(lpm.illuminatedFraction) << "\n";
  std::cout << "PD: " << std::to_string(lpm.phaseDecimal) << "\n";
  std::cout << "AL: " << std::to_string(lpm.apparentLongitude) << "\n";

  return 0;
}
