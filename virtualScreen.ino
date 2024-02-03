#include "virtualScreen.h"
#include "images/clock_1.h"

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    ScreenBuilder screens;
    // Adjust this setup according to your actual screen configuration
    // 6 screens 3 on each row
    screens.addRow({{7, 2}, {6, 2}, {15, 2}});
    screens.addRow({{16, 0}, {11, 0}, {9, 0}});
    
    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    tft->fillScreen(TFT_BLACK);
    tft->pushImage(0, 0, clock1_width, clock1_height, (uint16_t *)clock1);

    tft->output();
}

void loop()
{
}
