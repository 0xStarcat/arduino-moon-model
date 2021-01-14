// Sketch uses 31436 bytes.
// Global variables use 651 bytes of dynamic memory.

#include "ephemeris/Calendar.cpp"
#include "ephemeris/Ephemeris.cpp"
#include "drawMoon.hpp"
#include "time.hpp"
#include <RTClib.h>

// #include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

// Devices

// #define TFT_CLK 13  // SCK A5 // 13
// #define TFT_POCI 11 // SDI / SDA / A4 // 11
#define TFT_CS 10 // TCS
#define TFT_RST 9
#define TFT_DC 8 // RS
// #define TFT_PICO 0 // SDI / SDA / A4 // 11

// To reduce compile size & flash memory, only set 1 mode at a time.
#define SET_UTC_TIME 1 // set the initial UTC time (only needs to be run once)
#define DEBUG 1        // prints various debug messages
#define CALC_TEST 0    // for testing the astronomy calculations

// Switch to using POTS control instead of actual calculations
#define POTS 0

// Define hours to convert local time to UTC time.
// Remember to account for daylight savings if in effect.
// EST = -5 so TIMEZONE_OFFSET = 5;
// PST = -8 so TIMEZONE_OFFET = 8;
// GMT = +1 so TIMEZONE_OFFSET = -1;
#define TIMEZONE_OFFSET 0

#define UPDATE_SPEED 30 // > 1!

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

DrawConstants drawConstants;

RTC_DS3231 rtc;

uint8_t secondCounter = 0; // counts seconds until update calc/paint
uint8_t lastSecond = 0;

#if CALC_TEST
uint16_t testDay = 0;
uint16_t testHour = 0;
#endif

void setup()
{
  Serial.begin(9600);

  while (!Serial)
  {
    ; // wait for serial port to connect.
  }

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
#if SET_UTC_TIME
  // if (rtc.lostPower())
  // {
  //   Serial.println("RTC lost power, lets set the time!");
  //   // following line sets the RTC to the date & time this sketch was compiled
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //   DateTime utcTime = TimeWrapper::setTimeOffset(rtc.now(), TIMEZONE_OFFSET);
  //   rtc.adjust(utcTime);
  // }

  // following line sets the RTC to the date & time this sketch was compiled
  Serial.println("Setting Time.");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime utcTime = TimeWrapper::setTimeOffset(rtc.now(), TIMEZONE_OFFSET);
  rtc.adjust(utcTime);
  printTime(utcTime);
#endif

  initialize();
};

void loop()
{
  DateTime tm = rtc.now();
  int currentSecond = tm.second();
  if (secondCounter >= UPDATE_SPEED)
  {
    // reset secondCounter
    secondCounter = 1;

#if CALC_TEST
    calculationTest(testHour, testDay);
#endif

#if !CALC_TEST && !SET_UTC_TIME
    drawRealTimeData();
#endif

#if CALC_TEST
    testHour += 1;
    if (testHour >= 24)
    {
      testHour = 0;
      testDay += 1;
    }
#endif
  }
  // only increment secondCounter when click increments 1 second
  else if (currentSecond != lastSecond)
  {
    secondCounter += 1;
    lastSecond = currentSecond;
  }
};

void initialize()
{
#if DEBUG
  Serial.println("Init...");
#endif
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  // tft.setRotation(0);

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

  tft.fillScreen(COLOR_DARK_SKY_BLUE);

#if CALC_TEST
  calculationTest(testHour, testDay);
#endif

#if !CALC_TEST && !SET_UTC_TIME
  drawRealTimeData();
#endif
};

void drawRealTimeData()
{

  DateTime tm = rtc.now();
#if DEBUG || SET_UTC_TIME
  printTime(tm);
#endif

#if !POTS
  LunarPhaseMeasures lunar = getLunar(tm);
#else
  // Debug with potentiometers
  int sensorIlluminationValue = analogRead(A0);
  float outputIlluminationValue = map(sensorIlluminationValue, 0, 1023, 0, 1000);
  outputIlluminationValue = (outputIlluminationValue / 1000) - 0.05;

  // int sensorPhaseValue = analogRead(A1);
  // float outputPhaseValue = map(sensorPhaseValue, 0, 1023, 0, 1000);
  // outputPhaseValue = (outputPhaseValue / 1000) - 0.05;

  int sensorApparentLongitude = analogRead(A1);
  float outputLongitudeValue = map(sensorApparentLongitude, 0, 1023, 0, 360);

  LunarPhaseMeasures lunar;
  lunar.apparentLongitude = outputLongitudeValue;
  lunar.illuminatedFraction = outputIlluminationValue;
// lunar.phaseDecimal = outputPhaseValue;
#endif

#if DEBUG
  printLunarMeasures(lunar);
#endif

  drawHoroscopeSign(lunar, drawConstants);
  drawMoonToMeasurements(tm, lunar);
}

LunarPhaseMeasures getLunar(DateTime tm)
{

  return Ephemeris::getLunarPhaseMeasures(tm.day(), tm.month(), tm.year(), tm.hour(), tm.minute(), 0);
};

void drawMoonToMeasurements(DateTime tm, LunarPhaseMeasures lunar)
{

  // DrawMoon::drawMoonOutline(tft, drawConstants, ST7735_BLACK);

  // float phaseMult = lunar.phaseDecimal * 4; // fill based on phaseDecimal
  float phaseMult = lunar.illuminatedFraction * 2; // fill based on illuminatedFraction
  float radiusMultiplier = phaseMult;
  if (lunar.phaseDecimal <= 0.5)
  {

    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, 0, radiusMultiplier, ST7735_WHITE);
  }
  else
  {
    // radiusMultiplier = 4 - phaseMult; // fill based on phaseDecimal
    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, 180, radiusMultiplier, ST7735_WHITE);
  };
};

void drawHoroscopeSign(LunarPhaseMeasures lunar, DrawConstants drawConstants)
{
  DrawMoon::drawSignAtPosition(tft, drawConstants, lunar.apparentLongitude);
};

void printLunarMeasures(LunarPhaseMeasures lunar)
{
  Serial.print("I: ");
  Serial.println(lunar.illuminatedFraction, 4);
  Serial.print("P: ");
  Serial.println(lunar.phaseDecimal, 4);
  Serial.print("A: ");
  Serial.println(lunar.apparentLongitude, 4);
};

void printTime(DateTime tm)
{
  Serial.print("*Time: ");
  Serial.print(tm.month());
  Serial.print("/");
  Serial.print(tm.day());
  Serial.print("/");
  Serial.print(tm.year());
  Serial.print(" - ");
  print2digits(tm.hour());
  Serial.print(":");
  print2digits(tm.minute());
  Serial.print(":");
  print2digits(tm.second());
  Serial.println();
};

void print2digits(int number)
{
  if (number >= 0 && number < 10)
  {
    Serial.write('0');
  }
  Serial.print(number);
};

void calculationTest(uint8_t hour, uint16_t day)
{
  DateTime tm = DateTime(2020, 1, 1, 5, 0, 0);
  DateTime tmOffset = TimeWrapper::setTimeOffset(tm, hour, day);

  LunarPhaseMeasures lunar = getLunar(tm);
  // printTime(tmOffset);
  printLunarMeasures(lunar);
  // drawHoroscopeSign(lunar, drawConstants);
  // drawMoonToMeasurements(tmOffset, lunar);
}