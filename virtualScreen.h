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
    bool dirty;
} Screen;

class ScreenBuilder
{
    std::vector<Screen> screens;
    int totalRows = 0;
    int maxColumns = 0;
    int virtualScreenWidth = 0;
    int virtualScreenHeight = 0;

public:
    ScreenBuilder &addRow(std::initializer_list<ScreenRow> screenRows)
    {
        int columnCount = screenRows.size();
        maxColumns = std::max(maxColumns, columnCount);
        int column = 0;
        for (const auto &screenRow : screenRows)
        {
            screens.push_back({totalRows, column, screenRow.cs, screenRow.rotation});
            ++column;
        }
        ++totalRows;

        // Update virtual screen dimensions
        virtualScreenWidth = display.width() * maxColumns;
        virtualScreenHeight = display.height() * totalRows;

        return *this;
    }

    std::vector<Screen> &getScreens()
    {
        return screens;
    }

    Screen *getScreen(int x, int y)
    {
        // Calculate which row and column the x, y coordinates fall into
        int column = x / display.width();
        int row = y / display.height();

        // Iterate through the screens to find the matching one
        for (auto &screen : screens)
        {
            if (screen.row == row && screen.column == column)
            {
                return &screen; // Return the address of the screen
            }
        }

        // If no matching screen is found, return a null pointer
        return nullptr;
    }

    int width() const
    {
        return virtualScreenWidth;
    }

    int height() const
    {
        return virtualScreenHeight;
    }
};

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

    void clearDirtyFlag()
    {
        auto &screens = screenBuilder->getScreens();

        for (auto &screen : screens)
        {
            screen.dirty = false;
        }
    }
    void initPhysicalScreens()
    {
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
        clearDirtyFlag();
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
        Serial.printf("Virtual Screen Width=%d\n", _width);
        Serial.printf("Virtual Screen Height=%d\n", _height);
        return _ready;
    }

    void output()
    {
        const auto &screens = screenBuilder->getScreens();

        for (const auto &screen : screens)
        {

            uint16_t *screenImage = getScreenImage(screen);
            if (screen.dirty)
            {
                digitalWrite(screen.cs, LOW);
                display.pushImage(0, 0, display.width(), display.height(), screenImage);
                digitalWrite(screen.cs, HIGH);
            }
        }
        clearDirtyFlag();
    }

    ~VirtualDisplay()
    {
        if (canvas)
        {
            free(canvas);
        }
    }

    uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
    {
        return (uint16_t)((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

    int16_t width()
    {
        return _width;
    }
    int16_t height()
    {
        return _height;
    }

    void pushImage(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t *buffer)
    {
        if (x < 0)
        {
            x = 0;
        }
        if (y < 0)
        {
            y = 0;
        }
        if (x + width > _width)
        {
            x = _width;
        }
        if (y + height > _height)
        {
            y = _height;
        }

        for (int16_t i = 0; i < height; i++)
        {
            for (int16_t j = 0; j < width; j++)
            {
                int bufferIndex = i * width + j;

                int canvasIndex = (y + i) * _width + (x + j);

                drawPixel(x + j, y + i, buffer[bufferIndex]);
            }
        }
    }

    void readRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t *buffer)
    {
        if (x < 0)
        {
            x = 0;
        }
        if (y < 0)
        {
            y = 0;
        }
        if (x + width > _width)
        {
            x = _width;
        }
        if (y + height > _height)
        {
            y = _height;
        }

        for (int16_t row = 0; row < height; ++row)
        {
            for (int16_t col = 0; col < width; ++col)
            {
                int canvasIndex = ((y + row) * _width) + (x + col);
                int bufferIndex = (row * width) + col;
                buffer[bufferIndex] = canvas[canvasIndex];
            }
        }
    }
    void drawPixel(int16_t x, int16_t y, uint16_t color) override
    {
        if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
        {
            return;
        }

        Screen *screen = screenBuilder->getScreen(x, y);

        if (screen)
        {
            screen->dirty = true;
        }

        canvas[y * _width + x] = color;
    }

    void startWrite(void) override
    {
    }

    void endWrite(void) override
    {
    }
};

#endif
