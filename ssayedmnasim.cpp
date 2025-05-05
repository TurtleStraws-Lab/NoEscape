// Author: Sayed Jalal Sayed M Nasim
// date: 02/11/2025
//
//
#include <GL/glut.h>
#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "ssayedmnasim.h" 

using namespace std;

static ALuint buffer;
static ALuint source;
static float currentVolume = 1.0f; 

// Loading background sound
/*
bool initSound() {
    int argc =0;
    char *argv[] = { NULL };
    alutInit(&argc, argv);

    buffer = alutCreateBufferFromFile("background_fixed.wav");
    if (buffer == AL_NONE) {
        cerr << "Error loading sound file.\n";
        return false;
    }

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcef(source, AL_GAIN, currentVolume);

    return true;
}*/


bool initSound() {
    cout << "[DEBUG] Initializing ALUT..." << endl;

    int argc = 0;
    char *argv[] = { NULL };
    if (!alutInit(&argc, argv)) {
        cerr << "[ERROR] alutInit failed: " << alutGetErrorString(alutGetError()) << endl;
        return false;
    }

    cout << "[DEBUG] Loading sound file: background.wav" << endl;
    buffer = alutCreateBufferFromFile("background_fixed.wav");
    if (buffer == AL_NONE) {
        cerr << "[ERROR] Failed to load sound file: " << alutGetErrorString(alutGetError()) << endl;
        return false;
    }

    alGenSources(1, &source);
    if (alGetError() != AL_NO_ERROR) {
        cerr << "[ERROR] Failed to generate audio source." << endl;
        return false;
    }

    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcef(source, AL_GAIN, currentVolume);

    cout << "[DEBUG] Sound initialized successfully." << endl;
    return true;
}


// start the sound
/*
void startBackgroundSound() {
    alSourcePlay(source);
}*/
void startBackgroundSound() {
    cout << "[DEBUG] Attempting to play background sound..." << endl;
    alSourcePlay(source);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        cerr << "[ERROR] Failed to play sound: " << alutGetErrorString(error) << endl;
    } else {
        cout << "[DEBUG] Background sound playing." << endl;
    }
}

// fucntion to increase sound
void volumeUp() {
    currentVolume += 0.1f;
    if (currentVolume > 1.0f) {
        currentVolume = 1.0f;
    }
    alSourcef(source, AL_GAIN, currentVolume);
    cout << "Volume Up: " << currentVolume << endl;
}

// function to decrease the sound
void volumeDown() {
    currentVolume -= 0.1f;
    if (currentVolume < 0.0f) {
        currentVolume = 0.0f;
    }
    alSourcef(source, AL_GAIN, currentVolume);
    cout << "Volume Down: " << currentVolume << endl;
}

// function to close it
void shutdownSound() {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alutExit();
}

void drawButton(float x1, float y1, float x2, float y2, const char* label) {
    glColor3f(0.2f, 0.6f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x1 + 10, (y1 + y2) / 2);
    for (const char* c = label; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawButton(100, 300, 300, 350, "Volume Up");
    drawButton(100, 200, 300, 250, "Volume Down");

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = 480 - y; 

        if (x >= 100 && x <= 300 && y >= 300 && y <= 350) {
            volumeUp();
        }
        if (x >= 100 && x <= 300 && y >= 200 && y <= 250) {
            volumeDown();
        }
    }
}

void setup() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
}

void sayed()
{
   // Rect r;
   // r.left =10;
   // r.center =0;

   // ggprint8b(&r, 16, 0x00ff0000, "Sayed Jalal Sayed M Nasim");

    cout<<"the Game just started";
    initSound();
    startBackgroundSound();
}

void sayedend() {
     shutdownSound();
}


void changeDirection()
{
    // Reset velocity
//    g.ship.vel[0] = 0.0f;
//   g.ship.vel[1] = 0.0f;

    // Apply directional movement
//   if (gl.keys[XK_Up]) {
//        g.ship.vel[1] = 4.0f;
//       g.ship.angle = 0.0f;
//    }
//    if (gl.keys[XK_Down]) {
//        g.ship.vel[1] = -4.0f;
//        g.ship.angle = 180.0f;
//    }
//    if (gl.keys[XK_Left]) {
//        g.ship.vel[0] = -4.0f;
//        g.ship.angle = 270.0f;
//    }
//    if (gl.keys[XK_Right]) {
//        g.ship.vel[0] = 4.0f;
//        g.ship.angle = 90.0f;
//    }
}
