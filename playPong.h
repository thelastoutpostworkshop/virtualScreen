#include "virtualScreen.h"

void playPong(VirtualDisplay *tft)
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