#include "virtualScreen.h"

class BouncingBall
{
private:
    float x, y;       // Ball's position
    float vx, vy;     // Ball's velocity
    float gravity;    // Gravity effect
    float elasticity; // Bounce reduction factor
    int ballRadius;   // Ball radius

    VirtualDisplay *display; // Pointer to your virtual display

public:
    BouncingBall(VirtualDisplay *display) : display(display)
    {
        // Initial ball setup
        ballRadius = 10; // Adjust as needed
        x = 0;
        y = display->height() / 2;
        vx = 5;           // Initial horizontal velocity
        vy = 0;           // Initial vertical velocity
        gravity = 0.5;    // Adjust as needed
        elasticity = 1; // Adjust as needed (between 0 and 1)
    }

    void update()
    {
        // Apply gravity
        vy += gravity;

        // Update position
        x += vx;
        y += vy;

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
