#ifndef MGONZALEZ3_H
#define MGONZALEZ3_H

extern int coinsCollected;

class Maze {
public:
    void generate(int width, int height);
    void generateWithExit(int width, int height);
    void render();
    bool isWall(float x, float y, float cellSize);
private:
    void carveMaze(int x, int y);
    int width, height;
    int** grid; // 2D grid: 0 = wall, 1 = path
};


class Coin {
    public:
        int xres, yres;
        bool collected;

    
        void render(int xres, int yres, float area);
        void coinCollect(int xres1, int xres2, int xres3, int xres4, int yres1, int yres2, int yres3, int yres4, float pos1, float pos2);
};


class mgonzalez3 {
public:
    void darkBack(float r, float g, float b, float a);
    void lighting(float pos1, float pos2, float area);
};

#endif // MGONZALEZ3_H
