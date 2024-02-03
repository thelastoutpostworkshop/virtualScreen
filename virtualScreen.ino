#include "virtualScreen.h"
#include "images/clock_1.h"

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    ScreenBuilder screens;
    // Adjust this setup according to your actual screen configuration
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    tft->fillScreen(TFT_BLACK);
    tft->pushImage(0,0,clock1_width,clock1_height,(uint16_t *)clock1);

    tft->output();
}

void loop()
{
}

