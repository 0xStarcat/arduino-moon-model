#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

struct DrawConstants
{
  uint16_t moonPadding; // px
  uint16_t moonCenterX; // px
  uint16_t moonCenterY; // px
  uint16_t moonRadius;  // px
};

class DrawMoon
{
public:
  static void drawMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants, uint16_t color);

  // static void drawLightBody(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal);

  // static void drawProjectionShape(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal, uint16_t color);

  // static void drawIlluminatedMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants, float illuminatedFraction);

  static void drawMoonLight(Adafruit_ST7735 tft, DrawConstants drawConstants, float illuminatedFraction, int startAngle, float radiusMultiplier, uint16_t baseColor);

  static void fillArc2(Adafruit_ST7735 tft, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);
};