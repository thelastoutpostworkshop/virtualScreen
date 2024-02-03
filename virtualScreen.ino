#include "virtualScreen.h"
#include "images/ballcourt.h" // Ensure this is correctly included

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    ScreenBuilder screens;
    // Adjust this setup according to your actual screen configuration
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    // Start the bouncing ball animation
    runBouncingBall(tft);
}

void loop()
{
    // Your loop can remain empty if everything is handled in runBouncingBall
}

class BouncingBall
{
private:
    float x, y;        // Ball's position
    float vx, vy;      // Ball's velocity
    float gravity;     // Gravity effect
    float elasticity;  // Bounce reduction factor
    int ballRadius;    // Ball radius
    int captureRadius; // Slightly larger to ensure clean edges
    int rotation = 0;

    uint16_t *backgroundBuffer; // Buffer for storing background
    VirtualDisplay *display;    // Pointer to virtual display

public:
    BouncingBall(VirtualDisplay *display) : display(display)
    {
        ballRadius = 20;
        captureRadius = ballRadius + 4;
        x = ballRadius + 5;
        y = display->height() / 2;
        vx = 10;                                                                    // Initial horizontal velocity
        vy = 0;                                                                     // Initial vertical velocity
        gravity = 0.8;                                                              // Adjust as needed
        elasticity = 1;                                                             // Adjust as needed (between 0 and 1)
        backgroundBuffer = new uint16_t[(captureRadius * 2) * (captureRadius * 2)]; // Allocate memory for background buffer
    }

    ~BouncingBall()
    {
        if (backgroundBuffer)
        {
            delete[] backgroundBuffer; // Free memory when done
        }
    }

    void update()
    {
        // Logic to update ball position and handle collisions
        vy += gravity;
        x += vx;
        y += vy;

        if (x <= ballRadius || x >= display->width() - ballRadius)
        {
            vx = -vx;
            x += vx;
        }

        if (y <= ballRadius || y >= display->height() - ballRadius)
        {
            vy = -vy * elasticity;
            y += vy;
        }
        rotation += vx / 5.0; // Adjust rotation speed based on velocity
    }

    void init()
    {
        display->readRect(x - captureRadius, y - captureRadius, captureRadius * 2, captureRadius * 2, backgroundBuffer);
    }

    void draw()
    {

        // Erase the ball at the current position by restoring the background
        display->pushImage(x - captureRadius, y - captureRadius, captureRadius * 2, captureRadius * 2, backgroundBuffer);

        // Update position for the next frame
        update();

        // Capture the background at the new position, slightly larger area
        display->readRect(x - captureRadius, y - captureRadius, captureRadius * 2, captureRadius * 2, backgroundBuffer);

        // Draw the ball at the new position
        display->fillCircle(x, y, ballRadius, 0x8301);

        // Draw a round spot on the edge of the ball to simulate rolling
        float rad = rotation * (PI / 180.0);
        float spotX = x + (ballRadius - 8) * cos(rad); // Adjust -5 to ensure the spot is within the ball's edge
        float spotY = y + (ballRadius - 8) * sin(rad);
        display->fillCircle(spotX, spotY, 5, TFT_BLACK); // Draw the spot
    }
};

void runBouncingBall(VirtualDisplay *display)
{
    BouncingBall ball(display);

    display->fillScreen(TFT_BLACK);
    display->drawRGBBitmap(0, 0, (uint16_t *)ballcourt, ballcourt_width, ballcourt_height);
    ball.init();
    while (true)
    {
        ball.draw();
        display->output(); // Update the virtual display
    }
}
