#include "virtualScreen.h"
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>

#include "gameoflife.h"
#include "maze.h"
#include "playPong.h"
#include "tetris.h"
#include "font_test.h"
#include "image_test.h"
#include "nostromo.h"

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 18
#define TFT_DC 2
#define TFT_RST 4

VirtualDisplay *tft;

// Screen grid[ROWS][COLUMNS] = {
//     {{.row = 0, .column = 0, .cs = 7, .rotation = 2},
//      {.row = 0, .column = 1, .cs = 6, .rotation = 2},
//      {.row = 0, .column = 2, .cs = 15, .rotation = 2}},
//     {{.row = 1, .column = 0, .cs = 16, .rotation = 0},
//      {.row = 1, .column = 1, .cs = 11, .rotation = 0},
//      {.row = 1, .column = 2, .cs = 9, .rotation = 0}}};

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

    ScreenBuilder screens;

    // 4 sreens on one row
    // screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});

    // 6 screens 3 on each row
    screens.addRow({{7, 2}, {6, 2}, {15, 2}});
    screens.addRow({{16, 0}, {11, 0}, {9, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }
    Serial.printf("PSRAM Left = %lu\n", ESP.getFreePsram());

    // Demonstrations
    //

    // test_images();
    // playPong(tft);
    // gameOfLife(tft);
    // solveMaze(tft);
    // font_test(tft);
    // image_test(tft);
    playTetris(tft);
    // runNostromoSimulation(tft);
}

void loop()
{
}
