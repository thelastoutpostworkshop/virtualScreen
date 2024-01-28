#ifndef _VIRTUAL_SCREEN_
#include "virtualScreen.h"
#endif

int gridWidth;  // Width of the grid
int gridHeight; // Height of the grid
bool **currentGrid;
bool **nextGrid;
const int squareSize = 10;
const int statsWidth = 175; // Width of the stats area on the left, adjust as needed
int aliveCellsCount = 0;
int changeRate = 0;

VirtualDisplay *gameTFT;

void setupGameOfLife()
{
    gridWidth = (gameTFT->width() - statsWidth) / squareSize; // Adjusted for stats area
    gridHeight = gameTFT->height() / squareSize;

    // Allocate memory for the grids
    currentGrid = new bool *[gridHeight];
    nextGrid = new bool *[gridHeight];
    for (int y = 0; y < gridHeight; y++)
    {
        currentGrid[y] = new bool[gridWidth];
        nextGrid[y] = new bool[gridWidth];
    }

    // Randomly initialize the currentGrid
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            currentGrid[y][x] = esp_random() % 2; // Randomly alive or dead
        }
    }
}

void cleanupGameOfLife()
{
    for (int y = 0; y < gridHeight; y++)
    {
        delete[] currentGrid[y];
        delete[] nextGrid[y];
    }
    delete[] currentGrid;
    delete[] nextGrid;
}

void updateGameOfLife()
{
    int newAliveCellsCount = 0;
    int newChangeRate = 0;

    // Apply Game of Life rules to update the grid
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            int aliveNeighbors = 0;

            // Count alive neighbors
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0)
                    {
                        continue; // Skip the current cell
                    }
                    int nx = x + j;
                    int ny = y + i;
                    if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight && currentGrid[ny][nx])
                    {
                        aliveNeighbors++;
                    }
                }
            }

            // Apply the rules of the Game of Life
            bool newState = currentGrid[y][x];
            if (currentGrid[y][x] && (aliveNeighbors < 2 || aliveNeighbors > 3))
            {
                newState = false; // Cell dies
            }
            else if (!currentGrid[y][x] && aliveNeighbors == 3)
            {
                newState = true; // Cell becomes alive
            }

            if (newState != currentGrid[y][x])
            {
                newChangeRate++; // Increment change rate if cell state changes
            }

            if (newState)
            {
                newAliveCellsCount++; // Count alive cells
            }

            nextGrid[y][x] = newState;
        }
    }

    // Copy nextGrid to currentGrid for the next iteration and update stats
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            currentGrid[y][x] = nextGrid[y][x];
        }
    }

    aliveCellsCount = newAliveCellsCount;
    changeRate = newChangeRate;
}

void drawGameOfLife()
{
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            uint16_t color = currentGrid[y][x] ? TFT_WHITE : TFT_BLACK;
            gameTFT->fillRect(x * squareSize + statsWidth, y * squareSize, squareSize, squareSize, color);
        }
    }
    gameTFT->output();
}

void drawStats(int generation)
{
    // Clear the stats area
    gameTFT->fillRect(0, 0, statsWidth, gameTFT->height(), TFT_BLACK);

    // Display the generation count
    gameTFT->setTextColor(TFT_WHITE);
    gameTFT->setTextSize(2);
    gameTFT->setCursor(5, 5); // Adjust text position as needed
    gameTFT->print("Gen:");
    gameTFT->print(generation);
    gameTFT->setTextColor(TFT_WHITE);
    gameTFT->setCursor(5, 25); // Adjust position as needed
    gameTFT->print("Alive: ");
    gameTFT->print(aliveCellsCount);
    gameTFT->setCursor(5, 45); // Adjust for next stat position
    gameTFT->print("Change: ");
    gameTFT->print(changeRate);
}

void test_gameOfLife(VirtualDisplay *tft)
{
    gameTFT = tft;
    gameTFT->fillScreen(TFT_BLACK);
    setupGameOfLife(); // Initialize the game
    for (int i = 0; i < 1000; i++)
    {                       // Run for 1000 generations
        updateGameOfLife(); // Update the grid
        drawGameOfLife();   // Draw the grid
        drawStats(i);       // Draw the stats with the current generation

        delay(100); // Delay between generations
    }
    cleanupGameOfLife(); // Clean up the dynamically allocated memory
}
