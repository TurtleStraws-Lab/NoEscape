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

void draw_walls(Display *d, Window w, GC gc) {
    XDrawLine(d, w, gc, 30, 180, 30, 300);   // left shaft left wall
    XDrawLine(d, w, gc, 70, 220, 70, 300);   // left shaft right wall
    XDrawLine(d, w, gc, 30, 180, 220, 180);  // horizontal top
    XDrawLine(d, w, gc, 70, 220, 180, 220);  // horizontal bottom
    XDrawLine(d, w, gc, 180, 220, 180, 300); // right shaft left wall
    XDrawLine(d, w, gc, 220, 180, 220, 300); // right shaft right wall
    XDrawLine(d, w, gc, 180, 300, 220, 300); // dead-end bottom wall
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
    while (y > 220) {
        y -= STEP;
        XClearWindow(d, w);
        draw_walls(d, w, gc);
        draw_stickman_flipped(d, w, gc, x, y);
        XFlush(d);
        usleep(DELAY);
    }
    while (x > 50) {
        x -= STEP;
        XClearWindow(d, w);
        draw_walls(d, w, gc);
        draw_stickman_flipped(d, w, gc, x, y);
        XFlush(d);
        usleep(DELAY);
    }
    while (y < 300) {
        y += STEP;
        XClearWindow(d, w);
        draw_walls(d, w, gc);
        draw_stickman_flipped(d, w, gc, x, y);
        XFlush(d);
        usleep(DELAY);
    }
}

void animate_walk_with_walls(Display *d, Window w, GC gc, int *out_x, int *out_y) {
    int x = 50, y = 300;
    for (int step = 0; step < 34; step++) {
        XClearWindow(d, w);
        if (step < 10) {
            int y1 = 300 - step * 12;
            if (y1 < 180) y1 = 180;
            int y2 = 300 - step * 8;
            if (y2 < 220) y2 = 220;
            XDrawLine(d, w, gc, 30, 300, 30, y1);
            XDrawLine(d, w, gc, 70, 300, 70, y2);
        }
        if (step >= 10 && step < 23) {
            int x1 = 30 + (step - 10) * 15;
            if (x1 > 220) x1 = 220;
            XDrawLine(d, w, gc, 30, 180, x1, 180);
        }
        if (step >= 13 && step < 23) {
            int x2 = 70 + (step - 13) * 11;
            if (x2 > 180) x2 = 180;
            XDrawLine(d, w, gc, 70, 220, x2, 220);
        }
        if (step >= 23) {
            int y3 = 220 + (step - 23) * 8;
            if (y3 > 300) y3 = 300;
            int y4 = 180 + (step - 23) * 12;
            if (y4 > 300) y4 = 300;
            XDrawLine(d, w, gc, 180, 220, 180, y3);
            XDrawLine(d, w, gc, 220, 180, 220, y4);
        }
        if (step < 10) {
            y -= STEP;
        } else if (step < 25) {
            x += STEP;
        } else {
            y += STEP;
        }
        draw_stickman(d, w, gc, x, y);
        XFlush(d);
        usleep(DELAY);
    }
    *out_x = x;
    *out_y = y;
}

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

