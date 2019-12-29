// Sketch uses 19322 bytes.
// Global variables use 833 bytes of dynamic memory.

#include "ephemeris/Calendar.cpp"
#include "ephemeris/Ephemeris.cpp"
#include "time.hpp"
#include "utilities.hpp"
#include <DS1307RTC.h>

#include "SPI.h"
#include "TFT_22_ILI9225.h"

#define TFT_CS 10
#define TFT_DC 9    // RS
#define TFT_POCI 11 // SDI / SDA / A4 // 11
#define TFT_CLK 13  // SCK A5 // 13
#define TFT_RST A3
#define TFT_LED 0 // 0 if wired to +5V directly
#define TFT_BRIGHTNESS 200
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_DC, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

const uint8_t UPDATE_SPEED = 5; // seconds
bool initialized = false;
uint8_t secondCounter = 0; // counts seconds until update calc/paint

uint16_t moonPadding = 2; // px
uint16_t moonCenterX;     // px
uint16_t moonCenterY;     // px
uint16_t moonRadius;      // px
uint16_t moonLeft;        // px
uint16_t moonRight;       // px
uint16_t moonTop;         // px
uint16_t moonBottom;      // px

tmElements_t tm = TimeWrapper::getSystemTime();
void setup()
{
  Serial.begin(9600);
  RTC.write(tm);

  // INITIALIZE SCREEN
  tft.begin();
  tft.clear();
  tft.setBacklight(false);
  tft.setDisplay(false);
  tft.setOrientation(0);

  Serial.println("Beginning!");
  Serial.println("********************");
  Serial.println("Dimensions: ");
  Serial.print("X: ");
  Serial.println(tft.maxX());
  Serial.print("Y: ");
  Serial.println(tft.maxY());

  // SET GLOBAL VARIABLES after init because otherwise dimensions are bad
  moonCenterX = (tft.maxX() - moonPadding) / 2;
  moonCenterY = (tft.maxY() - moonPadding) / 2;
  moonRadius = moonCenterX / 2;
  moonLeft = moonCenterX - moonRadius;
  moonRight = moonCenterX + moonRadius;
  moonTop = moonCenterY - moonRadius;
  moonBottom = moonCenterY + moonRadius;

  Serial.println("Debug variables: ");
  Serial.print("Moon X center: ");
  Serial.println(moonCenterX);
  Serial.print("Moon Y center: ");
  Serial.println(moonCenterY);
  Serial.print("Moon radius: ");
  Serial.println(moonRadius);
}

