// Screen Layout : 2 screens on a single row
// See the tutorial : https://youtu.be/eCfeJh9Pyns
//

#include "virtualScreen.h"
#include "xtrchr30pt7b.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen layout
    screens.addRow({{6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();

    tft->setFont(&xtrchr30pt7b);
    tft->fillScreen(0xfaa0);
    tft->drawRoundRect(10, 10, tft->width() - 20, tft->height() - 20, 25, TFT_CYAN);
    tft->drawRoundRect(20, 20, tft->width() - 40, tft->height() - 40, 25, TFT_CYAN);
    tft->setTextColor(TFT_CYAN);
    tft->setCursor(80, 140);
    tft->print("Amazing!");
    tft->output();
}

void loop()
{
    static bool toggleColor = false;
    tft->fillCircle(400, tft->height()/2, 30, toggleColor ? TFT_YELLOW : 0xfaa0);
    tft->output();
    delay(400);
    toggleColor = !toggleColor;
}
