#include "virtualScreen.h"
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>

#include "gameoflife.h"
#include "maze.h"
#include "playPong.h"
#include "tetris.h"
#include "font_test.h"
#include "image_test.h"

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
    // font_test(tft);
    // image_test(tft);
    playTetris(tft);
}

void loop()
{
}

