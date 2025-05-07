//Name: Eve Turallo
//CMPS 2025 - Software Engineering
//
//
//Future personal file for code contributions to final project
//use quotations for local files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <GL/glx.h>
#include <GL/gl.h>
#include "fonts.h"
#include "jturallo.h"
//for images; from backgrounds.cpp
#include <unistd.h>
//for cutscene
#include <X11/Xlib.h>

/*-------------------------------------------------------------------------*/
//VARIABLE DECLARATIONS
/*------------------------------------------------------------------------*/
/*
#define WIN_WIDTH 840
#define WIN_HEIGHT 680
#define STEP 15
#define DELAY 100000
*/
Textbox* title = nullptr;
Textbox* names = nullptr;



Textbox::Textbox(int x, int y, int width, int height, 
        const std::string &text, unsigned int color)
    : x(x), y(y), width(width), height(height), 
    text(text), color(color) {}

void Textbox::draw()
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1;

    int lineSpacing = 20;

    std::istringstream stream(text);
    std::string line;

    while (std::getline(stream,line)) {
        ggprint8b(&r, 16, color, line.c_str());
        r.bot -= lineSpacing;
    }

}

void Textbox::setText(const std::string &newText) {
    text = newText;
}

void Textbox::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Textbox::setColor(unsigned int newColor) {
    color = newColor;
}

void makeCredits() {

    if (!title) {
        title = new Textbox(200, 500, 400, 50, "Game Credits", 0x00ffffff);
    }

    if (!names) {
        std::string creditsText = 
            "Game Credits\n"
            "Moises Gonzales\n"
            "Evelynn Turallo\n"
            "Geneva Regpala\n"
            "Sayed Jalal Sayed M Nasim\n"
            "Devin Vasquez";

        names = new Textbox(200, 450, 400, 250, creditsText, 0x0000ff00);
        //All names in one textbox
        /*
           Textbox names(200, 450, 400, 50, 
           "Moises Gonzales", 0x00ff0000);
           Textbox name2(200, 400, 400, 50, 
           "Evelynn Turallo", 0x0000ff00);
           Textbox name3(200, 350, 400, 50, 
           "Geneva Regpala", 0x000000ff);
           Textbox name4(200, 300, 400, 50, 
           "Sayed Jalal Sayed M Nasim", 0x00ff00ff);
           Textbox name5(200, 250, 400, 50, 
           "Devin Vasquez", 0x00ffff00);
           */

    }
}

void displayCredits() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (title) {
        title->draw();
    }

    if (names) {
        names->draw();
    }

    glFlush();

}

void removeCredits() {
    delete title;
    title = nullptr;

    delete names;
    names = nullptr;
}

Image::~Image() {
    delete[] data;
}

/*
void draw_red_x_stroke_by_stroke(Display *d, Window w, GC gc, int x, int y) {
    XColor red;
    Colormap colormap = DefaultColormap(d, 0);
    XAllocNamedColor(d, colormap, "red", &red, &red);
    XSetForeground(d, gc, red.pixel);
    XDrawLine(d, w, gc, x - 10, y - 10, x + 10, y + 10);
    XFlush(d);
    usleep(200000);
    XDrawLine(d, w, gc, x - 10, y + 10, x + 10, y - 10);
    XFlush(d);
    XSetForeground(d, gc, WhitePixel(d, 0));
}
*/


//Taken from background.cpp
//Variables in jturallo.h
Image::Image(const char *fname) {
    if (fname[0] == '\0')
        return;

    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    name[slen - 4] = '\0';

    char ppmname[80];
    sprintf(ppmname, "%s.ppm", name);


    char ts[100];
    sprintf(ts, "convert %s %s", fname, ppmname);
    system(ts);

    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);  
        fgets(line, 200, fpi);   


        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);

        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);


        int n = width * height * 3;
        data = new unsigned char[n];

        for (int i = 0; i < n; i++)
            data[i] = fgetc(fpi);

        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n", ppmname);
        exit(0);
    }


    unlink(ppmname);
}

