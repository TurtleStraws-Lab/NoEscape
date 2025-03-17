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

/*
   currently rendering a background for loading screen and for the
   background of maze
   use the background file rather than repurposing code
   */


//should the function end after receiving a signal that
//loading has finished? or should it have a min timer

/*-------------------------------------------------------------------------*/
//VARIABLE DECLARATIONS
/*------------------------------------------------------------------------*/
Textbox* title = nullptr;
Textbox* names = nullptr;


Textbox::Textbox(int x, int y, int width, int height, const std::string &text, unsigned int color)
    : x(x), y(y), width(width), height(height), text(text), color(color) {}

void Textbox::draw()
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;

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
    //Going to try having all names in one textbox
    //to minimize # of objects
    /*
       Textbox names(200, 450, 400, 50, "Moises Gonzales", 0x00ff0000);
       Textbox name2(200, 400, 400, 50, "Evelynn Turallo", 0x0000ff00);
       Textbox name3(200, 350, 400, 50, "Geneva Regpala", 0x000000ff);
       Textbox name4(200, 300, 400, 50, "Sayed Jalal Sayed M Nasim", 0x00ff00ff);
       Textbox name5(200, 250, 400, 50, "Devin Vasquez", 0x00ffff00);
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


//textbox class can be reused for an instruction menu

//notes:
//use small, individual statements that do not
//require too many variables being passed to it
//to isolate errors
//also declare prototypes of functions included in these files 
//in the main file
//use "extern" for these prototypes
//The prototype (and real function) can receive a pointer *
//The function call will pass an address (&)
//
//
