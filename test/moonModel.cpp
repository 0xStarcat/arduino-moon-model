#include <iostream>
#include "../time.cpp"

#include "../ephemeris/Calendar.cpp"
#include "../ephemeris/Ephemeris.cpp"

tmElements_t t = TimeWrapper::getSystemTime();
int main()
{

  LunarPhaseMeasures lunar = Ephemeris::getLunarPhaseMeasures(t.Day, t.Month, tmYearToCalendar(t.Year), t.Hour, t.Minute, t.Second);

  std::cout << std::to_string(lunar.illuminatedFraction) << "\n";
  std::cout << std::to_string(lunar.phaseDecimal);

  return 0;
}