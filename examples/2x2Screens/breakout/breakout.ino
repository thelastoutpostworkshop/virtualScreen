#include "virtualScreen.h"

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

    tft->fillScreen(TFT_BLACK);
    tft->output();
    setupGame();
}

void loop()
{
    updateGame();
    drawBricks();
    drawPaddle();
    drawBall();

    tft->output();
}

// Game settings
const int paddleWidth = 40;
const int paddleHeight = 15;
const int ballRadius = 10;
const int brickWidth = 40;
const int brickHeight = 20;
const int numRows = 7;
const int numCols = 10; // Adjust based on your display size

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
    ballY = prevBallY = tft->height() - 30; // Start the ball above the paddle
    memset(bricks, true, sizeof(bricks));   // Initialize all bricks as visible

    // Initially mark all previous bricks as not present to ensure they are drawn initially
    memset(prevBricks, false, sizeof(prevBricks)); // Force drawing of all bricks initially

    // Draw initial game state
    drawBricks(); // Draw all bricks initially
    drawPaddle();
    drawBall();
    tft->output(); // Update the display after initial setup
}

uint16_t calculateRowColor(int row, int totalRows) {
    // Define the midpoint for the transition from red to orange, then orange to green
    float midpoint = 0.5f;
    float progress = (float)row / (totalRows - 1); // Progress from 0.0 (top) to 1.0 (bottom)

    uint8_t red, green;

    // Calculate the RGB components based on the row's position
    if (progress <= midpoint) {
        // Transition from red to orange in the first half
        red = 31; // Maximum red value for the entire first half
        green = (progress / midpoint) * 31; // Scale green up to 31 (to get orange) at the midpoint
    } else {
        // Transition from orange to green in the second half
        red = (1.0f - (progress - midpoint) / midpoint) * 31; // Scale red down to 0 past the midpoint
        green = 31 + ((progress - midpoint) / midpoint) * (63 - 31); // Continue scaling green from 31 to 63
    }

    // Combine the components into a 16-bit color value
    return (red << 11) | (green << 5);
}

void drawPaddle()
{
    // Clear the previous paddle position
    tft->fillRect(prevPaddleX, tft->height() - paddleHeight - 1, paddleWidth, paddleHeight, TFT_BLACK);
    // Draw the new paddle position
    tft->fillRect(paddleX, tft->height() - paddleHeight - 1, paddleWidth, paddleHeight, TFT_CYAN);
    prevPaddleX = paddleX; // Update the previous paddle position
}

void drawBall()
{
    // Clear the previous ball position
    tft->fillCircle(prevBallX, prevBallY, ballRadius, TFT_BLACK);
    // Draw the new ball position
    tft->fillCircle(ballX, ballY, ballRadius, TFT_YELLOW);
    prevBallX = ballX; // Update the previous ball position
    prevBallY = ballY;
}

void drawBricks() {
    for (int row = 0; row < numRows; ++row) {
        // Calculate the color for the current row
        uint16_t rowColor = calculateRowColor(row, numRows);
        
        for (int col = 0; col < numCols; ++col) {
            // Draw only if the brick status has changed
            if (bricks[row][col] != prevBricks[row][col]) {
                int x = col * (brickWidth + 5);
                int y = row * (brickHeight + 5);
                if (bricks[row][col]) {
                    tft->fillRect(x, y, brickWidth, brickHeight, rowColor); // Use the row-specific color
                } else {
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

    // Collision detection with the paddle
    int paddleTopY = tft->height() - paddleHeight - 1;     // Y position of the top of the paddle
    if (ballY + ballVelocityY > paddleTopY - ballRadius && // Check if moving past the paddle's top edge
        ballY < paddleTopY + paddleHeight &&               // And is within paddle's height range
        ballX + ballVelocityX > paddleX - ballRadius &&    // Check if within paddle's left edge
        ballX + ballVelocityX < paddleX + paddleWidth + ballRadius)
    {                                    // And within paddle's right edge
        ballVelocityY = -ballVelocityY;  // Reverse Y direction
        ballY = paddleTopY - ballRadius; // Adjust ball position to avoid sticking or multiple collisions
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

                // Expanded collision detection area to account for the ball's radius
                if (ballX + ballRadius > brickX && ballX - ballRadius < brickX + brickWidth &&
                    ballY + ballRadius > brickY && ballY - ballRadius < brickY + brickHeight)
                {

                    // Determine collision direction
                    float middleX = brickX + brickWidth / 2;
                    float middleY = brickY + brickHeight / 2;
                    float deltaX = ballX - middleX;
                    float deltaY = ballY - middleY;

                    // Remove the brick
                    bricks[row][col] = false;

                    // Determine if collision is more horizontal or vertical
                    // and reverse the appropriate velocity component
                    if (fabs(deltaX) > fabs(deltaY))
                    {
                        if (ballX < middleX)
                        { // Hit was from the left
                            ballVelocityX = -abs(ballVelocityX);
                        }
                        else
                        { // Hit was from the right
                            ballVelocityX = abs(ballVelocityX);
                        }
                    }
                    else
                    {
                        if (ballY < middleY)
                        { // Hit was from above
                            ballVelocityY = -abs(ballVelocityY);
                        }
                        else
                        { // Hit was from below
                            ballVelocityY = abs(ballVelocityY);
                        }
                    }
                }
            }
        }
    }
}
