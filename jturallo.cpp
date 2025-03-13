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
#include "fonts.h"

/*
currently rendering a background for loading screen and for the
background of maze
use the background file rather than repurposing code
*/


//should the function end after receiving a signal that
//loading has finished? or should it have a min timer

/*
void render_Text(float x, float y, const char* text, void* font) {

}
*/


//will upgrade this to be done with graphics rather than window text soon
//adding now for the sake of committing code for lab
void display_Credits() {
    Rect r;
    glClear(GL_COLOR_BUFFER_BIT);
    r.bot = 10;
    //r.bot = gl.yres - 20;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Moises Gonzales");
    ggprint8b(&r, 16, 0x00ff0000, "Evelynn Turallo");
    ggprint8b(&r, 16, 0x00ff0000, "Geneva Regpala");
    ggprint8b(&r, 16, 0x00ff0000, "Sayed Jalal Sayed M Nasim");
    ggprint8b(&r, 16, 0x00ff0000, "Devin Vasquez");

}

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
