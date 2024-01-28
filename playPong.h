#include "virtualScreen.h"

const int messageAreaWidth = 240; // Width of the message area on the left
const int scoreAreaHeight = 50;  // Height of the score area at the top

void drawScores(VirtualDisplay *tft, int leftScore, int rightScore)
{
    // Clear the score area
    tft->fillRect(messageAreaWidth, 0, tft->width() - messageAreaWidth, scoreAreaHeight, TFT_BLACK);

    // Set text properties and display scores
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(2); // Adjust size as needed
    tft->setCursor(messageAreaWidth + 10, 5);
    tft->print("Left: ");
    tft->print(leftScore);
    tft->setCursor(tft->width() / 2 + 10, 5);
    tft->print("Right: ");
    tft->print(rightScore);
}

void drawMessage(VirtualDisplay *tft, const String &message)
{
    // Clear the message area
    tft->fillRect(0, 0, messageAreaWidth, tft->height(), TFT_BLACK);

    // Set text properties and display the message
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1); // Adjust size as needed
    tft->setCursor(5, 5);
    tft->print(message);
}

void playPong(VirtualDisplay *tft)
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

    for (int i = 0; i < 1000; i++)
    {
        // Clear the previous positions of the ball and paddles
        tft->fillRect(messageAreaWidth, leftPaddleY, paddleWidth, paddleHeight, TFT_BLACK); // Clear Left paddle
        tft->fillRect(tft->width() - paddleWidth, rightPaddleY, paddleWidth, paddleHeight, TFT_BLACK); // Clear Right paddle
        tft->fillCircle(ballX, ballY, ballSize, TFT_BLACK);

        // Update ball position
        ballX += ballVelocityX;
        ballY += ballVelocityY;

        // Check for ball collisions with the top and bottom of the screen
        if (ballY <= scoreAreaHeight || ballY >= tft->height() - ballSize)
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
        if ((ballX <= messageAreaWidth + paddleWidth && ballY >= leftPaddleY && ballY <= leftPaddleY + paddleHeight) ||
            (ballX >= tft->width() - paddleWidth - ballSize && ballY >= rightPaddleY && ballY <= rightPaddleY + paddleHeight))
        {
            ballVelocityX = -ballVelocityX;
            ballVelocityY += random(-2, 3); // Randomize the ball's Y velocity
        }

        // Draw the updated game state
        tft->fillRect(messageAreaWidth, leftPaddleY, paddleWidth, paddleHeight, 0xf818); // Left paddle
        tft->fillRect(tft->width() - paddleWidth, rightPaddleY, paddleWidth, paddleHeight, 0x07e6); // Right paddle
        tft->fillCircle(ballX, ballY, ballSize, 0xe7e0); // Ball

        drawScores(tft,10,10);
        drawMessage(tft,"Pong");
        // Update the display
        tft->output();
    }
}


