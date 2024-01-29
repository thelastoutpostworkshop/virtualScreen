#include "virtualScreen.h"
#include "fonts/Starjedi32pt7b.h"
#include "fonts/raidercrusader48pt7b.h"
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

void font_test(VirtualDisplay *tft)
{
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
}