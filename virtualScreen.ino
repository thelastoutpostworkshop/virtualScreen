#include "virtual.h"
#include "Adafruit_GC9A01A.h"
#include <Adafruit_GFX.h>

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 18
#define TFT_DC 2  
#define TFT_RST 4 

Adafruit_GC9A01A display(-1, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);

typedef struct
{
    int row;
    int column;
    int cs;
} Screen;

int virtualWidth = 0;
int virtualHeight = 0;
int displayWidth = 0;
int displayHeight = 0;

#define ROWS 1    // Number of rows
#define COLUMNS 4 // Number of columns

Screen grid[ROWS][COLUMNS] = {
    {{.row = 0, .column = 0, 16},
     {.row = 0, .column = 1, 6},
     {.row = 0, .column = 2, 7},
     {.row = 0, .column = 3, 15}}};

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    if (psramFound())
    {
        Serial.printf("PSRAM Size = %lu\n", ESP.getPsramSize());
    }
    else
    {
        Serial.println("No PSRAM!");
        return;
    }
    initDisplay();

    calculateVirtualScreenSize();

    tft = new VirtualDisplay(virtualWidth, virtualHeight);

    Serial.printf("PSRAM Left = %lu\n", ESP.getFreePsram());

    if (!tft->ready())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }
    tft->fillScreen(GC9A01A_BLACK);
    tft->setTextSize(5);
    tft->setTextColor(GC9A01A_CYAN);
    tft->setCursor(35, 100);
    tft->println("This a test on a large screen");

    tft->drawRect(20, 80, virtualWidth - 60, 70, GC9A01A_GREEN);


    output();
}

void loop()
{
}

void initDisplay()
{
    displayWidth = display.width();
    displayHeight = display.height();
    display.begin();
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            pinMode(grid[i][j].cs, OUTPUT);
            digitalWrite(grid[i][j].cs, HIGH);
        }
        for (int j = 0; j < COLUMNS; ++j)
        {
            digitalWrite(grid[i][j].cs, LOW);
            display.setRotation(2);
            digitalWrite(grid[i][j].cs, HIGH);
        }
    }
}
void calculateVirtualScreenSize()
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            if (i == 0)
            { // Add widths for the first row
                virtualWidth += display.width();
            }
            if (j == 0)
            { // Add heights for the first column
                virtualHeight += display.height();
            }
        }
    }

    Serial.print("Total Width: ");
    Serial.print(virtualWidth);
    Serial.print(", Total Height: ");
    Serial.println(virtualHeight);
}

void output()
{
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLUMNS; ++col)
        {
            Screen &currentScreen = grid[row][col];
            uint16_t *screenImage = getScreenImage(currentScreen);
            digitalWrite(currentScreen.cs, LOW);
            display.drawRGBBitmap(0, 0, screenImage, displayWidth, displayHeight);
            digitalWrite(currentScreen.cs, HIGH);

            delete[] screenImage;
        }
    }
}

uint16_t *getScreenImage(const Screen &screen)
{
    // Allocate memory for the screen image
    uint16_t *screenImage = new uint16_t[displayWidth * displayHeight];

    // Calculate the starting position of the screen in the buffer
    uint32_t position = (screen.row * displayHeight * virtualWidth) + (screen.column * displayWidth);

    uint16_t *buffer = (uint16_t *)tft->getCanvas();
    uint16_t *startPos = buffer + position;

    // Copy the screen image from the buffer
    for (int y = 0; y < displayHeight; ++y)
    {
        for (int x = 0; x < displayWidth; ++x)
        {
            screenImage[y * displayWidth + x] = startPos[y * virtualWidth + x];
        }
    }

    return screenImage;
}
