/*
 * Calendar.cpp
 *
 * Copyright (c) 2017 by Sebastien MARCHAND (Web:www.marscaper.com, Email:sebastien@marscaper.com)
 */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if ARDUINO
#include <Arduino.h>
#endif

#include "Calendar.hpp"

#define INT(value) (value >= 0 ? (long)(value) : (long)(value - 1))

JulianDay Calendar::julianDayForDate(FLOAT day, int month, int year)
{
  if (month <= 2)
  {
    year = year - 1;
    month = month + 12;
  }

  long A = INT(year / 100.0);
  long B = 2 - A + INT(A / 4);

  JulianDay julianDay;

  julianDay.day = INT(365.25 * (year + 4716)) + INT(30.6001 * (month + 1)) + (int)(day) + B - 1524.5;

  julianDay.time = 0.5 + (day - (int)day);

  // Adjust day and time if needed
  if (julianDay.time >= 1)
  {
    julianDay.time -= 1;

    julianDay.day += 1;
  }

  return julianDay;
};

JulianDay Calendar::julianDayForDateAndTime(int day, int month, int year,
                                            int hours, int minutes, int seconds)
{
  return julianDayForDate(day + hours / 24.0 + minutes / 1440.0 + seconds / 86400.0, month, year);
};