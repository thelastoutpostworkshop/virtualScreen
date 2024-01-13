#include <Adafruit_GFX.h>

#define pixelSize 2 // Pixel Size in bytes

typedef void (*CallbackFunction)();
class VirtualDisplay : public Adafruit_GFX
{
private:
    uint16_t *canvas;
    size_t canvasSize;
    int16_t _width;
    int16_t _height;
    CallbackFunction callBackFunction;

public:
    VirtualDisplay(int16_t w, int16_t h) : Adafruit_GFX(w, h)
    {
        _width = w;
        _height = h;
        canvasSize = w * h * pixelSize;
        if ((canvas = (uint16_t *)malloc(canvasSize)))
        {
            memset(canvas, 0, canvasSize);
        }
        // if ((canvasBefore = (uint8_t *)malloc(canvasSize)))
        // {
        //     memset(canvasBefore, 0, canvasSize);
        // }
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
    // uint8_t *getCurrentPixelsNotWritten()
    // {
    //     return canvasBefore;
    // }

    bool ready()
    {
        return canvas != NULL;
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
