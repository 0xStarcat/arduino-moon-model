#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#define COLOR_DARK_SKY_BLUE 258

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

  static void drawMoonLight(Adafruit_ST7735 tft, DrawConstants drawConstants, float illuminatedFraction, int startAngle, float radiusMultiplier, uint16_t baseColor);

  static void fillArc2(Adafruit_ST7735 tft, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint16_t colour);

  static void drawSignAtPosition(Adafruit_ST7735 tft, DrawConstants drawConstants, float apparentLongitude);
};