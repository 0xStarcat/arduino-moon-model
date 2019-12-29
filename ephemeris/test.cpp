#include "Calendar.cpp"
#include "Ephemeris.cpp"
#include <iostream>
#include <math.h>

struct TimeObject
{
  int Year;
  int Month; // 1 - 12
  int Day;
  int Hour;
  int Minute;
  int Second;
};

int main()
{

  TimeObject t;
  t.Year = 2019;
  t.Month = 12;
  t.Day = 25;
  t.Hour = 12;
  t.Minute = 00;
  t.Second = 0;

  // Ephemeris::setLocationOnEarth(40.71305, -74.66034); // NYC -- not needed for heliocentric coords

  LunarPhaseMeasures lpm = Ephemeris::getLunarPhaseMeasures(t.Day, t.Month, t.Year, t.Hour, t.Minute, t.Second);

  std::cout << "\n******\n";
  std::cout << "IF: " << std::to_string(lpm.illuminatedFraction) << "\n";
  std::cout << "PD: " << std::to_string(lpm.phaseDecimal) << "\n";

  return 0;
}