void draw_walls(Display *d, Window w, GC gc) {
    XDrawLine(d, w, gc, 180, 180, 180, 500);   // left shaft left wall
    XDrawLine(d, w, gc, 240, 180, 240, 500);   // left shaft right wall
    XDrawLine(d, w, gc, 240, 200, 620, 200);   // horizontal top wall (from right side of left shaft)
    XDrawLine(d, w, gc, 240, 260, 540, 260);   // horizontal bottom wall
    XDrawLine(d, w, gc, 540, 260, 540, 500);   // right shaft left wall
    XDrawLine(d, w, gc, 620, 200, 620, 500);   // right shaft right wall
    XDrawLine(d, w, gc, 540, 500, 620, 500);   // dead-end bottom wall
}

void draw_stickman(Display *d, Window w, GC gc, int x, int y) {
    XDrawLine(d, w, gc, x, y, x, y - 15);
    XDrawLine(d, w, gc, x, y - 15, x - 5, y - 22);
    XDrawLine(d, w, gc, x, y - 15, x + 5, y - 22);
    XDrawLine(d, w, gc, x, y, x - 5, y + 10);
    XDrawLine(d, w, gc, x, y, x + 5, y + 10);
    XDrawArc(d, w, gc, x - 5, y - 30, 10, 10, 0, 360 * 64);
}

void draw_stickman_flipped(Display *d, Window w, GC gc, int x, int y) {
    XDrawLine(d, w, gc, x, y, x, y - 15);
    XDrawLine(d, w, gc, x, y - 15, x + 5, y - 22);
    XDrawLine(d, w, gc, x, y - 15, x - 5, y - 22);
    XDrawLine(d, w, gc, x, y, x + 5, y + 10);
    XDrawLine(d, w, gc, x, y, x - 5, y + 10);
    XDrawArc(d, w, gc, x - 5, y - 30, 10, 10, 0, 360 * 64);
}

void stickman_turn_around(Display *d, Window w, GC gc, int x, int y) {
    usleep(300000);
    XClearWindow(d, w);
    draw_walls(d, w, gc);
    XColor red;
    Colormap colormap = DefaultColormap(d, 0);
    XAllocNamedColor(d, colormap, "red", &red, &red);
    XSetForeground(d, gc, red.pixel);
    XDrawLine(d, w, gc, x - 10, y - 10, x + 10, y + 10);
    XDrawLine(d, w, gc, x - 10, y + 10, x + 10, y - 10);
    XSetForeground(d, gc, WhitePixel(d, 0));
    draw_stickman_flipped(d, w, gc, x, y);
    XFlush(d);
}

void animate_walk_back(Display *d, Window w, GC gc, int x, int y) {
    while (y > TOP_Y) {
        y -= STEP;
        XClearWindow(d, w);
        draw_walls(d, w, gc);
        draw_stickman_flipped(d, w, gc, x, y);
        XFlush(d);
        usleep(DELAY);
    }

    while (x > LEFT_X) {
        x -= STEP;
        XClearWindow(d, w);
        draw_walls(d, w, gc);
        draw_stickman_flipped(d, w, gc, x, y);
        XFlush(d);
        usleep(DELAY);
    }

    while (y > 180) {  // Upward into extended left shaft
        y -= STEP;
        XClearWindow(d, w);
        draw_walls(d, w, gc);
        draw_stickman_flipped(d, w, gc, x, y);
        XFlush(d);
        usleep(DELAY);
    }
}

