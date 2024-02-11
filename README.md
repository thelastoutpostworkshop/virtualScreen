# VirtualScreen: An Arduino Library for Auto-Spanning Adafruit GFX Graphics Across Multiple Displays

**Seamlessly Integrate Multiple Displays with Ease: A User-Friendly Approach to Expansive Graphics**.<br>
<a href="https://www.buymeacoffee.com/thelastoutpostworkshop" target="_blank">
    <img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee">
</a>


## Youtube Tutorial
[<img src="https://github.com/thelastoutpostworkshop/images/blob/main/GPIO%20Viewer.png" width="300">](https://youtu.be/UxkOosaNohU)

## Installation
### Installation Arduino IDE (Version 2)

- Install the **VirtualDisplay Library with the Arduino IDE Library Manager**, it will also install the [TFT_eSPI library](https://github.com/Bodmer/TFT_eSPI) serving as driver for the displays.


## Usage
>ℹ️ There are many examples and display configuration included with the library `File > Examples > VirtualDisplay`<br>
```c
#include <virtualScreen.h>

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
  Serial.begin(115200);

    // Adjust this setup according to your actual screen configuration
    screens.addRow({{6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();

    // Use any Adafruit GFX Graphics functions
    tft->setTextColor(TFT_CYAN);
    tft->setCursor(0,90);
    tft->print("Virtual Display");

    tft->output(); // Use this function to output the result on the screens
}
```
## Screen arrangement
>ℹ️ You can only use screen of the same resolution<br>

Add rows of screens by using the ``` addRow() ``` function.<br>
A screen is definded by ``` {int chip_select_pin, int rotation} ```<br>
```screens.addRow({{6, 0}, {7, 0}}); ``` adds a row of two screens of rotation 0, the first screen has a chip select on pin 7, and the other has the chip select on pin 7.

## Driver Setup for your Screens
- You have to setup the driver in the [TFT_eSPI library](https://github.com/Bodmer/TFT_eSPI) configuration files for your display type, see my tutorial on how to do it.

## Display supported
- Any 16 bit-color display supported by the [TFT_eSPI library](https://github.com/Bodmer/TFT_eSPI)

## Recommended ESP32
- Spanning graphics across multiple displays requires a fair amount of memory, use an **ESP32 with PSRAM**.
- The [ESP32-S3 N16R2](https://amzn.to/3TsziTh) is a good choice.

## Graphics functions
>ℹ️ You can use any graphics functions from the [Adafruit GFX library](https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives)<br>
Here are other graphics functions built in the VirtualScreen Library :

| Graphics functions  | Description |
|---------|-------------|
|output()  | Output to the screens according to your screen arrangements |
|highlightArea(int16_t x, int16_t y, int16_t width, int16_t height, float intensity)  | Highlight a specified area of the canvas with adjustable intensity |
|highlightArea(int16_t centerX, int16_t centerY, int16_t radius, float intensity)  | Highlight a circular area of the canvas with adjustable intensity |
|color565(uint8_t r, uint8_t g, uint8_t b)  | Convert RGB Color to a 565 color code |
|readRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t *buffer)  | Read an image area into buffer (must be allocated before calling the function) |


## Performance
The library will redraw a screen only if its content has changed, so avoid redrawing the entire canvas if you can.

## Contributors
Contributors are welcomed!  If you want to submit pull requests, [here is how you can do it](https://docs.github.com/en/get-started/exploring-projects-on-github/contributing-to-a-project).
