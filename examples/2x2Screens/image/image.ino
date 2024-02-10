#include "virtualScreen.h"
#include "spaceship_480x480.h"
#include "spacedock_480x480.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen configuration
    screens.addRow({{6, 0}, {7, 0}});
    screens.addRow({{16, 2}, {15, 2}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();

    tft->fillScreen(TFT_BLACK);
    tft->drawRGBBitmap(0, 0, (uint16_t *)spaceship_480x480, spaceship_480x480_width, spaceship_480x480_height);
    tft->output();
    delay(5000);
    tft->drawRGBBitmap(0, 0, (uint16_t *)spacedock_480x480, spacedock_480x480_width, spacedock_480x480_height);
    tft->output();
}

void loop()
{
}

