#include "virtualScreen.h"
#include "images/clock_1.h"
#include "images/clock_2.h"
#include "images/clock_3.h"
#include "fonts/SecondWorldDemoRegular40pt7b.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

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
    tft->pushImage(240, 0, clock2_width, clock2_height, (uint16_t *)clock2);
    tft->pushImage(480, 0, clock3_width, clock3_height, (uint16_t *)clock3);
    tft->setFont(&SecondWorldDemoRegular40pt7b);
    tft->setTextColor(0x063f);
    tft->setCursor(50, 385);
    tft->print("Very Strange Clocks");
}
void loop()
{
    tft->highlightArea(360, 120, 50, 0.5f);

    tft->output();
    tft->pushImage(240, 0, clock2_width, clock2_height, (uint16_t *)clock2);
    tft->output();
}
