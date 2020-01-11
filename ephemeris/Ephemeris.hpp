/*
 * Ephemeris.hpp
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

// To speed up upload, you can disable planets calculations if not needed.
// VSOP87 and ELP2000 will not be loaded and solarSystemObjectAtDateAndTime()
// will simply return an empty object.

#ifndef Ephemeris_h
#define Ephemeris_h

#include "Calendar.hpp"

#include "utilities/lunaPeriodicTerms.hpp"

#ifndef PI
#define PI 3.1415926535
#endif

// Trigonometry using degrees
#define SIND(value) sin((value)*0.0174532925)
#define COSD(value) cos((value)*0.0174532925)
#define TAND(value) tan((value)*0.0174532925)

#define ACOSD(value) (acos((value)) * 57.2957795131);
#define ATAND(value) (atan((value)) * 57.2957795131);

// Limit range
#define LIMIT_DEGREES_TO_360(value) (value) >= 0 ? ((value) - (long)((value)*0.0027777778) * 360) : (((value) - (long)((value)*0.0027777778) * 360) + 360);
#define LIMIT_DEGREES_TO_180(value) (value) >= 0 ? ((value) - (long)((value)*0.0027777778) * 180) : (((value) - (long)((value)*0.0027777778) * 180) + 180);
#define LIMIT_HOURS_TO_24(value) (value) >= 0 ? ((value) - (long)((value)*0.0416666667) * 24) : ((value) + 24);
#define LIMIT_DEC_TO_90(value) (value);

// Convert degrees
#define DEGREES_TO_RADIANS(value) ((value)*0.0174532925)
#define DEGREES_TO_HOURS(value) ((value)*0.0666666667)
#define DEGREES_MINUTES_SECONDES_TO_SECONDS(deg, min, sec) ((FLOAT)(deg)*3600 + (FLOAT)(min)*60 + (FLOAT)sec)

// Convert radians
#define RADIANS_TO_DEGREES(value) ((value)*57.2957795131)
#define RADIANS_TO_HOURS(value) ((value)*3.81971863)

// Convert hours
#define HOURS_TO_RADIANS(value) ((value)*0.261799388)
#define HOURS_MINUTES_SECONDS_TO_SECONDS(hour, min, sec) ((FLOAT)(hour)*3600 + (FLOAT)(min)*60 + (FLOAT)sec)
#define HOURS_MINUTES_SECONDS_TO_DECIMAL_HOURS(hour, min, sec) ((FLOAT)(hour) + (FLOAT)(min)*0.0166666667 + (FLOAT)(sec)*0.0002777778)
#define HOURS_TO_DEGREES(value) ((value)*15)

// Convert seconds
#define SECONDS_TO_DECIMAL_DEGREES(value) ((FLOAT)(value)*0.0002777778)
#define SECONDS_TO_DECIMAL_HOURS(value) ((FLOAT)(value)*0.0002777778)

#define T_WITH_JD(day, time) ((day - 2451545.0 + time) / 36525)

/*! This structure describes geocentric coordinates. */
struct GeocentricCoordinates
{
  /*! Floating value for longitude. */
  FLOAT lon;

  /*! Floating value for latitude.*/
  FLOAT lat;

  /* ! distance in KM to the earth */
  FLOAT earthDistanceKm;
};

/*! Describes the moon phases */
struct LunarPhaseMeasures
{
  FLOAT apparentLongitude;
  FLOAT illuminatedFraction; /* 0 - 1 the % of the moon that's illuminated */
  FLOAT phaseDecimal;        /* 0 - 1 the current phase of the moon. 0 = new, 0.25 = first quarter, 0.5 = full, 0.75 = last quarter */
};

/*!
 * This class is used for astronomical calculations. The code is based on the book "Astronomical Algorithms" by Jean Meeus.
 */
class Ephemeris
{

public:
  /*! Flip longitude coordinate. Default: West is negative and East is positive. */
  static void flipLongitude(bool flip);

  /*! Set location on earth (used for horizontal coordinates conversion). */
  static void setLocationOnEarth(FLOAT floatingLatitude, FLOAT floatingLongitude);

  /*! Set location on earth (used for horizontal coordinates conversion). */
  static void setLocationOnEarth(FLOAT latDegrees, FLOAT latMinutes, FLOAT latSeconds,
                                 FLOAT lonDegrees, FLOAT lonMinutes, FLOAT lonSeconds);

  static FLOAT getLunarIllumination(GeocentricCoordinates moonCoords, GeocentricCoordinates sunCoords);

  static FLOAT getLunarIlluminationLowerAccuracy(unsigned int day, unsigned int month, unsigned int year,
                                                 unsigned int hours, unsigned int minutes, unsigned int seconds);

  static FLOAT getLunarPhaseDecimal(GeocentricCoordinates moonCoords, GeocentricCoordinates sunCoords);

  static FLOAT getLunarPhaseDecimalLowerAccuracy(unsigned int day, unsigned int month, unsigned int year,
                                                 unsigned int hours, unsigned int minutes, unsigned int seconds);

  static LunarPhaseMeasures getLunarPhaseMeasures(unsigned int day, unsigned int month, unsigned int year,
                                                  unsigned int hours, unsigned int minutes, unsigned int seconds);

private:
  /* ! Compute sun's heliocentric coordnates.
    * Reference: Astrological Algorithims (2015) ch 25
  */
  static GeocentricCoordinates geocentricCoordinatesForSun(FLOAT T);

  /* ! Compute earthMoon's heliocentric coordnates.
    * Reference: Astrological Algorithims (2015) ch 47
  */
  static GeocentricCoordinates geocentricCoordinatesForEarthsMoon(FLOAT T);

  /*! Compute the true obliquity (angle in floating degrees) of the ecliptic,
     *  delta obliquity and delta nutation for T.
     *  Reference: Chapter 13, page 53: Nutation et obliquité de l'écliptique. */
  static FLOAT obliquityAndNutationForT(FLOAT T, FLOAT *deltaObliquity, FLOAT *deltaNutation);

  /*! Compute lunar longitude terms.
     *  Reference: Chapter 47 (2015), page 339. */
  static FLOAT sumLunarLongitudeTerms(FLOAT E, FLOAT L1, FLOAT D, FLOAT M, FLOAT M1, FLOAT F, FLOAT A1, FLOAT A2);

  /*! Compute lunar distance terms.
     *  Reference: Chapter 47 (2015), page 341. */
  static FLOAT sumLunarDistanceTerms(FLOAT E, FLOAT D, FLOAT M, FLOAT M1, FLOAT F);

  /*! Compute lunar latitude terms.
     *  Reference: Chapter 47 (2015), page 341. */
  static FLOAT sumLunarLatitudeTerms(FLOAT E, FLOAT L1, FLOAT D, FLOAT M, FLOAT M1, FLOAT F, FLOAT A1, FLOAT A3);
};
#endif
