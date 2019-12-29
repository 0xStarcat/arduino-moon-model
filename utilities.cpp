#include "utilities.hpp"

char *Utilities::getSignByIndex(int i)
{
  switch (i)
  {
  case 0:
    return "Aries";
    break;
  case 1:
    return "Taurus";
    break;
  case 2:
    return "Gemini";
    break;
  case 3:
    return "Cancer";
    break;
  case 4:
    return "Leo";
    break;
  case 5:
    return "Virgo";
    break;
  case 6:
    return "Libra";
    break;
  case 7:
    return "Scorpio";
    break;
  case 8:
    return "Sagittarius";
    break;
  case 9:
    return "Capricorn";
    break;
  case 10:
    return "Aquarius";
    break;
  case 11:
    return "Pisces";
    break;
  default:
    return "no sign found";
  }
};