void animate_walk_with_walls(Display *d, Window w, GC gc, int *out_x, int *out_y) {
    int x = LEFT_X;
    int y = BOTTOM_Y;

    int up_steps = (BOTTOM_Y - TOP_Y) / STEP;
    int right_steps = (RIGHT_X - LEFT_X) / STEP;
    int down_steps = (BOTTOM_Y - TOP_Y) / STEP;

    Pixmap buffer = XCreatePixmap(d, w, WIN_WIDTH, WIN_HEIGHT, DefaultDepth(d, 0));

    // === UPWARD WALK ===
    for (int step = 0; step < up_steps; step++) {
        y -= STEP;
        XSetForeground(d, gc, BlackPixel(d, 0));
        XFillRectangle(d, buffer, gc, 0, 0, WIN_WIDTH, WIN_HEIGHT);

        int wall_bottom_limit = 200;
        int wall_top = (y < wall_bottom_limit) ? wall_bottom_limit : y;
        XSetForeground(d, gc, WhitePixel(d, 0));
        XDrawLine(d, buffer, gc, 180, BOTTOM_Y, 180, wall_top);
        int right_wall_top = (y > 260) ? y : 260;
        XDrawLine(d, buffer, gc, 240, BOTTOM_Y, 240, right_wall_top);

        draw_stickman(d, buffer, gc, x, y);
        XCopyArea(d, buffer, w, gc, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0);
        XFlush(d);
        usleep(DELAY);
    }

    // === RIGHT WALK ===
    for (int step = 0; step < right_steps; step++) {
        x += STEP;
        XSetForeground(d, gc, BlackPixel(d, 0));
        XFillRectangle(d, buffer, gc, 0, 0, WIN_WIDTH, WIN_HEIGHT);

        XSetForeground(d, gc, WhitePixel(d, 0));
        XDrawLine(d, buffer, gc, 180, BOTTOM_Y, 180, 200);
        XDrawLine(d, buffer, gc, 240, BOTTOM_Y, 240, 260);

        XDrawLine(d, buffer, gc, 240, 200, x, 200);
        if (x >= 240) {
            int wall_end_x = (x < 540) ? x : 540;
            XDrawLine(d, buffer, gc, 240, 260, wall_end_x, 260);
        }

        draw_stickman(d, buffer, gc, x, y);
        XCopyArea(d, buffer, w, gc, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0);
        XFlush(d);
        usleep(DELAY);
    }

    // === DOWNWARD WALK ===
    for (int step = 0; step < down_steps - 1; step++) {
        y += STEP;
        XSetForeground(d, gc, BlackPixel(d, 0));
        XFillRectangle(d, buffer, gc, 0, 0, WIN_WIDTH, WIN_HEIGHT);

        XSetForeground(d, gc, WhitePixel(d, 0));
        XDrawLine(d, buffer, gc, 180, BOTTOM_Y, 180, 200);
        XDrawLine(d, buffer, gc, 240, BOTTOM_Y, 240, 260);
        XDrawLine(d, buffer, gc, 240, 200, 620, 200);
        XDrawLine(d, buffer, gc, 240, 260, 540, 260);

        if (y > 260) {
            int shaft_bottom = (y < BOTTOM_Y) ? y : BOTTOM_Y;
            XDrawLine(d, buffer, gc, 540, 260, 540, shaft_bottom);
            XDrawLine(d, buffer, gc, 620, 200, 620, shaft_bottom);
        }

        if (step >= down_steps - 4) {
            int coin_y = BOTTOM_Y - STEP;
            XSetForeground(d, gc, WhitePixel(d, 0));
            XDrawArc(d, buffer, gc, RIGHT_X - 5, coin_y - 5, 10, 10, 0, 360 * 64);

            XColor gold;
            Colormap colormap = DefaultColormap(d, 0);
            XAllocNamedColor(d, colormap, "gold", &gold, &gold);
            XSetForeground(d, gc, gold.pixel);
            XFillArc(d, buffer, gc, RIGHT_X - 5, coin_y - 5, 10, 10, 0, 360 * 64);

            XSetForeground(d, gc, WhitePixel(d, 0));
        }

        draw_stickman(d, buffer, gc, x, y);
        XCopyArea(d, buffer, w, gc, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0);
        XFlush(d);
        usleep(DELAY);
    }

    XFreePixmap(d, buffer);
    *out_x = x;
    *out_y = y;
}

