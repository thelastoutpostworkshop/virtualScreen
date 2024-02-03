#include "virtualScreen.h"
#include "fonts/Fun_Games20pt7b.h"

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

    playPong();
}

void loop()
{
    // Your loop can remain empty if everything is handled in runBouncingBall
}

const int messageAreaWidth = 240; // Width of the message area on the left
const int scoreAreaHeight = 50;   // Height of the score area at the top

void playPong()
{
    const int paddleWidth = 20;
    const int paddleHeight = 50;
    const int ballSize = 8;
    int ballX, ballY;                           // Ball position
    int ballVelocityX = 12, ballVelocityY = 12; // Ball velocity
    int leftPaddleY, rightPaddleY;              // Paddle positions

    // Initialize ball and paddles positions
    ballX = messageAreaWidth + (tft->width() - messageAreaWidth) / 2;
    ballY = scoreAreaHeight + (tft->height() - scoreAreaHeight) / 2;
    leftPaddleY = rightPaddleY = scoreAreaHeight + (tft->height() - scoreAreaHeight - paddleHeight) / 2;

    tft->fillScreen(TFT_BLACK); // Clear the screen
    drawScores(10, 10);
    drawMessage("Pong");
    tft->output();
    while (true)
    {
        // Clear the previous positions of the ball and paddles
        tft->fillRect(messageAreaWidth, leftPaddleY, paddleWidth, paddleHeight, TFT_BLACK);            // Clear Left paddle
        tft->fillRect(tft->width() - paddleWidth, rightPaddleY, paddleWidth, paddleHeight, TFT_BLACK); // Clear Right paddle
        tft->fillCircle(ballX, ballY, ballSize, TFT_BLACK);

        // Update ball position
        ballX += ballVelocityX;
        ballY += ballVelocityY;

        // Check for ball collisions with the top and bottom of the screen
        if (ballY <= scoreAreaHeight + (ballSize * 2 + 5) || ballY >= tft->height() - ballSize)
        {
            ballVelocityY = -ballVelocityY;
        }

        // Update paddles position to follow the ball
        leftPaddleY = ballY - paddleHeight / 2;
        rightPaddleY = ballY - paddleHeight / 2;

        // Keep paddles within the screen bounds
        leftPaddleY = max(scoreAreaHeight, min(tft->height() - paddleHeight, leftPaddleY));
        rightPaddleY = max(scoreAreaHeight, min(tft->height() - paddleHeight, rightPaddleY));

        // Check for ball collisions with the paddles
        if ((ballX <= messageAreaWidth + paddleWidth+ballSize*2 && ballY >= leftPaddleY && ballY <= leftPaddleY + paddleHeight) ||
            (ballX >= tft->width() - paddleWidth - ballSize && ballY >= rightPaddleY && ballY <= rightPaddleY + paddleHeight))
        {
            ballVelocityX = -ballVelocityX;
            ballVelocityY += esp_random() % 5;
        }

        // Draw the updated game state
        tft->fillRect(messageAreaWidth, leftPaddleY, paddleWidth, paddleHeight, 0xf818);            // Left paddle
        tft->fillRect(tft->width() - paddleWidth, rightPaddleY, paddleWidth, paddleHeight, 0x07e6); // Right paddle
        tft->fillCircle(ballX, ballY, ballSize, 0xe7e0);                                            // Ball

        // Update the display
        tft->output();
    }
}

void drawScores(int leftScore, int rightScore)
{
    // Clear the score area
    tft->fillRoundRect(messageAreaWidth, 0, tft->width() - messageAreaWidth, scoreAreaHeight, 20, 0x17bb);

    // Set text properties and display scores
    tft->setTextColor(TFT_BLACK);
    tft->setFont(&Fun_Games20pt7b);
    tft->setCursor(messageAreaWidth + 3, 35);
    tft->print("Player 1: ");
    tft->print(leftScore);
    tft->setCursor(721, 35);
    tft->print("Player 2: ");
    tft->print(rightScore);
}

void drawMessage(const String &message)
{
    // Clear the message area
    tft->fillRect(0, 0, messageAreaWidth, tft->height(), TFT_BLACK);

    // Set text properties and display the message
    tft->setTextColor(TFT_WHITE);
    tft->setCursor(55, 125);
    tft->print(message);
}
