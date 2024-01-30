#include "virtualScreen.h"
#include "images/newyork_960x240.h"
#include "images/highway_720x480.h"
#include "images/night_earth_960x240.h"
#include "images/bazar_960x240.h"
#include "images/spacebattle_960x240.h"
#include "images/console_960x240.h"

// #define test_image bazar
// #define test_width bazar_width
// #define test_height bazar_height

// #define test_image spacebattle
// #define test_width spacebattle_width
// #define test_height spacebattle_height

#define test_image console
#define test_width console_width
#define test_height console_height

void image_test(VirtualDisplay *tft)
{
    tft->fillScreen(TFT_BLACK);
    tft->drawRGBBitmap(0, 0, (uint16_t *)test_image, test_width, test_height);
    tft->output();
}