void loop()
{

  if (!initialized)
  {
    initialize();
  };

  // assign lastSecond for comparison later
  int lastSecond = tm.Second;
  RTC.read(tm);

  int sensorIlluminationValue = analogRead(A0);
  float outputIlluminationValue = map(sensorIlluminationValue, 0, 1023, 0, 1000);
  outputIlluminationValue = (outputIlluminationValue / 1000) - 0.05;
  int sensorPhaseValue = analogRead(A1);
  float outputPhaseValue = map(sensorPhaseValue, 0, 1023, 0, 1000);
  outputPhaseValue = (outputPhaseValue / 1000) - 0.05;

  if (secondCounter >= UPDATE_SPEED)
  {
    // reset secondCounter
    secondCounter = 0;

    Serial.println("*********************");
    printTime(tm);

    // LunarPhaseMeasures lunar = Ephemeris::getLunarPhaseMeasures(tm.Day, tm.Month, tmYearToCalendar(tm.Year), tm.Hour, tm.Minute, tm.Second);

    // Debug mode with potentiometers
    LunarPhaseMeasures lunar;
    lunar.apparentLongitude = 0;
    lunar.illuminatedFraction = outputIlluminationValue;
    lunar.phaseDecimal = outputPhaseValue;
    printLunarMeasures(lunar);

    tft.clear();
    drawMoonOutline();
    drawLightBody(lunar.phaseDecimal);
    drawIlluminatedMoonOutline(lunar.illuminatedFraction);
    // tft.drawPixel(moonLeft, moonCenterY, COLOR_BLUE);
    // tft.drawPixel(moonRight, moonCenterY, COLOR_ORANGE);
    // tft.drawPixel(moonCenterX, moonTop, COLOR_GREEN);
    // tft.drawPixel(moonCenterX, moonBottom, COLOR_RED);
    // tft.drawLine(moonLeft, moonCenterY, moonRight, moonCenterY, COLOR_YELLOW);
    // tft.drawLine(moonCenterX, moonTop, moonCenterX, moonBottom, COLOR_MAGENTA);

    uint8_t moonLeftLimit = moonRight - (moonRight * lunar.illuminatedFraction);

    int xNormal = 0;
    int yNormal = 0;

    //    NEED TO LEARN TRIG
    // single outer outline
    // for (int i = 90; i > -90; i--)
    // {
    //   double radians = i * PI / 180;
    //   double px = moonCenterX + moonRadius * cos(radians);
    //   double py = moonCenterY + moonRadius * sin(radians);
    //   tft.drawPixel(px, py, COLOR_WHITE);
    // };

    // Thin outer circular layer
    // for (int rx = (moonRadius - (15 * lunar.illuminatedFraction)); rx < moonRadius; rx++)
    // {
    //   for (int i = (-90 * lunar.illuminatedFraction); i < (90 * lunar.illuminatedFraction); i++)
    //   {
    //     double radians = i * PI / 180;
    //     double px = moonCenterX + rx * cos(radians);
    //     double py = moonCenterY + rx * sin(radians);

    //     tft.drawPixel(px, py, COLOR_GREEN);
    //   };
    // };

    // NEED TO LEARN TRIG
    for (int rx = moonRadius - (moonRadius * lunar.illuminatedFraction); rx < moonRadius; rx++)
    {

      // paint start from top of circle and swings around to the bottom
      // for (int i = -90; i < 90; i++)
      // {
      //   double radians = i * PI / 180;
      //   // double px = (moonCenterX - moonCenterX * lunar.illuminatedFraction) + rx * cos(radians);
      //   // double py = moonCenterY + rx * sin(radians);
      //   double px = moonCenterX + rx * cos(radians);
      //   double py = moonCenterY + rx * sin(radians) / sqrt(pow(cos(radians), 2));

      //   double moonEdgeY = moonCenterY + moonRadius * sin(radians);
      //   // Limit Y to inside circle
      //   if ((i <= 0 && py >= (moonEdgeY)) || (i >= 0 && py <= (moonEdgeY)))
      //   {
      //     tft.drawPixel(px, py, COLOR_GREEN);
      //     // Serial.print(i);
      //     // Serial.print(":::: - outer Y: ");
      //     // Serial.print((moonEdgeY));
      //     // Serial.print(" - inner: ");
      //     // Serial.print(py);
      //     // Serial.print(" - sin: ");
      //     // Serial.print(rx * sin(radians));
      //     // Serial.println("***");
      //   };
      // };
    };

    // Moon is drawn from right to left and top to bottom to mimic natural appearance;
    // for (uint8_t x0 = moonRight; x0 >= moonLeftLimit; x0--)
    // {
    //   xNormal += 1;
    //   tft.drawPixel(x0, moonCenterY, COLOR_YELLOW);
    //   for (uint8_t y0 = moonTop; y0 < moonBottom; y0++)
    //   {
    //     yNormal += 1;
    //     if (y0 < moonCenterY && x0 > moonCenterX) // top right half of crescent
    //     {
    //       tft.drawPixel(x0, y0, COLOR_RED);
    //       if ((moonRadius - xNormal) <= y0)
    //       {
    //         // Serial.print(x0);
    //         // Serial.print("x");
    //         // Serial.print(y0);
    //         // Serial.print("||");
    //         // tft.drawPixel(x0, y0, COLOR_RED);
    //       };
    //     }
    //     else if (y0 > moonCenterY && x0 > moonCenterX) // bottom right half of crescent
    //     {
    //       tft.drawPixel(x0, y0, COLOR_BLUE);
    //       if (y0 >= (moonRadius + x0))
    //       {
    //         // tft.drawPixel(x0, y0, COLOR_BLUE);
    //       };
    //     };
    //   };
    // };
  }
  // only increment secondCounter when click increments 1 second
  else if (tm.Second != lastSecond)
  {
    secondCounter += 1;
  }
}

