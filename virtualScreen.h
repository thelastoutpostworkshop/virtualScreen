#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>
#include <vector>

#ifndef _VIRTUAL_SCREEN_
#define _VIRTUAL_SCREEN_

#define pixelSize 2 // Pixel Size in bytes

TFT_eSPI display = TFT_eSPI();

typedef struct
{
    int cs;
    int rotation;
} ScreenRow;

typedef struct
{
    int row;
    int column;
    int cs;
    int rotation;
} Screen;

class ScreenBuilder {
    std::vector<Screen> screens;
    int totalRows = 0;
    int maxColumns = 0;
    int virtualScreenWidth = 0;
    int virtualScreenHeight = 0;

public:
    ScreenBuilder &addRow(std::initializer_list<ScreenRow> screenRows) {
        int columnCount = screenRows.size();
        maxColumns = std::max(maxColumns, columnCount);
        int column = 0;
        for (const auto &screenRow : screenRows) {
            screens.push_back({totalRows, column, screenRow.cs, screenRow.rotation});
            ++column;
        }
        ++totalRows;

        // Update virtual screen dimensions
        virtualScreenWidth = display.width() * maxColumns;
        virtualScreenHeight = display.height() * totalRows;

        return *this;
    }

    const std::vector<Screen> &getScreens() const {
        return screens;
    }

    int width() const {
        return virtualScreenWidth;
    }

    int height() const {
        return virtualScreenHeight;
    }
};


typedef void (*CallbackFunction)();
class VirtualDisplay : public Adafruit_GFX
{
private:
    uint16_t *canvas;
    size_t canvasSize;
    size_t displayBufferSize;
    uint16_t *displayBuffer;
    int16_t _width;
    int16_t _height;
    bool _ready = false;
    CallbackFunction callBackFunction;
    ScreenBuilder *screenBuilder;

    void clearDisplayBuffer()
    {
        memset(canvas, 0, displayBufferSize);
    }

    uint16_t *getScreenImage(const Screen &screen)
    {
        // Calculate the starting position of the screen in the buffer
        uint32_t position = (screen.row * display.height() * screenBuilder->width()) + (screen.column * display.width());

        uint16_t *buffer = canvas;
        uint16_t *startPos = buffer + position;

        // Copy the screen image from the buffer
        for (int y = 0; y < display.height(); ++y)
        {
            for (int x = 0; x < display.width(); ++x)
            {
                displayBuffer[y * display.width() + x] = startPos[y * screenBuilder->width() + x];
            }
        }

        return displayBuffer;
    }

    void initPhysicalScreens() {
        display.begin();
        display.setSwapBytes(true);
        const auto &screens = screenBuilder->getScreens();

        for (const auto &screen : screens)
        {
            pinMode(screen.cs, OUTPUT);
            digitalWrite(screen.cs, HIGH);
        }
        for (const auto &screen : screens)
        {
            digitalWrite(screen.cs, LOW);
            display.setRotation(screen.rotation);
            digitalWrite(screen.cs, HIGH);
        }
    }

public:
    VirtualDisplay(int16_t w, int16_t h, ScreenBuilder *builder) : Adafruit_GFX(w, h)
    {
        _width = w;
        _height = h;
        canvasSize = w * h * pixelSize;
        if ((canvas = (uint16_t *)malloc(canvasSize)))
        {
            memset(canvas, 0, canvasSize);
        }
        displayBufferSize = w * h * pixelSize;
        if ((displayBuffer = (uint16_t *)malloc(displayBufferSize)))
        {
            clearDisplayBuffer();
        }
        _ready = true;
        screenBuilder = builder;
    }

    bool begin()
    {
        initPhysicalScreens();
        Serial.printf("Virtual Screen Width=%d\n",_width);
        Serial.printf("Virtual Screen Height=%d\n",_height);
        return _ready;
    }

    void output()
    {
        const auto &screens = screenBuilder->getScreens();

        for (const auto &screen : screens)
        {
            uint16_t *screenImage = getScreenImage(screen);
            digitalWrite(screen.cs, LOW);
            display.pushImage(0, 0, display.width(), display.height(), screenImage);
            digitalWrite(screen.cs, HIGH);
        }
    }

    ~VirtualDisplay()
    {
        if (canvas)
        {
            free(canvas);
        }
    }
    // void setCallBack(CallbackFunction callback)
    // {
    //     callBackFunction = callback;
    //     memcpy(canvasBefore, canvas, canvasSize);
    // }

    int16_t width()
    {
        return _width;
    }
    int16_t height()
    {
        return _height;
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color) override
    {
        if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
        {
            return;
        }

        // // Calculate the position in the canvas array
        canvas[y * _width + x] = color;
    }

    void startWrite(void) override
    {
        // startWriteLevel++;
    }

    void endWrite(void) override
    {
        // endWriteLevel++;
        // if (startWriteLevel == endWriteLevel)
        // {
        //     callBackFunction();
        //     resetWriteLevels();
        // }
    }
};

#endif
