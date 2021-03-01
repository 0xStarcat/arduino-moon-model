// Sketch uses 31436 bytes.
// Global variables use 651 bytes of dynamic memory.

#include "ephemeris/Calendar.cpp"
#include "ephemeris/Ephemeris.cpp"
#include "drawMoon.hpp"
#include "time.hpp"
#include <RTClib.h>

// #include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735 screen
#include <Adafruit_DotStar.h> // lib for the dotstar LED
#include <SPI.h>

#define NUMPIXELS 30 // Number of LEDs in strip
#define DATAPIN 41
#define CLOCKPIN 40
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

// Devices

// #define TFT_CLK 13  // SCK A5 // 13
// #define TFT_POCIa 11 // SDI / SDA / A4 // 11
#define TFT_CS 18  // A4
#define TFT_RST 17 // A3 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 19  // A5
// #define TFT_PICO 0 // SDI / SDA / A4 // 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Run DEBUG mode when setting up a moonstone for the first time to set the time.
// Open serial monitor to ensure the moonstone works.
// once time is set, re-upload with DEBUG turned off.
#define DEBUG 0     // prints various debug messages. Sets the initial time.
#define CALC_TEST 0 // for testing the astronomy calculations

// Switch to using POTS control instead of actual calculations
#define POTS 0

// Define hours to convert local time to UTC time.
// Remember to account for daylight savings if in effect.
// EST = -5 so TIMEZONE_OFFSET = 5;
// PST = -8 so TIMEZONE_OFFET = 8;
// GMT = +1 so TIMEZONE_OFFSET = -1;
#define TIMEZONE_OFFSET -5 // ONLY NEEDS TO BE ACCURATE AT COMPILE TIME / LOCATION BASED ON COMPUTER CLOCK

#define UPDATE_SPEED 60 // > 1! (in seconds)

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
  // initialize digital pin 13 for LED light and turn it off
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // initialize dotstar and turn it off
  strip.begin();
  strip.show();
  Serial.begin(9600);

#if DEBUG
  while (!Serial)
  {
    ; // wait for serial port to connect.
  }

#endif

  if (rtc.begin())
  {
    Serial.println("RTC running.");
  }
  else
  {
    Serial.println("RTC not found.");
    Serial.flush();
    abort();
  }

  // deciding to set the time every upload.
  Serial.println("Checking RTC power...");
  if (rtc.lostPower())
  {
    Serial.println("RTC lost power.");
    setTime();
  }

#if DEBUG
  setTime(); // Set time on debug
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
#else
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
  Serial.println("Initializing.");
  tft.initR(INITR_GREENTAB); // Init ST7735R chip, green tab

  tft.setRotation(0);

  // SET GLOBAL VARIABLES after init because otherwise dimensions are bad
  Serial.println("Screen size detected: ");
  Serial.print("Width: ");
  Serial.println(tft.width());
  Serial.print("Height: ");
  Serial.println(tft.height());
  drawConstants.moonPadding = 2; // px
  drawConstants.moonCenterX = (tft.width() - drawConstants.moonPadding) / 2;
  drawConstants.moonCenterY = (tft.width() - drawConstants.moonPadding) / 2; // use width instead of height to keep square aspect ratio
  drawConstants.moonRadius = drawConstants.moonCenterX / 2;

  // Debug variables
  // Serial.println("Debug variables: ");
  // Serial.print("Moon X center: ");
  // Serial.println(drawConstants.moonCenterX);
  // Serial.print("Moon Y center: ");
  // Serial.println(drawConstants.moonCenterY);
  // Serial.print("Moon radius: ");
  // Serial.println(drawConstants.moonRadius);

  DrawMoon::clearScreen(tft);

#if CALC_TEST
  calculationTest(testHour, testDay);
#endif

#if !CALC_TEST
  drawRealTimeData();
#endif
};

void drawRealTimeData()
{

  DateTime tm = rtc.now();
#if DEBUG
  printTime(tm);
#endif

#if POTS
  LunarPhaseMeasures lunar;
  // Debug with potentiometers
  int sensorIlluminationValue = analogRead(A0);
  float outputIlluminationValue = map(sensorIlluminationValue, 0, 1023, 0, 1000);
  outputIlluminationValue = (outputIlluminationValue / 1000) - 0.05;
  lunar.illuminatedFraction = outputIlluminationValue;

  // For A1 Choose either phaseValue (moon phase) or apparent longitude (zodiac sign) to alter

  // int sensorPhaseValue = analogRead(A1);
  // float outputPhaseValue = map(sensorPhaseValue, 0, 1023, 0, 1000);
  // outputPhaseValue = (outputPhaseValue / 1000) - 0.05;
  // lunar.phaseDecimal = outputPhaseValue;

  int sensorApparentLongitude = analogRead(A1);
  float outputLongitudeValue = map(sensorApparentLongitude, 0, 1023, 0, 360);
  lunar.apparentLongitude = outputLongitudeValue;

#else
  LunarPhaseMeasures lunar = getLunar(tm);
#endif

#if DEBUG
  printLunarMeasures(lunar);
#endif

  DrawMoon::clearScreen(tft);
  DrawMoon::drawMoonOutline(tft, drawConstants, MOON_BLACK);
  drawHoroscopeSign(lunar, drawConstants);
  drawMoonToMeasurements(tm, lunar);
}

LunarPhaseMeasures getLunar(DateTime tm)
{

  return Ephemeris::getLunarPhaseMeasures(tm.day(), tm.month(), tm.year(), tm.hour(), tm.minute(), 0);
};

void drawMoonToMeasurements(DateTime tm, LunarPhaseMeasures lunar)
{

  // float phaseMult = lunar.phaseDecimal * 4; // fill based on phaseDecimal
  float phaseMult = lunar.illuminatedFraction * 2; // fill based on illuminatedFraction
  float radiusMultiplier = phaseMult;
  int angleModier = 0; // the offset to make the moon slanted
  if (lunar.phaseDecimal <= 0.5)
  {
    int startAngle = 0; // + 30 // slight axis tilt to the right
    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, startAngle + angleModier, radiusMultiplier, ST7735_WHITE);
  }
  else
  {
    int startAngle = 180; // + 30 // slight axis tilt to the right
    // radiusMultiplier = 4 - phaseMult; // fill based on phaseDecimal
    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, startAngle + angleModier, radiusMultiplier, ST7735_WHITE);
  };
};

void drawHoroscopeSign(LunarPhaseMeasures lunar, DrawConstants drawConstants)
{
  DrawMoon::drawSignAtPosition(tft, drawConstants, lunar.apparentLongitude);
};

void printLunarMeasures(LunarPhaseMeasures lunar)
{
  Serial.print("Illumuniated fraction: ");
  Serial.println(lunar.illuminatedFraction, 4);
  Serial.print("Phase decimal: ");
  Serial.println(lunar.phaseDecimal, 4);
  Serial.print("Apparent longitude: ");
  Serial.println(lunar.apparentLongitude, 4);
};

void setTime()
{
  // following line sets the RTC to the date & time this sketch was compiled
  Serial.println("Setting the time");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime utcTime = TimeWrapper::setTimeOffset(rtc.now(), TIMEZONE_OFFSET);
  rtc.adjust(utcTime);
  printTime(utcTime);
}

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