void initialize()
{
  tft.setBacklight(true);
  tft.setDisplay(true);
  delay(1000);
  drawMoonOutline();
  initialized = true;
};

void drawProjectionShape(float phaseDecimal, uint16_t color)
{
  // no smaller than 1Radius
  // 1 Radius + 1 Radius * 8phaseDecimal
  float projectionSize = max(moonRadius, moonRadius + (moonRadius * (phaseDecimal * 8)));
  Serial.println("*** proj size: ");
  Serial.println(projectionSize);

  // Increment faster to speed up draw time when projectionSize is very large
  // ...until paintbrush touches moon (than increment normally)
  for (int rx = 0; rx < projectionSize; (projectionSize > (moonRadius * 2) && rx < (projectionSize - (moonRadius * 2))) ? rx += (projectionSize / 4) : rx += 1)
  {
    for (int i = 0; i < 360; i++)
    {
      double radians = i * PI / 180;

      double px = (moonCenterX - (projectionSize) + moonRadius - (moonRadius * (phaseDecimal * 3.3))) + rx * cos(radians);
      double py = moonCenterY + rx * sin(radians);

      tft.drawPixel(px, py, COLOR_BLACK);
    };
  }
}

void drawLightBody(float phaseDecimal)
{

  float fullMoonOrb = 0.05;

  // full moon, Waxing crescent, waning crescent, fill moon
  if ((phaseDecimal < 0.25) || (phaseDecimal > 0.75 && phaseDecimal < 1) || (phaseDecimal > 0.5 - fullMoonOrb && phaseDecimal < 0.5 + fullMoonOrb))
  {

    tft.fillCircle(moonCenterX, moonCenterY, moonRadius, COLOR_WHITE);
    drawProjectionShape(phaseDecimal, COLOR_BLACK);
  }
  else
  {
    drawProjectionShape(phaseDecimal, COLOR_WHITE);
  }
};

void drawMoonOutline()
{
  tft.drawCircle(moonCenterX, moonCenterY, moonRadius, COLOR_WHITE);
};

void drawIlluminatedMoonOutline(float illuminatedFraction)
{
  // Thin outer circular layer
  // for (int rx = (moonRadius - (15 * illuminatedFraction)); rx < moonRadius; rx++)
  // {
  for (int i = (-90 * (illuminatedFraction * 4)); i < (90 * (illuminatedFraction * 4)); i++)
  {
    double radians = i * PI / 180;
    double px = moonCenterX + moonRadius * cos(radians);
    double py = moonCenterY + moonRadius * sin(radians);

    tft.drawPixel(px, py, COLOR_WHITE);
  };
  // };
}

void printSign(LunarPhaseMeasures lunar)
{
  int index = floor(lunar.apparentLongitude / 30);
  char *sign = Utilities::getSignByIndex(index);
  Serial.print("Sign: ");
  Serial.println(sign);
};

void printLunarMeasures(LunarPhaseMeasures lunar)
{
  // Serial.print("Longitude: ");
  // Serial.println(lunar.apparentLongitude, 4);
  // printSign(lunar);
  Serial.print("Illumination: ");
  Serial.println(lunar.illuminatedFraction, 4);
  Serial.print("Phase Decimal: ");
  Serial.println(lunar.phaseDecimal, 4);
}

void printTime(tmElements_t tm)
{
  Serial.print(String(tm.Month));
  Serial.print("/");
  Serial.print(String(tm.Day));
  Serial.print("/");
  Serial.print(String(tmYearToCalendar(tm.Year)));
  Serial.print(" -- ");
  print2digits(tm.Hour);
  Serial.print(":");
  print2digits(tm.Minute);
  Serial.print(":");
  print2digits(tm.Second);
  Serial.println();
}

void print2digits(int number)
{
  if (number >= 0 && number < 10)
  {
    Serial.write('0');
  }
  Serial.print(number);
}