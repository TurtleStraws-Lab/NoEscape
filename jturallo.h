#ifndef JTURALLO_H
#define JTURALLO_H

#include <string>
#include <GL/glut.h>
#include <sstream>

class Textbox {
    private:
        int x, y;
        int width, height;
        std::string text;
        unsigned int color;

    public:
        Textbox(int x, int y, int width, int height, const std::string &text, unsigned int color = 0x00ff0000);
        void draw();
        void setText(const std::string &newText);
        void setPosition(int newX, int newY);
        void setColor(unsigned int newColor);
};

extern Textbox* title;
extern Textbox* names;

void makeCredits();
void displayCredits();
void removeCredits();

#endif
