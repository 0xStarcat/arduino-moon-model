// Sketch uses 19322 bytes.
// Global variables use 833 bytes of dynamic memory.

#include "ephemeris/Calendar.cpp"
#include "ephemeris/Ephemeris.cpp"
#include "time.hpp"
#include "utilities.hpp"
#include "drawMoon.hpp"
#include <DS1307RTC.h>

// #include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

// #define TFT_CLK 13  // SCK A5 // 13
// #define TFT_POCI 11 // SDI / SDA / A4 // 11
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8 // RS
// #define TFT_PICO 0 // SDI / SDA / A4 // 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const uint8_t UPDATE_SPEED = 10; // seconds
uint8_t secondCounter = 0;       // counts seconds until update calc/paint

DrawConstants drawConstants;

tmElements_t tm = TimeWrapper::getSystemTime();

void setup()
{
  Serial.begin(9600);
  RTC.write(tm);

  // Serial.println("Beginning!");
  // Serial.println("********************");
  // Serial.println("Dimensions: ");
  // Serial.print("X: ");
  // Serial.println(tft.width());
  // Serial.print("Y: ");
  // Serial.println(tft.height());

  initialize();
}

void loop()
{
  // assign lastSecond for comparison later
  int lastSecond = tm.Second;
  RTC.read(tm);

  if (secondCounter >= UPDATE_SPEED)
  {
    // reset secondCounter
    secondCounter = 0;
    Serial.println("*********************");
    printTime(tm);
    drawMoonToMeasurements();
  }
  // only increment secondCounter when click increments 1 second
  else if (tm.Second != lastSecond)
  {
    secondCounter += 1;
  }
}

void initialize()
{
  // Serial.println("Initializing...");
  // tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  tft.setRotation(0);

  // SET GLOBAL VARIABLES after init because otherwise dimensions are bad
  drawConstants.moonPadding = 2; // px
  drawConstants.moonCenterX = (tft.width() - drawConstants.moonPadding) / 2;
  drawConstants.moonCenterY = (tft.height() - drawConstants.moonPadding) / 2;
  drawConstants.moonRadius = drawConstants.moonCenterX / 2;

  // Debug variables
  // Serial.println("Debug variables: ");
  // Serial.print("Moon X center: ");
  // Serial.println(drawConstants.moonCenterX);
  // Serial.print("Moon Y center: ");
  // Serial.println(drawConstants.moonCenterY);
  // Serial.print("Moon radius: ");
  // Serial.println(drawConstants.moonRadius);

  // Get time
  RTC.read(tm);

  // Init first draw
  drawMoonToMeasurements();
};

void drawMoonToMeasurements()
{
  LunarPhaseMeasures lunar = Ephemeris::getLunarPhaseMeasures(tm.Day, tm.Month, tmYearToCalendar(tm.Year), tm.Hour, tm.Minute, tm.Second);

  // Debug with potentiometers
  // int sensorIlluminationValue = analogRead(A0);
  // float outputIlluminationValue = map(sensorIlluminationValue, 0, 1023, 0, 1000);
  // outputIlluminationValue = (outputIlluminationValue / 1000) - 0.05;
  // int sensorPhaseValue = analogRead(A1);
  // float outputPhaseValue = map(sensorPhaseValue, 0, 1023, 0, 1000);
  // outputPhaseValue = (outputPhaseValue / 1000) - 0.05;

  // Debug mode with potentiometers
  // LunarPhaseMeasures lunar;
  // lunar.apparentLongitude = 0;
  // lunar.illuminatedFraction = outputIlluminationValue;
  // lunar.phaseDecimal = outputPhaseValue;
  printLunarMeasures(lunar);

  tft.fillScreen(ST7735_BLACK);
  DrawMoon::drawMoonOutline(tft, drawConstants, ST7735_WHITE);

  if (lunar.phaseDecimal <= 0.5)
  {
    int startAngle = 0;
    float radiusMultiplier = lunar.phaseDecimal * 4;
    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, startAngle, radiusMultiplier, ST7735_WHITE);
  }
  else
  {
    int startAngle = 180;
    float radiusMultiplier = 4 - (lunar.phaseDecimal * 4);
    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, startAngle, radiusMultiplier, ST7735_WHITE);
  };
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
  printSign(lunar);
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