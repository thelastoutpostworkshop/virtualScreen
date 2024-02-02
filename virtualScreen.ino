#include "virtualScreen.h"
#include "images/ballcourt.h"

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    ScreenBuilder screens;

    // 4 sreens on one row
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    // Demonstrations
    //

    runBouncingBall(tft);

}

void loop()
{
}

class BouncingBall
{
private:
    float x, y;       // Ball's position
    float vx, vy;     // Ball's velocity
    float gravity;    // Gravity effect
    float elasticity; // Bounce reduction factor
    int ballRadius;   // Ball radius
    int rotation = 0; // Rotation angle of the ball's texture

    VirtualDisplay *display; // Pointer to your virtual display

public:
    BouncingBall(VirtualDisplay *display) : display(display)
    {
        // Initial ball setup
        ballRadius = 20; // Adjust as needed
        x = 0;
        y = display->height() / 2;
        vx = 20;        // Initial horizontal velocity
        vy = 5;         // Initial vertical velocity
        gravity = 0.5;  // Adjust as needed
        elasticity = 1; // Adjust as needed (between 0 and 1)
    }

    void update()
    {
        // Apply gravity
        vy += gravity;

        // Update position
        x += vx;
        y += vy;

        // Update rotation
        rotation += vx / 5.0; // Adjust rotation speed based on velocity

        // Collision detection with the screen bounds
        if (y > display->height() - ballRadius)
        { // Bottom edge
            y = display->height() - ballRadius;
            vy = -vy * elasticity;
        }
        else if (y < ballRadius)
        { // Top edge
            y = ballRadius;
            vy = -vy * elasticity;
        }

        if (x > display->width() - ballRadius || x < ballRadius)
        { // Side edges
            vx = -vx;
        }
        // Keep the rotation within 0-360 degrees
        rotation = (rotation + 10) % 360;
    }

    void draw(uint16_t color)
    {
        display->fillCircle(x, y, ballRadius, color); // Draw the ball

        if (color != TFT_BLACK)
        {
            // Draw a round spot on the edge of the ball to simulate rolling
            float rad = rotation * (PI / 180.0);
            float spotX = x + (ballRadius - 8) * cos(rad); // Adjust -5 to ensure the spot is within the ball's edge
            float spotY = y + (ballRadius - 8) * sin(rad);
            display->fillCircle(spotX, spotY, 5, TFT_BLACK); // Draw the spot
        }
    }
};

void runBouncingBall(VirtualDisplay *display)
{
    BouncingBall ball(display);
    display->fillScreen(TFT_BLACK); // Clear the screen
    display->drawRGBBitmap(0, 0, (uint16_t *)ballcourt, ballcourt_width, ballcourt_height);
    while (true)
    {
        ball.draw(TFT_BLACK);
        ball.update();
        ball.draw(0xfda0);
        display->output();
    }
}
