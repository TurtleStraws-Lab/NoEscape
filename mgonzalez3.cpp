#include "mgonzalez3.h"
#include <GL/gl.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <ctime>


using namespace std;
void Maze::generate(int w, int h) {
    width = w;
    height = h;

    // Allocate and initialize grid with walls (0)
    grid = new int*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new int[width];
        for (int j = 0; j < width; ++j)
            grid[i][j] = 0; //initalize all cells as walls
    }
    static std::mt19937 g(static_cast<unsigned int>(std::time(nullptr)));
    carveMaze(1, 1); // Start carving from cell (1,1)
}

void Maze::carveMaze(int x, int y) {
    static const int DX[4] = { 0, 0, -1, 1};
    static const int DY[4] = { -1, 1, 0, 0};
    std::vector<int> dirs = { 0, 1, 2, 3 };

    std::random_shuffle(dirs.begin(), dirs.end());

    grid[y][x] = 1;

    for (int i : dirs) {
        int nx = x + DX[i] * 2;  
        //calculates coordinates of the cell two steps away... 
        //checking if unvisited and carves if so
        int ny = y + DY[i] * 2;   

        if (ny > 0 && ny < height - 1 && nx > 0 && nx < width - 1 && grid[ny][nx] == 0) {
            grid[y + DY[i]][x + DX[i]] = 1; // carve wall between
            carveMaze(nx, ny);
        }
    }
}

void Maze::render() {

    float cellSize = 14.5;
    glPushMatrix();
    glTranslatef((-width * cellSize / 400.0) - 4.0f, (-height * cellSize / 25.0f) + 20.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f); // White lines

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == 0) { // draw wall cell
                float xpos = x * cellSize; 
                //- (width * cellSize / 2);
                float ypos = (height - y - 1) * cellSize;
                // - (height * cellSize / 2);
                glBegin(GL_LINE_LOOP);
                glVertex2f(xpos, ypos);
                glVertex2f(xpos + cellSize, ypos);
                glVertex2f(xpos + cellSize, ypos + cellSize);
                glVertex2f(xpos, ypos + cellSize);
                glEnd();
            }
        }

    }

    glPopMatrix();
}
void mgonzalez3::darkBack(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);

    glClear(GL_COLOR_BUFFER_BIT);
}


void mgonzalez3::lighting(float pos1, float pos2, float area) {
    //darkBack(0.0f, 0.0f, 0.0f, 1.0f); 
    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    glColor4f(1.0f, 1.0f, 0.0f, 0.1f);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(pos1, pos2);

    int circle = 100;

    for (int i = 0; i <= circle; i++) {
        float angle = (i / (float)circle) * 4.0f * M_PI;

        float x = pos1 + cos(angle) * area;
        float y = pos2 + sin(angle) * area;

        glVertex2f(x, y);
    }

    glEnd();

    glPopMatrix();
}

void Coin::render(int xres, int yres, float area) {
    //darkBack(0.0f, 0.0f, 0.0f, 1.0f); 
    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(xres, yres);

    int circle = 100;

    for (int i = 0; i <= circle; i++) {
        float angle = (i / (float)circle) * 2.0f * M_PI;

        float x = xres + cos(angle) * area;
        float y = yres + sin(angle) * area;
        glVertex2f(x, y);
    }


    glEnd();

    glPopMatrix();
}

bool coin1 = false;
bool coin2 = false;
bool coin3 = false;
bool coin4 = false;
int coinsCollected = 0;

void Coin::coinCollect(int xres1, int xres2, int xres3, int xres4, int yres1, int yres2, int yres3, int yres4, float pos1, float pos2)
{
    Coin coin;
    if (abs(pos1 - xres1) <= 10 && abs(pos2 - yres1) <= 10 && !coin1) {
        // coins disappear two at a time for some reason 
        if (!coin1) {
            coinsCollected++;
        }
        coin1 = true;
    } else {
        if(!coin1) {
            coin.render(xres1, yres1, 5.0f);
        }
    }
    if (abs(pos1 - xres2) <= 10 && abs(pos2 - yres2) <= 10 && !coin2) {
        if (!coin2) {
            coinsCollected++;
        }
        coin2 = true;
    } else {
        if (!coin2) {
            coin.render(xres2, yres2, 5.0f);
        }
    }
    if (abs(pos1 - xres3) <= 10 && abs(pos2 - yres3) <= 10 && !coin3) {
        if (!coin3) {
            coinsCollected++;
        }
        coin3 = true;
    } else {
        if (!coin3) {
            coin.render(xres3, yres3, 5.0f);
        }
    }
    if (abs(pos1 - xres4) <= 10 && abs(pos2 - yres4) <= 10 && !coin4) {
        if (!coin4) {
            coinsCollected++;
        }
        coin4 = true;
    } else {
        if (!coin4) {
            coin.render(xres4, yres4, 5.0f);
        }
    }

}

bool Maze::isWall(float x, float y, float cellSize) {
    int cellX = x / cellSize;
    int cellY = (height - 1) - (y / cellSize);


    return grid[cellY][cellX] == 0;
}





