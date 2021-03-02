#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

// http://www.barth-dev.de/online/rgb565-color-picker/
// rgb565

#define COLOR_DARK_SKY_BLUE 0x422A
#define MOON_BLACK 0x10A3

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
  static void clearScreen(Adafruit_ST7735 tft);

  static void drawMoonOutline(Adafruit_ST7735 tft, DrawConstants drawConstants, uint16_t color);

  static void drawMoonLight(Adafruit_ST7735 tft, DrawConstants drawConstants, float illuminatedFraction, int startAngle, float radiusMultiplier, uint16_t baseColor);

  static void fillArc2(Adafruit_ST7735 tft, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint16_t colour);

  static void drawSignAtPosition(Adafruit_ST7735 tft, DrawConstants drawConstants, float apparentLongitude);
};