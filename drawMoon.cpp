#include "drawMoon.hpp"

void DrawMoon::drawMoonOutline(TFT_22_ILI9225 tft, DrawConstants drawConstants)
{
  tft.drawCircle(drawConstants.moonCenterX, drawConstants.moonCenterY, drawConstants.moonRadius, COLOR_WHITE);
};

void DrawMoon::drawLightBody(TFT_22_ILI9225 tft, DrawConstants drawConstants, float phaseDecimal)
{
  float fullMoonOrb = 0.05;

  // full moon, Waxing crescent, waning crescent, fill moon
  if ((phaseDecimal < 0.25) || (phaseDecimal > 0.75 && phaseDecimal < 1) || (phaseDecimal > 0.5 - fullMoonOrb && phaseDecimal < 0.5 + fullMoonOrb))
  {

    tft.fillCircle(drawConstants.moonCenterX, drawConstants.moonCenterY, drawConstants.moonRadius, COLOR_WHITE);
    drawProjectionShape(tft, drawConstants, phaseDecimal, COLOR_BLACK);
  }
  else
  {
    drawProjectionShape(tft, drawConstants, phaseDecimal, COLOR_WHITE);
  };
};

void DrawMoon::drawProjectionShape(TFT_22_ILI9225 tft, DrawConstants drawConstants, float phaseDecimal, uint16_t color)
{
  // no smaller than 1Radius
  // 1 Radius + 1 Radius * 8phaseDecimal
  float projectionSize = max(drawConstants.moonRadius, drawConstants.moonRadius + (drawConstants.moonRadius * (phaseDecimal * 8)));
  Serial.println("*** proj size: ");
  Serial.println(projectionSize);

  // Increment faster to speed up draw time when projectionSize is very large
  // ...until paintbrush touches moon (than increment normally)
  for (int rx = 0; rx < projectionSize; (projectionSize > (drawConstants.moonRadius * 2) && rx < (projectionSize - (drawConstants.moonRadius * 2))) ? rx += (projectionSize / 4) : rx += 1)
  {
    for (int i = 0; i < 360; i++)
    {
      double radians = i * PI / 180;

      double px = (drawConstants.moonCenterX - (projectionSize) + drawConstants.moonRadius - (drawConstants.moonRadius * (phaseDecimal * 3.3))) + rx * cos(radians);
      double py = drawConstants.moonCenterY + rx * sin(radians);

      tft.drawPixel(px, py, COLOR_BLACK);
    };
  }
};

void DrawMoon::drawIlluminatedMoonOutline(TFT_22_ILI9225 tft, DrawConstants drawConstants, float illuminatedFraction)
{
  // Thin outer circular layer
  // for (int rx = (moonRadius - (15 * illuminatedFraction)); rx < moonRadius; rx++)
  // {
  for (int i = (-90 * (illuminatedFraction * 4)); i < (90 * (illuminatedFraction * 4)); i++)
  {
    double radians = i * PI / 180;
    double px = drawConstants.moonCenterX + drawConstants.moonRadius * cos(radians);
    double py = drawConstants.moonCenterY + drawConstants.moonRadius * sin(radians);

    tft.drawPixel(px, py, COLOR_WHITE);
  };
  // };
};