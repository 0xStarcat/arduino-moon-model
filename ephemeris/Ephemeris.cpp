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

FLOAT Ephemeris::obliquityAndNutationForT(FLOAT T, FLOAT *deltaObliquity, FLOAT *deltaNutation)
{
  FLOAT TSquared = T * T;
  FLOAT TCubed = TSquared * T;

  FLOAT Ls = 280.4565 + T * 36000.7698 + TSquared * 0.000303;
  Ls = LIMIT_DEGREES_TO_360(Ls);

  FLOAT Lm = 218.3164 + T * 481267.8812 - TSquared * 0.001599;
  Lm = LIMIT_DEGREES_TO_360(Lm);

  FLOAT Ms = 357.5291 + T * 35999.0503 - TSquared * 0.000154;
  Ms = LIMIT_DEGREES_TO_360(Ms);

  FLOAT Mm = 134.9634 + T * 477198.8675 + TSquared * 0.008721;
  Mm = LIMIT_DEGREES_TO_360(Mm);

  FLOAT omega = 125.0443 - T * 1934.1363 + TSquared * 0.008721;
  omega = LIMIT_DEGREES_TO_360(omega);

  // Delta Phi
  FLOAT dNutation =
      -(17.1996 + 0.01742 * T) * SIND(omega) - (1.3187 + 0.00016 * T) * SIND(2 * Ls) - 0.2274 * SIND(2 * Lm) + 0.2062 * SIND(2 * omega) + (0.1426 - 0.00034 * T) * SIND(Ms) + 0.0712 * SIND(Mm) - (0.0517 - 0.00012 * T) * SIND(2 * Ls + Ms) - 0.0386 * SIND(2 * Lm - omega) - 0.0301 * SIND(2 * Lm + Mm) + 0.0217 * SIND(2 * Ls - Ms) - 0.0158 * SIND(2 * Ls - 2 * Lm + Mm) + 0.0129 * SIND(2 * Ls - omega) + 0.0123 * SIND(2 * Lm - Mm);

  if (deltaNutation)
  {
    *deltaNutation = dNutation;
  }

  // Delta Eps
  FLOAT dObliquity =
      +(9.2025 + 0.00089 * T) * COSD(omega) + (0.5736 - 0.00031 * T) * COSD(2 * Ls) + 0.0977 * COSD(2 * Lm) - 0.0895 * COSD(2 * omega) + 0.0224 * COSD(2 * Ls + Ms) + 0.0200 * COSD(2 * Lm - omega) + 0.0129 * COSD(2 * Lm + Mm) - 0.0095 * COSD(2 * Ls - Ms) - 0.0070 * COSD(2 * Ls - omega);

  if (deltaObliquity)
  {
    *deltaObliquity = dObliquity;
  }

  FLOAT eps0 = DEGREES_MINUTES_SECONDES_TO_SECONDS(23, 26, 21.448) - T * 46.8150 - TSquared * 0.00059 + TCubed * 0.001813;

  FLOAT obliquity = eps0 + dObliquity;
  obliquity = SECONDS_TO_DECIMAL_DEGREES(obliquity);

  return obliquity;
}

void Ephemeris::setLocationOnEarth(FLOAT floatingLatitude, FLOAT floatingLongitude)
{
  latitudeOnEarth = floatingLatitude;
  longitudeOnEarth = floatingLongitude;
}
