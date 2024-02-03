#include "virtualScreen.h"
#include "fonts/BebasNeue_Regular13pt7b.h"

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

    gameOfLife();
}

void loop()
{
    // Your loop can remain empty if everything is handled in runBouncingBall
}

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

void gameOfLife()
{
    tft->fillScreen(TFT_BLACK);
    setupGameOfLife(); // Initialize the game
    for (int i = 0; i < 1000; i++)
    {                       // Run for 1000 generations
        updateGameOfLife(); // Update the grid
        drawGameOfLife();   // Draw the grid
        drawStats(i);       // Draw the stats with the current generation
        drawPopulationGraph();

        delay(100); // Delay between generations
    }
    cleanupGameOfLife(); // Clean up the dynamically allocated memory
}

void setupGameOfLife()
{
    gridWidth = (tft->width() - statsWidth) / squareSize; // Adjusted for stats area
    gridHeight = tft->height() / squareSize;

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

void drawGameOfLife()
{
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            uint16_t color = currentGrid[y][x] ? 0x07e0 : TFT_BLACK;
            tft->fillRect(x * squareSize + statsWidth, y * squareSize, squareSize, squareSize, color);
        }
    }
    tft->output();
}
void drawPopulationGraph()
{
    int graphWidth = statsWidth - 10;              // Width of the graph, leaving some margin
    int graphHeight = 50;                          // Height of the graph, adjust as needed
    int graphX = 5;                                // X position of the graph
    int graphY = tft->height() - graphHeight - 10; // Y position, placed at the bottom
    int titleHeight = 15;                          // Height for the title area

    // Clear the area for the graph and title
    tft->fillRect(graphX, graphY - titleHeight, graphWidth, graphHeight + titleHeight, statsBgColor);

    // Draw the title for the graph
    tft->setTextColor(0x07fb);
    tft->setCursor(8, graphY - titleHeight); // Set position for the title
    tft->print("Population Over Time");

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
        tft->drawLine(x0, y0, x1, y1, TFT_YELLOW);
    }
}

void drawStats(int generation)
{
    // Clear the stats area
    tft->fillRect(0, 0, statsWidth, tft->height(), statsBgColor);

    tft->setTextColor(0x07fb);
    tft->setFont(&BebasNeue_Regular13pt7b);
    tft->setCursor(8, 30); 
    tft->print("Generation:");
    tft->setTextColor(TFT_WHITE);
    tft->print(generation);
    tft->setTextColor(0x07fb);
    tft->setCursor(8, 60); 
    tft->print("Alive: ");
    tft->setTextColor(TFT_WHITE);
    tft->print(aliveCellsCount);
    tft->setCursor(8, 90); 
    tft->setTextColor(0x07fb);
    tft->print("Change: ");
    tft->setTextColor(TFT_WHITE);
    tft->print(changeRate);
}
