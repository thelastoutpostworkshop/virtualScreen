#include <virtualScreen.h>
#include "xtrchr30pt7b.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen configuration
    screens.addRow({{16, 0}, {15, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    tft->setFont(&xtrchr30pt7b);
    tft->setTextColor(0x3fa7);
    tft->setCursor(0,90);
    tft->print("Virtual Display with Adafruit GFX");
    tft->output();
}

void loop()
{
}

