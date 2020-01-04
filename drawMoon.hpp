#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

struct DrawConstants
{
  uint16_t moonPadding; // px
  uint16_t moonCenterX; // px
  uint16_t moonCenterY; // px
  uint16_t moonRadius;  // px
  uint16_t moonLeft;    // px
  uint16_t moonRight;   // px
  uint16_t moonTop;     // px
  uint16_t moonBottom;  // px
};

class DrawMoon
{
public:
  static void drawMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants);

  static void drawLightBody(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal);

  static void drawProjectionShape(Adafruit_ST7735 tft, DrawConstants drawConstants, float phaseDecimal, uint16_t color);

  static void drawIlluminatedMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants, float illuminatedFraction);
};