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

    tft->setTextColor(TFT_CYAN);
    tft->setCursor(0,90);
    tft->print("Virtual Display");
    tft->output();
}
```

## Performance
- Ensure you have a strong Wifi signal with a good transfer rate.  25ms sampling interval works great on Wifi 6 with 125 Mbps.
- If you get "ERROR: Too many messages queued" on the Serial Monitor, this means the data is not read fast enough by the web application.  The data will still be displayed, but with some latency.  Reduce the sampling interval or try to improve your Wifi performance.

## Contributors
Contributors are welcomed!  If you want to submit pull requests, [here is how you can do it](https://docs.github.com/en/get-started/exploring-projects-on-github/contributing-to-a-project).
