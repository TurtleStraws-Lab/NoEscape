#ifndef MGONZALEZ3_H
#define MGONZALEZ3_H

class Maze {
public:
    void generate(int width, int height);
    void render();

private:
    void carveMaze(int x, int y);
    int width, height;
    int** grid; // 2D grid: 0 = wall, 1 = path
};

class mgonzalez3 {
public:
    void darkBack(float r, float g, float b, float a);
    void lighting(float pos1, float pos2, float area);
};

#endif // MGONZALEZ3_H
