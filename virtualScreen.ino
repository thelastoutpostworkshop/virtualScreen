#include "virtualScreen.h"
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>

#include "images/newyork_960x240.h"
#include "images/highway_720x480.h"
#include "images/night_earth_960x240.h"
#include <Fonts/GFXFF/FreeSansBold24pt7b.h>
#include "fonts/Aurebesh_Bold32pt7b.h"
#include "fonts/Bombing40pt7b.h"

#include "gameoflife.h"
#include "maze.h"
#define test_image highway

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 18
#define TFT_DC 2
#define TFT_RST 4

VirtualDisplay *tft;

void setup()
{
    Serial.begin(115200);

    if (psramFound())
    {
        Serial.printf("PSRAM Size = %lu\n", ESP.getPsramSize());
    }
    else
    {
        Serial.println("No PSRAM!");
        return;
    }
    Serial.printf("PSRAM Left = %lu\n", ESP.getFreePsram());

    ScreenBuilder builder;
    builder.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});
    tft = new VirtualDisplay(builder.width(), builder.height(), &builder);

    if (!tft->begin())
    {
        Serial.println("Memory Allocation for virtual screen failed");
        return;
    }

    // Demonstrations
    //
    // test_animation();
    // test_images();
    // test_pong();
    gameOfLife(tft);
    // test_maze(tft);
}

void loop()
{
}

void test_pong()
{
    const int paddleWidth = 20;
    const int paddleHeight = 50;
    const int ballSize = 8;
    int ballX, ballY;                           // Ball position
    int ballVelocityX = 12, ballVelocityY = 12; // Ball velocity
    int leftPaddleY, rightPaddleY;              // Paddle positions
    ballX = tft->width() / 2;
    ballY = tft->height() / 2;
    leftPaddleY = rightPaddleY = (tft->height() - paddleHeight) / 2;
    tft->fillScreen(TFT_BLACK); // Clear the screen
    for (int i = 0; i < 1000; i++)
    {
        tft->fillRect(0, leftPaddleY, paddleWidth, paddleHeight, TFT_BLACK);                           // Left paddle
        tft->fillRect(tft->width() - paddleWidth, rightPaddleY, paddleWidth, paddleHeight, TFT_BLACK); // Right paddle
        tft->fillCircle(ballX, ballY, ballSize, TFT_BLACK);

        // Update ball position
        ballX += ballVelocityX;
        ballY += ballVelocityY;

        // Check for ball collisions with the top and bottom of the screen
        if (ballY <= 0 || ballY >= tft->height() - ballSize)
        {
            ballVelocityY = -ballVelocityY;
        }

        // Update paddles position to follow the ball
        leftPaddleY = ballY - paddleHeight / 2;
        rightPaddleY = ballY - paddleHeight / 2;

        // Keep paddle within the screen bounds
        leftPaddleY = max(0, min(tft->height() - paddleHeight, leftPaddleY));
        rightPaddleY = max(0, min(tft->height() - paddleHeight, rightPaddleY));

        // Check for ball collisions with the paddles
        if ((ballX <= paddleWidth && ballY >= leftPaddleY && ballY <= leftPaddleY + paddleHeight) ||
            (ballX >= tft->width() - paddleWidth - ballSize && ballY >= rightPaddleY && ballY <= rightPaddleY + paddleHeight))
        {
            ballVelocityX = -ballVelocityX;

            // Randomize the ball's Y velocity slightly to change its direction
            ballVelocityY += random(-2, 3); // Change in Y velocity can be -2, -1, 0, 1, or 2
        }

        // Draw the updated game state
        tft->fillRect(0, leftPaddleY, paddleWidth, paddleHeight, 0xf818);                           // Left paddle
        tft->fillRect(tft->width() - paddleWidth, rightPaddleY, paddleWidth, paddleHeight, 0x07e6); // Right paddle
        tft->fillCircle(ballX, ballY, ballSize, 0xe7e0);                                            // Ball
        tft->output();
    }
}

void test_images()
{
    tft->drawRGBBitmap(0, 0, (uint16_t *)newyork, newyork_width, newyork_height);
    tft->output();
    delay(5000);
    tft->drawRGBBitmap(0, 0, (uint16_t *)night_earth, night_earth_width, night_earth_height);
    tft->output();
}

void test_1()
{
    tft->fillScreen(TFT_BLACK);
    tft->setFont(&FreeSansBold24pt7b);
    tft->setTextColor(TFT_CYAN);
    tft->setCursor(10, 75);
    tft->println("This a test on a large screen, resolution is 720 x 480 pixels!  You can use any AdafruitGFX functions on the virtual Screen.");
    tft->output();
}
void test_2()
{
    tft->fillScreen(TFT_BLACK);
    tft->drawRect(10, 20, tft->width() - 50, tft->height() - 50, TFT_GREEN);
    tft->drawRect(12, 23, tft->width() - 48, tft->height() - 48, TFT_GREEN);
    tft->drawRoundRect(20, 50, tft->width() - 80, tft->height() - 100, 20, TFT_CYAN);
    printCenteredText("Welcome to Virtual Screens!", &Bombing40pt7b, 0x00ff0c);
    tft->output();
}

void test_animation()
{
    int ballX = 30;               // Initial X position
    int ballY = 30;               // Initial Y position
    int ballSize = 50;            // Diameter of the ball
    int velocityX = 8;            // X Velocity
    int velocityY = 8;            // Y Velocity
    uint16_t ballColor = TFT_RED; // Start with red color

    tft->fillScreen(TFT_BLACK); // Clear the screen

    for (int i = 0; i < 1000; i++)
    {
        tft->fillCircle(ballX, ballY, ballSize, TFT_BLACK);

        // Update the ball's position
        ballX += velocityX;
        ballY += velocityY;

        // Check for collisions with the screen edges
        if (ballX <= 0 || ballX >= tft->width() - ballSize || ballY <= 0 || ballY >= tft->height() - ballSize)
        {
            if (ballX <= 0 || ballX >= tft->width() - ballSize)
            {
                velocityX = -velocityX; // Reverse X direction
            }
            if (ballY <= 0 || ballY >= tft->height() - ballSize)
            {
                velocityY = -velocityY; // Reverse Y direction
            }

            // Change the ball color on collision
            ballColor = getRandomColor();
        }

        // Draw the new ball
        tft->fillCircle(ballX, ballY, ballSize, ballColor);
        // Assuming output() is a function to update the display
        tft->output();
    }
}

uint16_t getRandomColor()
{
    // Generate random values for red (5 bits), green (6 bits), and blue (5 bits)
    uint8_t red = random(0, 32);   // 0 to 31
    uint8_t green = random(0, 64); // 0 to 63
    uint8_t blue = random(0, 32);  // 0 to 31

    // Combine them into one 16-bit color in RGB565 format
    return (red << 11) | (green << 5) | blue;
}

void printCenteredText(const String &text, const GFXfont *font, uint16_t color)
{

    tft->setFont(font);
    tft->setTextColor(color);

    int16_t x1, y1;
    uint16_t w, h;
    tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h); // Calculate the bounds of the text

    int x = (tft->width() - w) / 2;  // Calculate the x position to center the text
    int y = (tft->height() + h) / 2; // Center vertically

    tft->setCursor(x, y);
    tft->println(text);
}
