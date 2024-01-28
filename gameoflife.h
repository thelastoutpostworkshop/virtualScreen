#ifndef _VIRTUAL_SCREEN_
#include "virtualScreen.h"
#endif
#ifndef _TFT_eSPIH_
#include <TFT_eSPI.h>
#endif
const int gridWidth = 64;  // Width of the grid
const int gridHeight = 48; // Height of the grid
bool currentGrid[gridHeight][gridWidth];
bool nextGrid[gridHeight][gridWidth];
VirtualDisplay *gameTFT;

void setupGameOfLife()
{
    // Randomly initialize the grid
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            currentGrid[y][x] = random(2); // Randomly alive or dead
        }
    }
}

void updateGameOfLife()
{
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
                        continue; // Skip the current cell
                    int nx = x + j;
                    int ny = y + i;
                    if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight && currentGrid[ny][nx])
                    {
                        aliveNeighbors++;
                    }
                }
            }

            // Apply the rules of the Game of Life
            if (currentGrid[y][x] && (aliveNeighbors < 2 || aliveNeighbors > 3))
            {
                nextGrid[y][x] = false; // Cell dies
            }
            else if (!currentGrid[y][x] && aliveNeighbors == 3)
            {
                nextGrid[y][x] = true; // Cell becomes alive
            }
            else
            {
                nextGrid[y][x] = currentGrid[y][x]; // State remains the same
            }
        }
    }

    // Copy nextGrid to currentGrid for the next iteration
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            currentGrid[y][x] = nextGrid[y][x];
        }
    }
}

void drawGameOfLife()
{
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            gameTFT->drawPixel(x, y, currentGrid[y][x] ? TFT_WHITE : TFT_BLACK);
        }
    }
    output();
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
        delay(100);         // Delay between generations
    }
}