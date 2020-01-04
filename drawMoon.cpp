#include "drawMoon.hpp"
#define DEG2RAD 0.0174532925

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

void DrawMoon::drawWaxingMoon(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal)
{
  int x = drawConstants.moonCenterX;
  int y = drawConstants.moonCenterY;
  int xRadius = drawConstants.moonRadius;
  int yRadius = drawConstants.moonRadius;
  int width = 1;
  int startAngle = 0;
  int segments = 60;                         // half moon
  float radiusMultiplier = phaseDecimal * 4; // phaseDecimal at 0 - 0.5

  Serial.println("Multi: ");
  Serial.println(radiusMultiplier);

  for (int i = 0; i < (drawConstants.moonRadius * radiusMultiplier); i++)
  {

    fillArc2(tft, x, y, startAngle, segments, (xRadius - i), (yRadius), width, ST7735_WHITE);
  }
}

void DrawMoon::drawWaningMoon(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal)
{
  int x = drawConstants.moonCenterX;
  int y = drawConstants.moonCenterY;
  int xRadius = drawConstants.moonRadius;
  int yRadius = drawConstants.moonRadius;
  int width = 1;
  int startAngle = 180;
  int segments = 60;                               // half moon
  float radiusMultiplier = 4 - (phaseDecimal * 4); // phaseDecimal at 0.5 - 1

  Serial.println("Multi: ");
  Serial.println(radiusMultiplier);

  for (int i = 0; i < (drawConstants.moonRadius * radiusMultiplier); i++)
  {

    fillArc2(tft, x, y, startAngle, segments, (xRadius - i), (yRadius), width, ST7735_WHITE);
  }

  // fillArc(160, 100, 300, 40, rx - n * w, ry - n * w, w, 31 - n * 6);
}

// #########################################################################
// Draw a circular or elliptical arc with a defined thickness
// #########################################################################

//https://forum.arduino.cc/index.php?topic=413186.0
// x,y == coords of centre of arc
// start_angle = 0 - 359
// seg_count = number of 3 degree segments to draw (120 => 360 degree arc)
// rx = x axis radius
// yx = y axis radius
// w  = width (thickness) of arc in pixels
// color = 16 bit color value
// Note if rx and ry are the same then an arc of a circle is drawn

void DrawMoon::fillArc2(Adafruit_ST7735 tft, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int color)
{

  byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  // Draw color blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc)
  {

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
    tft.fillTriangle(x1, y1, x2, y2, x3, y3, color);

    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
}
