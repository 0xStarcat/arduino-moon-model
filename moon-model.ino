// Sketch uses 19322 bytes.
// Global variables use 833 bytes of dynamic memory.

#include "ephemeris/Calendar.cpp"
#include "ephemeris/Ephemeris.cpp"
#include "time.hpp"
#include "utilities.hpp"
#include "drawMoon.hpp"
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
uint8_t secondCounter = 0;      // counts seconds until update calc/paint

DrawConstants drawConstants;

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

  initialize();

  Serial.println("Debug variables: ");
  Serial.print("Moon X center: ");
  Serial.println(drawConstants.moonCenterX);
  Serial.print("Moon Y center: ");
  Serial.println(drawConstants.moonCenterY);
  Serial.print("Moon radius: ");
  Serial.println(drawConstants.moonRadius);
}

void loop()
{
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
    DrawMoon::drawMoonOutline(tft, drawConstants);
    DrawMoon::drawLightBody(tft, drawConstants, lunar.phaseDecimal);
    DrawMoon::drawIlluminatedMoonOutline(tft, drawConstants, lunar.illuminatedFraction);
  }
  // only increment secondCounter when click increments 1 second
  else if (tm.Second != lastSecond)
  {
    secondCounter += 1;
  }
}

void initialize()
{
  Serial.println("Initializing...");
  // SET GLOBAL VARIABLES after init because otherwise dimensions are bad
  drawConstants.moonPadding = 2; // px
  drawConstants.moonCenterX = (tft.maxX() - drawConstants.moonPadding) / 2;
  drawConstants.moonCenterY = (tft.maxY() - drawConstants.moonPadding) / 2;
  drawConstants.moonRadius = drawConstants.moonCenterX / 2;
  drawConstants.moonLeft = drawConstants.moonCenterX - drawConstants.moonRadius;
  drawConstants.moonRight = drawConstants.moonCenterX + drawConstants.moonRadius;
  drawConstants.moonTop = drawConstants.moonCenterY - drawConstants.moonRadius;
  drawConstants.moonBottom = drawConstants.moonCenterY + drawConstants.moonRadius;

  // Init first draw
  tft.setBacklight(true);
  tft.setDisplay(true);
  DrawMoon::drawMoonOutline(tft, drawConstants);
};

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