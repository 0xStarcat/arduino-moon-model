#include "drawMoon.hpp"
#define DEG2RAD 0.0174532925

void DrawMoon::drawMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants, uint16_t color)
{
  tft.fillCircle(drawConstants.moonCenterX, drawConstants.moonCenterY, drawConstants.moonRadius, color);
};

// radiusMultiplier = float from 0 - 2 to represent how far to draw the crescent / gibbous out. 2 radius = full moon
// startAngle = int from 0 - 359. the side to start drawing from. 0 = right side, 180 = left side.
void DrawMoon::drawMoonLight(Adafruit_ST7735 tft, DrawConstants drawConstants, float illuminatedFraction, int startAngle, float radiusMultiplier, uint16_t baseColor)
{

  int fillAmount = drawConstants.moonRadius * radiusMultiplier; // 0 - 2R

  for (int i = drawConstants.moonRadius * 2; i >= 0; i--)
  {

    // white to gray gradient
    /////////////////////////
    // int rgbHigh = 255;
    // int rgbLow = min(255, (255 * illuminatedFraction) + 15);
    // float fillPercent = ((float)i / fillAmount);
    // int rgb = 255 - (fillPercent * (rgbHigh - rgbLow));

    // uint16_t color = (((31 * (rgb + 4)) / 255) << 11) |
    //                  (((63 * (rgb + 2)) / 255) << 5) |
    //                  ((31 * (rgb + 4)) / 255);
    uint16_t color = 0;
    if (i <= fillAmount)
    {
      // fill moon light
      color = baseColor;
    }
    else
    {
      // Fill remainder black
      color = ST7735_BLACK;
    }

    fillArc2(tft, drawConstants.moonCenterX, drawConstants.moonCenterY, startAngle, 60, (drawConstants.moonRadius - i), drawConstants.moonRadius, 1, color);
  }
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

void DrawMoon::fillArc2(Adafruit_ST7735 tft, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint16_t color)
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
};

// Memory concerns:
// Draw glyphs in < 10 shapes
// Do not use drawPixel

void drawM(Adafruit_ST7735 tft, int x, int y, uint16_t color)
{
  tft.drawLine(x + 4, y + 3, x + 4, y + 10, color);
  tft.drawLine(x + 4, y + 2, x + 5, y + 2, color);
  tft.drawLine(x + 7, y + 3, x + 7, y + 10, color);
  tft.drawLine(x + 8, y + 2, x + 9, y + 2, color);
  tft.drawLine(x + 10, y + 3, x + 10, y + 12, color);
};

void drawAries(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_RED;
  // tft.drawLine(x + 2, y + 3, x + 2, y + 5, color);
  // tft.drawLine(x + 3, y + 2, x + 5, y + 2, color);
  // tft.drawLine(x + 6, y + 3, x + 6, y + 4, color);

  // tft.drawLine(x + 9, y + 3, x + 9, y + 4, color);
  // tft.drawLine(x + 10, y + 2, x + 12, y + 2, color);
  // tft.drawLine(x + 13, y + 3, x + 13, y + 5, color);
  // tft.drawRect(x + 7, y + 6, 2, 7, color);

  tft.drawLine(x + 4, y + 2, x + 2, y + 5, color);
  tft.drawLine(x + 6, y + 5, x + 4, y + 2, color);

  tft.drawLine(x + 7, y + 6, x + 7, y + 12, color);

  tft.drawLine(x + 8, y + 5, x + 10, y + 2, color);
  tft.drawLine(x + 10, y + 2, x + 12, y + 4, color);
};

void drawTaurus(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_GREEN;
  tft.drawCircle(x + 7, y + 8, 3, color);
  tft.drawLine(x + 3, y + 3, x + 5, y + 5, color);

  // tft.drawLine(x + 3, y + 3, x + 4, y + 3, color);
  // tft.drawLine(x + 4, y + 4, x + 5, y + 4, color);
  // tft.drawLine(x + 5, y + 5, x + 6, y + 5, color);

  tft.drawLine(x + 9, y + 5, x + 11, y + 3, color);
  // tft.drawLine(x + 11, y + 3, x + 12, y + 3, color);
  // tft.drawLine(x + 10, y + 4, x + 11, y + 4, color);
  // tft.drawLine(x + 9, y + 5, x + 10, y + 5, color);
};

