#ifndef _VIRTUAL_SCREEN_
#include "virtualScreen.h"
#endif
#include <vector>
#include <stack>
#include <utility> // For std::pair

VirtualDisplay *mazeTFT;

struct Cell
{
    bool visited = false;
    bool topWall = true;
    bool bottomWall = true;
    bool leftWall = true;
    bool rightWall = true;
};

const int cellSize = 30;   // Global cell size in pixels
const int paddingSize = 5; // Total padding in pixels, adjust as needed

std::vector<std::vector<Cell>> maze;
int mazeWidth;  // Maze width in cells
int mazeHeight; // Maze height in cells

std::vector<std::pair<int, int>> solveMaze()
{
    std::stack<std::pair<int, int>> stack;
    std::vector<std::vector<bool>> visited(mazeHeight, std::vector<bool>(mazeWidth, false));
    std::vector<std::pair<int, int>> path;

    // Start from an edge, for example, (0, 0)
    stack.push({0, 0});
    visited[0][0] = true;

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        path.push_back({x, y}); // Add to path

        // Check if the center is reached
        if (x == mazeWidth / 2 && y == mazeHeight / 2)
        {
            return path; // Return the path to the center
        }

        // Find unvisited neighbors without walls in between
        std::vector<std::pair<int, int>> unvisitedNeighbors;
        if (x > 0 && !visited[y][x - 1] && !maze[y][x].leftWall)
        {
            unvisitedNeighbors.push_back({x - 1, y});
        }
        if (y > 0 && !visited[y - 1][x] && !maze[y][x].topWall)
        {
            unvisitedNeighbors.push_back({x, y - 1});
        }
        if (x < mazeWidth - 1 && !visited[y][x + 1] && !maze[y][x].rightWall)
        {
            unvisitedNeighbors.push_back({x + 1, y});
        }
        if (y < mazeHeight - 1 && !visited[y + 1][x] && !maze[y][x].bottomWall)
        {
            unvisitedNeighbors.push_back({x, y + 1});
        }

        if (!unvisitedNeighbors.empty())
        {
            auto [nextX, nextY] = unvisitedNeighbors.back();
            visited[nextY][nextX] = true;
            stack.push({nextX, nextY});
        }
        else
        {
            stack.pop();
            path.pop_back(); // Remove from path if it's a dead end
        }
    }
    return path; // In case the center can't be reached, return the attempted path
}

void generateMaze()
{
    std::stack<std::pair<int, int>> stack;

    // Start from the center of the maze
    int centerX = mazeWidth / 2;
    int centerY = mazeHeight / 2;
    maze[centerY][centerX].visited = true;
    stack.push({centerX, centerY});

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        std::vector<std::pair<int, int>> unvisitedNeighbors;

        // Check unvisited neighbors
        if (x > 0 && !maze[y][x - 1].visited)
        {
            unvisitedNeighbors.push_back({x - 1, y});
        }
        if (y > 0 && !maze[y - 1][x].visited)
        {
            unvisitedNeighbors.push_back({x, y - 1});
        }
        if (x < mazeWidth - 1 && !maze[y][x + 1].visited)
        {
            unvisitedNeighbors.push_back({x + 1, y});
        }
        if (y < mazeHeight - 1 && !maze[y + 1][x].visited)
        {
            unvisitedNeighbors.push_back({x, y + 1});
        }

        if (!unvisitedNeighbors.empty())
        {
            // Randomly select a neighbor
            auto [nextX, nextY] = unvisitedNeighbors[esp_random() % unvisitedNeighbors.size()];

            // Remove the wall between the current cell and the chosen cell
            if (nextX == x + 1)
            {
                maze[y][x].rightWall = false;
                maze[nextY][nextX].leftWall = false;
            }
            else if (nextX == x - 1)
            {
                maze[y][x].leftWall = false;
                maze[nextY][nextX].rightWall = false;
            }
            else if (nextY == y + 1)
            {
                maze[y][x].bottomWall = false;
                maze[nextY][nextX].topWall = false;
            }
            else if (nextY == y - 1)
            {
                maze[y][x].topWall = false;
                maze[nextY][nextX].bottomWall = false;
            }

            maze[nextY][nextX].visited = true;
            stack.push({nextX, nextY});
        }
        else
        {
            stack.pop();
        }
    }
}
void drawMaze(VirtualDisplay *display, const std::vector<std::pair<int, int>> &path)
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

    // Draw the solution path in yellow
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        // Coordinates of the current cell
        int x0 = path[i].first * cellSize + cellSize / 2;
        int y0 = path[i].second * cellSize + cellSize / 2;

        // Coordinates of the next cell
        int x1 = path[i + 1].first * cellSize + cellSize / 2;
        int y1 = path[i + 1].second * cellSize + cellSize / 2;

        // Check if the next cell is a direct neighbor of the current cell
        bool isNeighbor = (path[i].first == path[i + 1].first && abs(path[i].second - path[i + 1].second) == 1) ||
                          (path[i].second == path[i + 1].second && abs(path[i].first - path[i + 1].first) == 1);

        // Draw line only if the next cell is a neighbor
        if (isNeighbor)
        {
            display->drawLine(x0, y0, x1, y1, TFT_YELLOW);
            display->output();
        }
    }
    if (!path.empty())
    {
        // Get the coordinates of the last cell in the path
        auto [lastX, lastY] = path.back();
        int centerX = lastX * cellSize + cellSize / 2;
        int centerY = lastY * cellSize + cellSize / 2;

        // Draw a yellow circle at the end of the path
        int radius = cellSize / 4; // Radius of the circle
        display->fillCircle(centerX, centerY, radius, TFT_YELLOW);
    }

    display->output();
}

void setupMaze()
{
    // Adjusted dimensions to account for padding
    mazeWidth = (mazeTFT->width() - paddingSize) / cellSize;   // Ensure odd number
    mazeHeight = (mazeTFT->height() - paddingSize) / cellSize; // Ensure odd number

    // Ensure dimensions are odd numbers
    if (mazeWidth % 2 == 0)
    {
        mazeWidth--;
    }
    if (mazeHeight % 2 == 0)
    {
        mazeHeight--;
    }

    // Allocate memory for the maze grid
    maze.resize(mazeHeight, std::vector<Cell>(mazeWidth));

    // Generate and draw the maze
    generateMaze();
    auto path = solveMaze();

    drawMaze(mazeTFT, path);
}

void test_maze(VirtualDisplay *tft)
{
    mazeTFT = tft;
    mazeTFT->fillScreen(TFT_BLACK);
    setupMaze();
}
