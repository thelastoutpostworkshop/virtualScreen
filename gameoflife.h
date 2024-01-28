#include "virtualScreen.h"

#include "fonts/BebasNeue_Regular13pt7b.h"

int gridWidth;  // Width of the grid
int gridHeight; // Height of the grid
bool **currentGrid;
bool **nextGrid;
const int squareSize = 10;
const int statsWidth = 240; // Width of the stats area on the left, adjust as needed
int aliveCellsCount = 0;
int changeRate = 0;
const uint16_t statsBgColor = 0x3186;
std::vector<int> populationHistory;

void setupGameOfLife(VirtualDisplay *gameTFT)
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
    populationHistory.push_back(newAliveCellsCount);
    changeRate = newChangeRate;
}

void drawGameOfLife(VirtualDisplay *gameTFT)
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
void drawPopulationGraph(VirtualDisplay *gameTFT)
{
    int graphWidth = statsWidth - 10;                  // Width of the graph, leaving some margin
    int graphHeight = 50;                              // Height of the graph, adjust as needed
    int graphX = 5;                                    // X position of the graph
    int graphY = gameTFT->height() - graphHeight - 10; // Y position, placed at the bottom
    int titleHeight = 15;                              // Height for the title area

    // Clear the area for the graph and title
    gameTFT->fillRect(graphX, graphY - titleHeight, graphWidth, graphHeight + titleHeight, statsBgColor);

    // Draw the title for the graph
    gameTFT->setTextColor(TFT_WHITE);
    gameTFT->setCursor(8, graphY - titleHeight); // Set position for the title
    gameTFT->print("Population Over Time");

    // Check if there is enough data to draw the graph
    if (populationHistory.size() < 2)
        return;

    // Calculate the scaling factor for the graph
    int maxPopulation = *std::max_element(populationHistory.begin(), populationHistory.end());
    float scaleY = static_cast<float>(graphHeight) / maxPopulation;
    float scaleX = static_cast<float>(graphWidth) / (populationHistory.size() - 1);

    // Draw the graph line
    for (size_t i = 0; i < populationHistory.size() - 1; ++i)
    {
        int x0 = graphX + i * scaleX;
        int y0 = graphY + graphHeight - populationHistory[i] * scaleY;
        int x1 = graphX + (i + 1) * scaleX;
        int y1 = graphY + graphHeight - populationHistory[i + 1] * scaleY;
        gameTFT->drawLine(x0, y0, x1, y1, TFT_YELLOW);
    }
}


void drawStats(int generation,VirtualDisplay *gameTFT)
{
    // Clear the stats area
    gameTFT->fillRect(0, 0, statsWidth, gameTFT->height(), statsBgColor);

    // Display the generation count
    gameTFT->setTextColor(TFT_WHITE);
    gameTFT->setFont(&BebasNeue_Regular13pt7b);
    gameTFT->setCursor(8, 25); // Adjust text position as needed
    gameTFT->print("Gen:");
    gameTFT->print(generation);
    gameTFT->setTextColor(TFT_WHITE);
    gameTFT->setCursor(8, 55); // Adjust position as needed
    gameTFT->print("Alive: ");
    gameTFT->print(aliveCellsCount);
    gameTFT->setCursor(8, 85); // Adjust for next stat position
    gameTFT->print("Change: ");
    gameTFT->print(changeRate);
}

void gameOfLife(VirtualDisplay *tft)
{
    tft->fillScreen(TFT_BLACK);
    setupGameOfLife(tft); // Initialize the game
    for (int i = 0; i < 1000; i++)
    {                       // Run for 1000 generations
        updateGameOfLife(); // Update the grid
        drawGameOfLife(tft);   // Draw the grid
        drawStats(i,tft);       // Draw the stats with the current generation
        drawPopulationGraph(tft);

        delay(100); // Delay between generations
    }
    cleanupGameOfLife(); // Clean up the dynamically allocated memory
}