void drawGemini(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_YELLOW;
  tft.drawLine(x + 4, y + 4, x + 11, y + 4, color);
  tft.drawLine(x + 4, y + 12, x + 11, y + 12, color);
  tft.drawLine(x + 6, y + 4, x + 6, y + 12, color);
  tft.drawLine(x + 9, y + 4, x + 9, y + 12, color);
};

void drawCancer(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_BLUE;
  tft.drawCircle(x + 4, y + 5, 3, color);
  tft.drawCircle(x + 10, y + 10, 3, color);
  tft.drawLine(x + 4, y + 2, x + 12, y + 2, color);
  tft.drawLine(x + 4, y + 13, x + 12, y + 13, color);
};

void drawLeo(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_RED;
  tft.drawCircle(x + 5, y + 7, 3, color);
  // tft.drawLine(x + 5, y + 4, x + 5, y + 2, color);
  // tft.drawLine(x + 6, y + 2, x + 9, y + 2, color);
  tft.drawLine(x + 5, y + 4, x + 10, y + 3, color);

  tft.drawLine(x + 10, y + 3, x + 10, y + 12, color);
  tft.drawLine(x + 10, y + 12, x + 12, y + 12, color);
};

void drawVirgo(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_GREEN;
  drawM(tft, x, y, color);
  tft.drawLine(x + 11, y + 4, x + 13, y + 4, color);
  tft.drawLine(x + 13, y + 4, x + 13, y + 7, color);
  tft.drawLine(x + 13, y + 7, x + 6, y + 13, color);
};

void drawLibra(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_YELLOW;
  tft.drawCircle(x + 7, y + 5, 2, color);
  tft.drawLine(x + 2, y + 8, x + 13, y + 8, color);
  tft.drawLine(x + 2, y + 11, x + 13, y + 11, color);
};

void drawScorpio(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_BLUE;
  drawM(tft, x, y, color);
  tft.drawLine(x + 11, y + 12, x + 13, y + 12, color);
};

void drawSagittarius(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_RED;
  tft.drawLine(x + 3, y + 12, x + 12, y + 3, color);
  // tft.drawTriangle(x + 8, y + 3, x + 12, y + 3, x + 12, y + 7, color);
  tft.drawLine(x + 8, y + 3, x + 12, y + 3, color);
  tft.drawLine(x + 12, y + 3, x + 12, y + 7, color);
  tft.drawLine(x + 4, y + 7, x + 8, y + 11, color);
};

void drawCapricorn(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_GREEN;
  tft.drawLine(x + 2, y + 2, x + 5, y + 10, color);
  tft.drawLine(x + 5, y + 10, x + 8, y + 2, color);
  tft.drawLine(x + 8, y + 2, x + 10, y + 10, color);
  tft.drawCircle(x + 12, y + 10, 2, color);
  tft.drawLine(x + 5, y + 13, x + 10, y + 13, color);
};

void drawAquarius(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_YELLOW;
  for (int i = 0; i < 5; i += 4)
  {
    tft.drawLine(x + 1, y + 6 + i, x + 4, y + 4 + i, color);
    tft.drawLine(x + 4, y + 4 + i, x + 5, y + 6 + i, color);
    tft.drawLine(x + 5, y + 6 + i, x + 8, y + 4 + i, color);
    tft.drawLine(x + 8, y + 4 + i, x + 9, y + 6 + i, color);
    tft.drawLine(x + 9, y + 6 + i, x + 12, y + 4 + i, color);
    tft.drawLine(x + 12, y + 4 + i, x + 13, y + 6 + i, color);
  }
};

void drawPisces(Adafruit_ST7735 tft, int x, int y)
{
  uint16_t color = ST7735_BLUE;
  tft.drawLine(x + 5, y + 4, x + 3, y + 2, color);
  tft.drawLine(x + 5, y + 4, x + 5, y + 10, color);
  tft.drawLine(x + 5, y + 10, x + 3, y + 13, color);

  tft.drawLine(x + 9, y + 4, x + 11, y + 2, color);
  tft.drawLine(x + 9, y + 4, x + 9, y + 10, color);
  tft.drawLine(x + 9, y + 10, x + 11, y + 13, color);

  tft.drawLine(x + 4, y + 7, x + 10, y + 7, color);
};

