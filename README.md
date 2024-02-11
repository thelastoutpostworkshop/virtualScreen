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
#include <gpio_viewer.h> // Must me the first include in your project
GPIOViewer gpio_viewer;

void setup()
{
  Serial.begin(115200);

  // Comment the next line, If your code aleady include connection to Wifi in mode WIFI_STA (WIFI_AP and WIFI_AP_STA are not supported)
  gpio_viewer.connectToWifi("Your SSID network", "Your WiFi Password");
  // gpio_viewer.setPort(5555);   // You can set the http port, if not set default port is 8080

  // Your own setup code start here

  // Must be at the end of your setup
  // gpio_viewer.setSamplingInterval(25); // You can set the sampling interval in ms, if not set default is 100ms
  gpio_viewer.begin();
}
```
>ℹ️ The default HTTP port is **8080** and default sampling interval is **100ms**<br>
>ℹ️ Wifi must be in mode WIFI_STA (WIFI_AP and WIFI_AP_STA are not supported) 


## Performance
- Ensure you have a strong Wifi signal with a good transfer rate.  25ms sampling interval works great on Wifi 6 with 125 Mbps.
- If you get "ERROR: Too many messages queued" on the Serial Monitor, this means the data is not read fast enough by the web application.  The data will still be displayed, but with some latency.  Reduce the sampling interval or try to improve your Wifi performance.

## Contributors
Contributors are welcomed!  If you want to submit pull requests, [here is how you can do it](https://docs.github.com/en/get-started/exploring-projects-on-github/contributing-to-a-project).
