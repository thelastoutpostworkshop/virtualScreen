#include "src/virtualScreen.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen configuration
    screens.addRow({{6, 0}, {7, 0}});
    screens.addRow({{16, 2}, {15, 2}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();

    tft->fillScreen(TFT_WHITE);
    tft->output();
    // setupGame();
}

void loop()
{
    // Update and draw game elements
    // updateGame();
    // drawBricks();
    // drawPaddle();
    // drawBall();

    // // Refresh the display
    // tft->output();
}

// Game settings
const int paddleWidth = 30;
const int paddleHeight = 5;
const int ballRadius = 10;
const int brickWidth = 20;
const int brickHeight = 10;
const int numRows = 4;
const int numCols = 8; // Adjust based on your display size

// Game state
int paddleX;
int ballX, ballY;
int ballVelocityX = 10, ballVelocityY = -10;
bool bricks[numRows][numCols]; // True if the brick is still there

int prevPaddleX;
int prevBallX, prevBallY;
bool prevBricks[numRows][numCols]; // To track changes in bricks

// Initialize game state
void setupGame()
{
    paddleX = prevPaddleX = (tft->width() - paddleWidth) / 2;
    ballX = prevBallX = tft->width() / 2;
    ballY = prevBallY = tft->height() - 30;
    memset(bricks, true, sizeof(bricks));
    memcpy(prevBricks, bricks, sizeof(bricks)); // Initialize all bricks as visible and track them
}

void drawPaddle()
{
    // Clear the previous paddle position
    tft->fillRect(prevPaddleX, tft->height() - paddleHeight - 1, paddleWidth, paddleHeight, TFT_BLACK);
    // Draw the new paddle position
    tft->fillRect(paddleX, tft->height() - paddleHeight - 1, paddleWidth, paddleHeight, TFT_WHITE);
    prevPaddleX = paddleX; // Update the previous paddle position
}

void drawBall()
{
    // Clear the previous ball position
    tft->fillCircle(prevBallX, prevBallY, ballRadius, TFT_BLACK);
    // Draw the new ball position
    tft->fillCircle(ballX, ballY, ballRadius, TFT_RED);
    prevBallX = ballX; // Update the previous ball position
    prevBallY = ballY;
}

void drawBricks()
{
    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols; ++col)
        {
            // Draw only if the brick status has changed
            if (bricks[row][col] != prevBricks[row][col])
            {
                int x = col * (brickWidth + 5);
                int y = row * (brickHeight + 5);
                if (bricks[row][col])
                {
                    tft->fillRect(x, y, brickWidth, brickHeight, TFT_BLUE);
                }
                else
                {
                    tft->fillRect(x, y, brickWidth, brickHeight, TFT_BLACK); // Clear the brick if it was hit
                }
                prevBricks[row][col] = bricks[row][col]; // Update the tracked brick status
            }
        }
    }
}

void updateGame()
{
    // Move the paddle automatically by following the ball
    paddleX = ballX - paddleWidth / 2;

    // Update ball position
    ballX += ballVelocityX;
    ballY += ballVelocityY;

    // Collision detection with the walls
    if (ballX <= 0 || ballX >= tft->width())
        ballVelocityX = -ballVelocityX;
    if (ballY <= 0)
        ballVelocityY = -ballVelocityY;
    // Collision with the paddle
    if (ballY >= tft->height() - paddleHeight - ballRadius && ballX >= paddleX && ballX <= paddleX + paddleWidth)
    {
        ballVelocityY = -ballVelocityY; // Reverse direction
    }

    // Collision detection with bricks
    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols; ++col)
        {
            if (bricks[row][col])
            { // If the brick is there
                int brickX = col * (brickWidth + 5);
                int brickY = row * (brickHeight + 5);
                // Simple collision detection
                if (ballX >= brickX && ballX <= brickX + brickWidth &&
                    ballY >= brickY && ballY <= brickY + brickHeight)
                {
                    bricks[row][col] = false;       // Remove the brick
                    ballVelocityY = -ballVelocityY; // Bounce the ball
                }
            }
        }
    }
}
