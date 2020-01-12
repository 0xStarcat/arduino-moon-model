/*
 * Ephemeris.cpp
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
// #include <stdio.h>
// #include <math.h>

#include "Ephemeris.hpp"
#include "utilities/luna.cpp"
#include "utilities/sun.cpp"

// Observer's coordinates on Earth
static FLOAT latitudeOnEarth = NAN;
static FLOAT longitudeOnEarth = NAN;

void Ephemeris::setLocationOnEarth(FLOAT floatingLatitude, FLOAT floatingLongitude)
{
  latitudeOnEarth = floatingLatitude;
  longitudeOnEarth = floatingLongitude;
}
