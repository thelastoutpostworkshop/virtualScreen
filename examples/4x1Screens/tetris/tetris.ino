// Screen Layout : 4 screens on a single row
// See the tutorial : 
//

#include "virtualScreen.h"

VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen layout
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});

    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();

    playTetris();
}

void loop()
{
}

std::vector<std::vector<bool>> updateGrid;

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
            clearLines();     // Call the line-clearing function
            spawnTetromino(); // Spawn a new tetromino
        }

        placeTetromino();
        drawGrid();

        tft->output(); // Assuming this function outputs the virtual display to the actual display
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
                int newX = currentTetromino.x + j;
                int newY = currentTetromino.y + i;

                if (newX >= 0 && newX < tetrisWidth && newY >= 0 && newY < tetrisHeight)
                {
                    grid[newX][newY] = currentTetromino.color;
                    updateGrid[newX][newY] = true; // Mark this cell for updating
                }
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
                    grid[newX][newY] = 0;          // Clear the cell
                    updateGrid[newX][newY] = true; // Mark this cell for updating
                }
            }
        }
    }
}

void setupTetris()
{
    tetrisWidth = tft->width() / blockSize;
    tetrisHeight = tft->height() / blockSize;
    grid = std::vector<std::vector<int>>(tetrisWidth, std::vector<int>(tetrisHeight, 0));
    updateGrid = std::vector<std::vector<bool>>(tetrisWidth, std::vector<bool>(tetrisHeight, true)); // Initially, all cells need to be drawn

    spawnTetromino();
}

// Modify drawGrid to only redraw cells that have changed
void drawGrid()
{
    for (int x = 0; x < tetrisWidth; x++)
    {
        for (int y = 0; y < tetrisHeight; y++)
        {
            if (updateGrid[x][y]) // Only update cells marked for redraw
            {
                int drawX = x * blockSize;
                int drawY = y * blockSize;
                uint16_t color = grid[x][y] ? grid[x][y] : TFT_BLACK;
                tft->fillRect(drawX, drawY, blockSize, blockSize, color);

                if (!grid[x][y]) // Optionally, redraw the grid lines for empty cells
                {
                    tft->drawRect(drawX, drawY, blockSize, blockSize, 0x4a89);
                }

                updateGrid[x][y] = false; // Reset the update flag
            }
        }
    }
}
