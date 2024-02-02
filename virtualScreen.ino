#include "virtualScreen.h"

#include "ball.h"

VirtualDisplay *tft;


void setup()
{
    Serial.begin(115200);

    ScreenBuilder screens;

    // 4 sreens on one row
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    // Demonstrations
    //

    runBouncingBall(tft);

}

void loop()
{
}
