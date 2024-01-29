#include "virtualScreen.h"
#include "images/newyork_960x240.h"
#include "images/highway_720x480.h"
#include "images/night_earth_960x240.h"

void image_test(VirtualDisplay *tft)
{
    tft->drawRGBBitmap(0, 0, (uint16_t *)newyork, newyork_width, newyork_height);
    tft->output();
    delay(5000);
    tft->drawRGBBitmap(0, 0, (uint16_t *)night_earth, night_earth_width, night_earth_height);
    tft->output();
}