void drawSign(Adafruit_ST7735 tft, int x, int y, int i)
{
  switch (i)
  {
  case 0:
    return drawAries(tft, x, y);
  case 1:
    return drawTaurus(tft, x, y);
  case 2:
    return drawGemini(tft, x, y);
  case 3:
    return drawCancer(tft, x, y);
  case 4:
    return drawLeo(tft, x, y);
  case 5:
    return drawVirgo(tft, x, y);
  case 6:
    return drawLibra(tft, x, y);
  case 7:
    return drawScorpio(tft, x, y);
  case 8:
    return drawSagittarius(tft, x, y);
  case 9:
    return drawCapricorn(tft, x, y);
  case 10:
    return drawAquarius(tft, x, y);
  case 11:
    return drawPisces(tft, x, y);
  };
};

double getRadialX(DrawConstants drawConstants, double radians, float padding)
{
  return drawConstants.moonCenterX + (drawConstants.moonRadius + padding) * cos(radians);
};

double getRadialY(DrawConstants drawConstants, double radians, float padding)
{
  return drawConstants.moonCenterY + (drawConstants.moonRadius + padding) * sin(radians);
};

void clearZodiacArea(Adafruit_ST7735 tft, DrawConstants drawConstants)
{
  // Clear zodiac area w/ 4 rectangles
  int horizontalOffset = drawConstants.moonCenterX - drawConstants.moonRadius;
  int verticalOffset = drawConstants.moonCenterY - drawConstants.moonRadius;
  // Top left right
  tft.fillRect(0, 0, tft.width(), verticalOffset, COLOR_DARK_SKY_BLUE);
  // top left - down
  tft.fillRect(0, 0, horizontalOffset, tft.height(), COLOR_DARK_SKY_BLUE);
  // top right - down
  tft.fillRect(tft.width() - horizontalOffset, 0, horizontalOffset, tft.height(), COLOR_DARK_SKY_BLUE);
  // bottom left - right
  tft.fillRect(0, tft.height() - verticalOffset, tft.width(), verticalOffset, COLOR_DARK_SKY_BLUE);
}

void DrawMoon::drawSignAtPosition(Adafruit_ST7735 tft, DrawConstants drawConstants, float apparentLongitude)
{

  int iStart = 3; // offset for zodiac (Aries begins on top - cancer on horizon)
  int zodiacPadding = 22;
  int signPadding = 12;
  int markerOffset = signPadding / 2;

  clearZodiacArea(tft, drawConstants);

  // Draw boundary markers
  for (int i = iStart; i < iStart + 12; i++)
  {

    double boundaryRadians = -((((i * 30) - 15) * PI / 180)); // start of 30 degree zodiac segment
    double x = getRadialX(drawConstants, boundaryRadians, zodiacPadding);
    double y = getRadialY(drawConstants, boundaryRadians, zodiacPadding);

    // tft.drawRect(px - markerOffset, py - markerOffset, 16, 16, ST7735_RED);
    // drawSign(tft, px - markerOffset, py - markerOffset, i - iStart);

    // Draws zodiac boundaries
    tft.drawLine(x, y, x, y, ST7735_WHITE);
  };

  // Draw horoscope sign and position
  for (int i = iStart; i < iStart + 12; i++)
  {
    int index = floor(apparentLongitude / 30);
    // Sign X/Y
    double radians = -((i * 30) * PI / 180); // start at left of circle, position in betwen divisions
    double px = getRadialX(drawConstants, radians, signPadding);
    double py = getRadialY(drawConstants, radians, signPadding);

    // Draws longitude position marker
    int apLon = (int)apparentLongitude;
    double positionRadians = -(((((i * 30) - 15) + apLon % 30) * PI / 180)); // set progress position within 30 degree secment
    double ppx = getRadialX(drawConstants, positionRadians, zodiacPadding);
    double ppy = getRadialY(drawConstants, positionRadians, zodiacPadding);
    if (index == i - iStart)
    {
      // Draws glyph
      drawSign(tft, px - markerOffset, py - markerOffset, index);
      tft.drawCircle(ppx, ppy, 1, ST7735_MAGENTA);
    };
  };
};
