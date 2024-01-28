#include <Adafruit_GFX.h>

#ifndef _VIRTUAL_SCREEN_
#define _VIRTUAL_SCREEN_

#define pixelSize 2 // Pixel Size in bytes

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
    
    void clearDisplayBuffer()
    {
        memset(canvas, 0, displayBufferSize);
    }

public:
    VirtualDisplay(int16_t w, int16_t h, int16_t displayWidth, int16_t displayHeight) : Adafruit_GFX(w, h)
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

    uint16_t *getCanvas()
    {
        return canvas;
    }

    uint16_t *getDisplayBuffer()
    {
        return displayBuffer;
    }

    bool ready()
    {
        return _ready;
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
