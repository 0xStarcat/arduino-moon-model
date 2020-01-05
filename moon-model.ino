// Sketch uses 19322 bytes.
// Global variables use 833 bytes of dynamic memory.

#include "ephemeris/Calendar.cpp"
#include "ephemeris/Ephemeris.cpp"
#include "utilities.hpp"
#include "drawMoon.hpp"
#include <RTClib.h>

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

DrawConstants drawConstants;

RTC_DS3231 rtc;

const uint8_t UPDATE_SPEED = 10; // seconds
uint8_t secondCounter = 0;       // counts seconds until update calc/paint
uint8_t lastSecond = 0;
void setup()
{
  Serial.begin(9600);
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  // if (rtc.lostPower())
  // {
  //   Serial.println("RTC lost power, lets set the time!");
  //   // following line sets the RTC to the date & time this sketch was compiled
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //   // This line sets the RTC with an explicit date & time, for example to set
  //   // January 21, 2014 at 3am you would call:
  //   // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  // }

  initialize();
};

void loop()
{
  DateTime tm = rtc.now();

  if (secondCounter >= UPDATE_SPEED)
  {
    // reset secondCounter
    secondCounter = 1;
    Serial.print("*** ");
    printTime(tm);
    LunarPhaseMeasures lunar = Ephemeris::getLunarPhaseMeasures(tm.day(), tm.month(), tm.year(), tm.hour(), tm.minute(), 0);

    drawMoonToMeasurements(tm, lunar);
    drawHoroscopeSign(lunar, drawConstants);
  }
  // only increment secondCounter when click increments 1 second
  else if (tm.second() != lastSecond)
  {
    secondCounter += 1;
    lastSecond = tm.second();
  }
};

void initialize()
{
  Serial.println("Initializing...");
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  // tft.setRotation(0);

  // SET GLOBAL VARIABLES after init because otherwise dimensions are bad
  // drawConstants.moonPadding = 2; // px
  drawConstants.moonCenterX = (tft.width() - 2) / 2;
  drawConstants.moonCenterY = (tft.height() - 2) / 2;
  drawConstants.moonRadius = drawConstants.moonCenterX / 2;

  // Debug variables
  // Serial.println("Debug variables: ");
  // Serial.print("Moon X center: ");
  // Serial.println(drawConstants.moonCenterX);
  // Serial.print("Moon Y center: ");
  // Serial.println(drawConstants.moonCenterY);
  // Serial.print("Moon radius: ");
  // Serial.println(drawConstants.moonRadius);

  // Init first draw
  DateTime tm = rtc.now();
  printTime(tm);

  LunarPhaseMeasures lunar = Ephemeris::getLunarPhaseMeasures(tm.day(), tm.month(), tm.year(), tm.hour(), tm.minute(), 0);

  drawMoonToMeasurements(tm, lunar);
  drawHoroscopeSign(lunar, drawConstants);
};

void drawMoonToMeasurements(DateTime tm, LunarPhaseMeasures lunar)
{

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
  float phaseMult = lunar.phaseDecimal * 4;

  if (lunar.phaseDecimal <= 0.5)
  {

    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, 0, phaseMult, ST7735_WHITE);
  }
  else
  {

    DrawMoon::drawMoonLight(tft, drawConstants, lunar.illuminatedFraction, 180, 4 - (phaseMult), ST7735_WHITE);
  };

  DrawMoon::drawMoonOutline(tft, drawConstants, ST7735_WHITE);
};

void drawHoroscopeSign(LunarPhaseMeasures lunar, DrawConstants drawConstants)
{
  DrawMoon::drawSignAtPosition(tft, drawConstants, lunar.apparentLongitude);
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
  Serial.print("IF: ");
  Serial.println(lunar.illuminatedFraction, 4);
  Serial.print("PD: ");
  Serial.println(lunar.phaseDecimal, 4);
  Serial.print("AL: ");
  Serial.println(lunar.apparentLongitude, 4);
};

void printTime(DateTime tm)
{
  Serial.print(tm.month());
  Serial.print("/");
  Serial.print(tm.day());
  Serial.print("/");
  Serial.print(tm.year());
  Serial.print(" -- ");
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