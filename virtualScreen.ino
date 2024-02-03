#include "virtualScreen.h"

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

    playTetris();
}

void loop()
{
    // Your loop can remain empty if everything is handled in runBouncingBall
}

// Define Tetromino shapes
const int I[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

const int O[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};

const int T[4][4] = {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

const int S[4][4] = {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

const int Z[4][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

const int J[4][4] = {
    {1, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

const int L[4][4] = {
    {0, 0, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

// Define colors for each tetromino shape
const uint16_t tetrominoColors[7] = {
    TFT_RED,    // Color for shape I
    TFT_GREEN,  // Color for shape O
    TFT_BLUE,   // Color for shape T
    TFT_YELLOW, // Color for shape S
    TFT_PURPLE, // Color for shape Z
    TFT_ORANGE, // Color for shape J
    TFT_CYAN    // Color for shape L
};

const int blockSize = 20; // Size of each Tetris block in pixels

const int (*tetrominoes[7])[4] = {I, O, T, S, Z, J, L};

int tetrisWidth;                    // Width of the Tetris grid, set in setupTetris
int tetrisHeight;                   // Height of the Tetris grid, set in setupTetris
std::vector<std::vector<int>> grid; // Game grid

struct Tetromino
{
    int x, y;        // Position
    int shape[4][4]; // Shape
    uint16_t color;  // Color
} currentTetromino;


void playTetris()
{
    setupTetris();

    while (true)
    {
        clearTetrominoPosition();

        // Attempt to move the tetromino down by incrementing its x position
        if (canPlace(currentTetromino.x + 1, currentTetromino.y, currentTetromino.shape))
        {
            currentTetromino.x++;
        }
        else
        {
            // If the tetromino cannot move down, place it on the grid
            placeTetromino();
            // clearLines(); // Call the line-clearing function
            spawnTetromino(); // Spawn a new tetromino
        }

        placeTetromino();
        // Draw the grid and current tetromino
        drawGrid();

        tft->output();
    }
}

void clearLines()
{
    for (int row = tetrisHeight - 1; row >= 0; row--)
    {
        bool rowFilled = true;

        // Check if the row is filled
        for (int col = 0; col < tetrisWidth; col++)
        {
            if (grid[row][col] == 0)
            {
                rowFilled = false;
                break;
            }
        }

        // Clear the row if it's filled
        if (rowFilled)
        {
            for (int y = row; y > 0; y--)
            {
                for (int x = 0; x < tetrisWidth; x++)
                {
                    grid[y][x] = grid[y - 1][x];
                }
            }

            // Set the top row to empty
            for (int x = 0; x < tetrisWidth; x++)
            {
                grid[0][x] = 0;
            }

            // Since we moved the rows down, we need to check the same row again
            row++;
        }
    }
}

// Initialize a new tetromino at the top of the grid
void spawnTetromino()
{
    int shapeIndex = esp_random() % 7; // Random index for shape
    memcpy(currentTetromino.shape, tetrominoes[shapeIndex], sizeof(currentTetromino.shape));
    currentTetromino.x = 0; // Start from the left side (which is now the top)

    // Choose a random y position within the grid's width
    // Subtract 4 to ensure the entire tetromino shape fits within the grid
    currentTetromino.y = esp_random() % (tetrisHeight - 4);

    currentTetromino.color = tetrominoColors[shapeIndex];
}

// Check if the tetromino can be placed at a position in the grid
bool canPlace(int x, int y, int shape[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j])
            {
                int newX = x + j; // newX represents vertical movement in the rotated grid
                int newY = y + i; // newY represents horizontal movement

                // Check for out-of-bounds or collision with existing blocks
                if (newX < 0 || newX >= tetrisWidth || newY < 0 || newY >= tetrisHeight || grid[newX][newY])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// Place the tetromino on the grid
void placeTetromino()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (currentTetromino.shape[i][j])
            {
                int newX = currentTetromino.x + j; // Calculate the correct x position
                int newY = currentTetromino.y + i; // Calculate the correct y position

                // Check if the calculated position is within the bounds of the grid
                if (newX >= 0 && newX < tetrisWidth && newY >= 0 && newY < tetrisHeight)
                    grid[newX][newY] = currentTetromino.color;
            }
        }
    }
}

void setupTetris()
{
    tetrisWidth = tft->width() / blockSize;   // Use the full width of the tft
    tetrisHeight = tft->height() / blockSize; // Height based on the tft height
    grid = std::vector<std::vector<int>>(tetrisWidth, std::vector<int>(tetrisHeight, 0));

    spawnTetromino();
}

void drawGrid()
{
    for (int x = 0; x < tetrisWidth; x++)
    {
        for (int y = 0; y < tetrisHeight; y++)
        {
            int drawX = x * blockSize; // Corresponds to tft's width
            int drawY = y * blockSize; // Corresponds to tft's height

            if (grid[x][y])
            {
                tft->fillRect(drawX, drawY, blockSize, blockSize, grid[x][y]);
            }
            else
            {
                tft->fillRect(drawX, drawY, blockSize, blockSize, TFT_BLACK);
                tft->drawRect(drawX, drawY, blockSize, blockSize, 0x4a89);
            }
        }
    }
}

void clearTetrominoPosition()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (currentTetromino.shape[i][j])
            {
                int newX = currentTetromino.x + j;
                int newY = currentTetromino.y + i;

                if (newX >= 0 && newX < tetrisWidth && newY >= 0 && newY < tetrisHeight)
                {
                    grid[newX][newY] = 0; // Clear the cell
                }
            }
        }
    }
}

