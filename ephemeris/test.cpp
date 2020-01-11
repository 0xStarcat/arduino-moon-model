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

  // Ephemeris::setLocationOnEarth(40.71305, -74.66034); // NYC -- not needed for heliocentric coords
  for (int day = 1; day <= 28; day++)
  {
    for (int hour = 0; hour <= 23; hour++)
    {
      t.Year = 2020;
      t.Month = 2;
      t.Day = day;
      t.Hour = hour;
      t.Minute = 0;
      t.Second = 0;

      LunarPhaseMeasures lpm = Ephemeris::getLunarPhaseMeasures(day, t.Month, t.Year, t.Hour, t.Minute, t.Second);
      // std::cout << "\n******\n";
      std::cout << std::to_string(lpm.illuminatedFraction) << "\n";
      // std::cout << std::to_string(lpm.phaseDecimal) << "\n";
      // std::cout << std::to_string(t.Month) << "/" << std::to_string(t.Day) << "/" << t.Year << std::endl;
      // std::cout << std::to_string(lpm.apparentLongitude) << "\n";
    }
  }

  return 0;
}
