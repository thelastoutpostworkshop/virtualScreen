#include "virtualScreen.h"
#include "images/clock_1.h"
#include "images/clock_2.h"
#include "images/clock_3.h"
#include "fonts/SecondWorldDemoRegular40pt7b.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen configuration
    // 6 screens 3 on each row
    screens.addRow({{7, 2}, {6, 2}, {15, 2}});
    screens.addRow({{16, 0}, {11, 0}, {9, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    tft->fillScreen(TFT_BLACK);
    tft->pushImage(0, 0, clock1_width, clock1_height, (uint16_t *)clock1);
    tft->pushImage(240, 0, clock2_width, clock2_height, (uint16_t *)clock2);
    tft->pushImage(480, 0, clock3_width, clock3_height, (uint16_t *)clock3);
    tft->setFont(&SecondWorldDemoRegular40pt7b);
    tft->setTextColor(0xef48);
    tft->setCursor(50, 385);
    tft->print("Very Strange Clocks");
}

unsigned long lastUpdateTime = 0;
const long updateInterval = 1000; // Update every second

void loop()
{
    tft->highlightArea(360, 120, 50, 0.5f);

    tft->output();
    tft->pushImage(240, 0, clock2_width, clock2_height, (uint16_t *)clock2);
    tft->output();

    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdateTime >= updateInterval)
    {
        lastUpdateTime = currentMillis;

        // Assuming you have a way to get the current time in seconds
        int seconds = (currentMillis / 1000) % 60; // Simulated for demonstration

        tft->pushImage(0, 0, clock1_width, clock1_height, (uint16_t *)clock1);

        // Draw the new seconds hand
        drawSecondsHand(seconds, TFT_RED); // White color for the hand

        tft->output(); // Update the display with the changes
    }
}

void drawSecondsHand(int seconds, uint16_t color) {
    float angle = (seconds - 15) * (2 * M_PI / 60); // Convert seconds to angle
    int16_t centerX = 120, centerY = 120; // Center of the clock
    int16_t length = 60; // Length of the seconds hand

    // Tip of the seconds hand
    int16_t endX = centerX + length * cos(angle);
    int16_t endY = centerY + length * sin(angle);

    // Width of the base of the triangle hand
    int16_t handWidth = 10;

    // Calculate the two additional points for the base of the triangle
    float baseAngle = angle + M_PI / 2; // Perpendicular to the hand direction
    int16_t baseOffsetX = handWidth * cos(baseAngle) / 2;
    int16_t baseOffsetY = handWidth * sin(baseAngle) / 2;

    int16_t basePoint1X = centerX + baseOffsetX;
    int16_t basePoint1Y = centerY + baseOffsetY;
    int16_t basePoint2X = centerX - baseOffsetX;
    int16_t basePoint2Y = centerY - baseOffsetY;

    // Define points array for triangle
    int16_t points[6] = {endX, endY, basePoint1X, basePoint1Y, basePoint2X, basePoint2Y};

    // Draw the triangle
    tft->fillTriangle(points[0], points[1], points[2], points[3], points[4], points[5], color);
}

