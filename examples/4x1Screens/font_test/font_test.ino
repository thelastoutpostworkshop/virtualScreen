// Screen Layout : 4 screens on a single row
// See the tutorial : 
//


#include "virtualScreen.h"
#include "Starjedi32pt7b.h"
#include "raidercrusader48pt7b.h"
#include "Star_Trek_Enterprise_Future80pt7b.h"

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    ScreenBuilder screens;

    // 4 sreens on one row
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();


    tft->fillScreen(TFT_BLACK);
    tft->drawRect(10, 20, tft->width() - 50, tft->height() - 50, TFT_GREEN);
    tft->drawRect(12, 23, tft->width() - 48, tft->height() - 48, TFT_GREEN);
    tft->drawRoundRect(20, 50, tft->width() - 80, tft->height() - 100, 20, TFT_CYAN);
    printCenteredText(tft, "Star Wars Yoda Jedi", &Starjedi32pt7b, 0x00ff0c);
    tft->output();
    delay(5000);
    tft->fillScreen(TFT_BLACK);
    printCenteredText(tft, "We are Crusaders!", &raidercrusader48pt7b, 0x17bb);
    tft->output();
    delay(5000);
    tft->fillScreen(TFT_BLACK);
    printCenteredText(tft, "Star Trek Kirk", &Star_Trek_Enterprise_Future80pt7b, 0x3728);
    tft->output();
}

void loop()
{
}

void printCenteredText(VirtualDisplay *tft, const String &text, const GFXfont *font, uint16_t color)
{

    tft->setFont(font);
    tft->setTextColor(color);

    int16_t x1, y1;
    uint16_t w, h;
    tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h); // Calculate the bounds of the text

    int x = (tft->width() - w) / 2;  // Calculate the x position to center the text
    int y = (tft->height() + h) / 2; // Center vertically

    tft->setCursor(x, y);
    tft->println(text);
}

