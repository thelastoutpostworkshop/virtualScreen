#include "virtualScreen.h"
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>

#include "images/newyork_960x240.h"
#include "images/highway_720x480.h"
#include "images/night_earth_960x240.h"
#include <Fonts/GFXFF/FreeSansBold24pt7b.h>
#include "fonts/Aurebesh_Bold32pt7b.h"
#include "fonts/Bombing40pt7b.h"

#include "gameoflife.h"
#include "maze.h"
#include "playPong.h"
#include "font_test.h"
#define test_image highway

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 18
#define TFT_DC 2
#define TFT_RST 4

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    if (psramFound())
    {
        Serial.printf("PSRAM Size = %lu\n", ESP.getPsramSize());
    }
    else
    {
        Serial.println("No PSRAM!");
        return;
    }
    Serial.printf("PSRAM Left = %lu\n", ESP.getFreePsram());

    ScreenBuilder screens;
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});
    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    // Demonstrations
    //

    // test_animation();
    // test_images();
    // playPong(tft);
    // gameOfLife(tft);
    // solveMaze(tft);
    font_test(tft);
}

void loop()
{
}

void test_images()
{
    tft->drawRGBBitmap(0, 0, (uint16_t *)newyork, newyork_width, newyork_height);
    tft->output();
    delay(5000);
    tft->drawRGBBitmap(0, 0, (uint16_t *)night_earth, night_earth_width, night_earth_height);
    tft->output();
}

uint16_t getRandomColor()
{
    // Generate random values for red (5 bits), green (6 bits), and blue (5 bits)
    uint8_t red = random(0, 32);   // 0 to 31
    uint8_t green = random(0, 64); // 0 to 63
    uint8_t blue = random(0, 32);  // 0 to 31

    // Combine them into one 16-bit color in RGB565 format
    return (red << 11) | (green << 5) | blue;
}

