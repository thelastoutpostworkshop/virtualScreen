#ifndef _VIRTUAL_SCREEN_
#include "virtualScreen.h"
#endif
#include <vector>
#include <stack>
#include <utility> // For std::pair
#include <random>

VirtualDisplay *mazeTFT;

struct Cell
{
    bool visited = false;
    bool topWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool rightWall = true;
};

const int cellSize = 30; // Global cell size in pixels

std::vector<std::vector<Cell>> maze;
int mazeWidth;  // Maze width in cells
int mazeHeight; // Maze height in cells

void generateMaze()
{
    std::stack<std::pair<int, int>> stack;
    int currentX = 0;
    int currentY = 0;
    maze[currentY][currentX].visited = true;
    stack.push({currentX, currentY});

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        std::vector<std::pair<int, int>> unvisitedNeighbors;

        // Check unvisited neighbors
        if (x > 0 && !maze[y][x - 1].visited)
            unvisitedNeighbors.push_back({x - 1, y});
        if (y > 0 && !maze[y - 1][x].visited)
            unvisitedNeighbors.push_back({x, y - 1});
        if (x < mazeWidth - 1 && !maze[y][x + 1].visited)
            unvisitedNeighbors.push_back({x + 1, y});
        if (y < mazeHeight - 1 && !maze[y + 1][x].visited)
            unvisitedNeighbors.push_back({x, y + 1});

        if (!unvisitedNeighbors.empty())
        {
            // Randomly select a neighbor
            auto [nextX, nextY] = unvisitedNeighbors[rand() % unvisitedNeighbors.size()];

            // Remove the wall between the current cell and the chosen cell
            if (nextX == x + 1)
            {
                maze[y][x].rightWall = false;
                maze[nextY][nextX].leftWall = false;
            }
            // ... (handle other directions)

            maze[nextY][nextX].visited = true;
            stack.push({nextX, nextY});
        }
        else
        {
            stack.pop();
        }
    }
}
void drawMaze(VirtualDisplay *display)
{
    for (int y = 0; y < mazeHeight; y++)
    {
        for (int x = 0; x < mazeWidth; x++)
        {
            // Calculate the top left corner of the cell
            int x0 = x * cellSize;
            int y0 = y * cellSize;

            // Draw the walls of the cell
            if (maze[y][x].topWall)
            {
                display->drawLine(x0, y0, x0 + cellSize, y0, TFT_WHITE); // Top wall
            }
            if (maze[y][x].bottomWall)
            {
                display->drawLine(x0, y0 + cellSize, x0 + cellSize, y0 + cellSize, TFT_WHITE); // Bottom wall
            }
            if (maze[y][x].leftWall)
            {
                display->drawLine(x0, y0, x0, y0 + cellSize, TFT_WHITE); // Left wall
            }
            if (maze[y][x].rightWall)
            {
                display->drawLine(x0 + cellSize, y0, x0 + cellSize, y0 + cellSize, TFT_WHITE); // Right wall
            }
        }
    }

    display->output(); // Refresh the display with the drawn maze
}

void setupMaze()
{

    // Define the size of the maze
    mazeWidth = mazeTFT->width() / cellSize;   // Number of cells horizontally
    mazeHeight = mazeTFT->height() / cellSize; // Number of cells vertically

    // Allocate memory for the maze grid
    maze.resize(mazeHeight, std::vector<Cell>(mazeWidth));

    // Generate the maze
    generateMaze(); // Implement this function as per the earlier discussion

    // Draw the maze
    drawMaze(mazeTFT); // Implement this function to draw the maze on the display
}

void test_maze(VirtualDisplay *tft)
{
    mazeTFT = tft;
    mazeTFT->fillScreen(TFT_BLACK);
    setupMaze();
}
