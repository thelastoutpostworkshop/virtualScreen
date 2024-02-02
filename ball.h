#include "virtualScreen.h"

class BouncingBall
{
private:
    float x, y;       // Ball's position
    float vx, vy;     // Ball's velocity
    float gravity;    // Gravity effect
    float elasticity; // Bounce reduction factor
    int ballRadius;   // Ball radius
    float rotation;   // Rotation angle of the ball's texture

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
    }

    void draw(uint16_t color)
    {
        display->fillCircle(x, y, ballRadius, color); // Draw the ball

        if (color != TFT_BLACK)
        {
            // Draw texture on the ball to simulate rolling
            for (int i = 0; i < 360; i += 45)
            { // Draw every 45 degrees
                float rad = (i + rotation) * (PI / 180.0);
                float lineX = ballRadius * cos(rad);
                float lineY = ballRadius * sin(rad);
                display->drawLine(x, y, x + lineX, y + lineY, TFT_BLACK);
            }
        }
    }
};

void runBouncingBall(VirtualDisplay *display)
{
    BouncingBall ball(display);
    display->fillScreen(TFT_BLACK); // Clear the screen

    while (true)
    {
        ball.draw(TFT_BLACK);
        ball.update();
        ball.draw(TFT_WHITE);
        display->output();
    }
}
