#include "drawMoon.hpp"

void DrawMoon::drawMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants)
{
  tft.drawCircle(drawConstants.moonCenterX, drawConstants.moonCenterY, drawConstants.moonRadius, ST7735_WHITE);
};

void DrawMoon::drawLightBody(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal)
{
  float fullMoonOrb = 0.05;

  // full moon, Waxing crescent, waning crescent, fill moon
  if ((phaseDecimal < 0.25) || (phaseDecimal > 0.75 && phaseDecimal < 1) || (phaseDecimal > 0.5 - fullMoonOrb && phaseDecimal < 0.5 + fullMoonOrb))
  {

    tft.fillCircle(drawConstants.moonCenterX, drawConstants.moonCenterY, drawConstants.moonRadius, ST7735_WHITE);
    drawProjectionShape(tft, drawConstants, phaseDecimal, ST7735_BLACK);
  }
  else
  {
    drawProjectionShape(tft, drawConstants, phaseDecimal, ST7735_WHITE);
  };
};

void DrawMoon::drawProjectionShape(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal, uint16_t color)
{
  // no smaller than 1Radius
  // 1 Radius + 1 Radius * 8phaseDecimal
  int projectionRadius = max(drawConstants.moonRadius, drawConstants.moonRadius + (drawConstants.moonRadius * (phaseDecimal * 8)));
  int projectionCenterX = (drawConstants.moonCenterX - (projectionRadius) + drawConstants.moonRadius - (drawConstants.moonRadius * (phaseDecimal * 3.3)));
  // int projectionCenterX = (drawConstants.moonCenterX + (projectionRadius) + drawConstants.moonRadius + (drawConstants.moonRadius * (phaseDecimal * 3.3)));

  int projectionCenterY = drawConstants.moonCenterY;
  Serial.println("*** proj radius: ");
  Serial.println(projectionRadius);
  Serial.println(drawConstants.moonLeft);
  Serial.println(drawConstants.moonRadius);
  Serial.println(projectionCenterX);

  // tft.fillCircle(px, py, (projectionRadius / 2), ST7735_GREEN);

  // Increment faster to speed up draw time when projectionRadius is very large
  // ...until paintbrush touches moon (than increment normally)
  for (int rx = projectionCenterX >= 0 ? 0 : drawConstants.moonLeft; rx < projectionRadius; (projectionRadius > (drawConstants.moonRadius * 2) && rx < (projectionRadius - (drawConstants.moonRadius * 2))) ? rx += (projectionRadius / 4) : rx += 1)
  {
    int angleStart = projectionCenterX >= drawConstants.moonLeft ? 0 : -90;
    int angleEnd = projectionCenterX >= drawConstants.moonLeft ? 360 : 90;
    for (int i = angleStart; i < angleEnd; i += 1)
    {
      float radians = i * PI / 180;

      float px = (drawConstants.moonCenterX - (projectionRadius) + drawConstants.moonRadius - (drawConstants.moonRadius * (phaseDecimal * 3.4))) + rx * cos(radians);
      float py = drawConstants.moonCenterY + rx * sin(radians);

      tft.drawPixel(px, py, color);
    };
  }
};

void DrawMoon::drawIlluminatedMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants, float illuminatedFraction)
{
  // Thin outer circular layer
  // for (int rx = (moonRadius - (15 * illuminatedFraction)); rx < moonRadius; rx++)
  // {
  for (int i = (-90 * (illuminatedFraction * 4)); i < (90 * (illuminatedFraction * 4)); i++)
  {
    double radians = i * PI / 180;
    double px = drawConstants.moonCenterX + drawConstants.moonRadius * cos(radians);
    double py = drawConstants.moonCenterY + drawConstants.moonRadius * sin(radians);

    tft.drawPixel(px, py, ST7735_WHITE);
  };
  // };
};