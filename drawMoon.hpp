#include "TFT_22_ILI9225.h"

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
  static void drawMoonOutline(TFT_22_ILI9225 tft, DrawConstants drawConstants);

  static void drawLightBody(TFT_22_ILI9225 tft, DrawConstants drawConstants, float phaseDecimal);

  static void drawProjectionShape(TFT_22_ILI9225 tft, DrawConstants drawConstants, float phaseDecimal, uint16_t color);

  static void drawIlluminatedMoonOutline(TFT_22_ILI9225 tft, DrawConstants drawConstants, float illuminatedFraction);
};