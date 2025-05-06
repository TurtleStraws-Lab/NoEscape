#ifndef JTURALLO_H
#define JTURALLO_H

//for textbox
#include <string>
#include <GL/glut.h>
#include <sstream>
#include <X11/Xlib.h>
//might need more for images

#define WIN_WIDTH 840
#define WIN_HEIGHT 680
#define STEP 15
#define DELAY 100000
#define LEFT_X 210
#define RIGHT_X 580
#define TOP_Y 230
#define BOTTOM_Y 500

class Textbox {
    private:
        int x, y;
        int width, height;
        std::string text;
        unsigned int color;

    public:
        Textbox(
                int x, int y, int width, int height, 
                const std::string &text, unsigned int color = 0x00ff0000
                );
        void draw();
        void setText(const std::string &newText);
        void setPosition(int newX, int newY);
        void setColor(unsigned int newColor);
};

class Image {
    public:
        int  width, height;
        unsigned char *data;

        ~Image();
        Image(const char *fname);
};

void draw_walls(Display *d, Window w, GC gc);
void draw_stickman(Display *d, Window w, GC gc, int x, int y);
void draw_stickman_flipped(Display *d, Window w, GC gc, int x, int y);
void draw_red_x_stroke_by_stroke(Display *d, Window w, GC gc, int x, int y);
void stickman_turn_around(Display *d, Window w, GC gc, int x, int y);
void animate_walk_back(Display *d, Window w, GC gc, int x, int y);
void animate_walk_with_walls(Display *d, Window w, GC gc, int *out_x, int *out_y);

extern Textbox* title;
extern Textbox* names;

void makeCredits();
void displayCredits();
void removeCredits();

#endif
