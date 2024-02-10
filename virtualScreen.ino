#include "src/virtualScreen.h"
#include "fonts/xtrchr30pt7b.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen configuration
    screens.addRow({{6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();

    tft->setFont(&xtrchr30pt7b);
    tft->setTextColor(TFT_CYAN);
    tft->setCursor(0,90);
    tft->print("Virtual Display");
    tft->output();
}

void loop()
{
}

