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

// buffers to store the sound
static ALuint beepBuffer;
static ALuint beepSource;
static ALuint buffer;
static ALuint source;
float currentVolume = 1.0f;

// need to change the color of the bar
void setVolumeBarColor(float volume) {
    if (volume > 0.7f) {
        float r = 0.0f;
        float g = 1.0f;
        float b = 0.0f;
        glColor3f(r, g, b); // Green
    }
    else if (volume > 0.4f) {
        float r = 1.0f;
        float g = 1.0f;
        float b = 0.0f;
        glColor3f(r, g, b); // Yellow
    }
    else {
        float r = 1.0f;
        float g = 0.0f;
        float b = 0.0f;
        glColor3f(r, g, b); // Red
    }
}


// the volume bar
void drawVolumeBar(float volume) {
    // Move the bar slightly to the right
    float x1 = 110.0f;
    float x2 = 210.0f;
    float y1 = 25.0f;
    float y2 = 45.0f;

    float barWidth = (x2 - x1) * volume;

    // Draw background bar (dark gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    // Draw filled portion (green)
    setVolumeBarColor(volume);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1 + barWidth, y1);
    glVertex2f(x1 + barWidth, y2);
    glVertex2f(x1, y2);
    glEnd();

    // Draw border (black)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Loading background sound
bool initSound() {
    cout << "[DEBUG] Initializing ALUT..." << endl;

    // background sound
    int argc = 0;
    char *argv[] = { NULL };
    if (!alutInit(&argc, argv)) {
        cerr << "[ERROR] alutInit failed: "
            << alutGetErrorString(alutGetError()) << endl;
        return false;
    }

    cout << "[DEBUG] Loading sound file: background.wav"
        << endl;
    buffer =
        alutCreateBufferFromFile("sounds/background.wav");
    if (buffer == AL_NONE) {
        cerr << "[ERROR] Failed to load sound file: "
            << alutGetErrorString(alutGetError()) << endl;
        return false;
    }

    alGenSources(1, &source);
    if (alGetError() != AL_NO_ERROR) {
        cerr << "[ERROR] Failed to generate audio source."
            << endl;
        return false;
    }

    // beep sound
    beepBuffer =
        alutCreateBufferFromFile("sounds/beep.wav");
    if (beepBuffer == AL_NONE) {
        cerr << "[ERROR] Failed to load beep.wav: "
            << alutGetErrorString(alutGetError()) << endl;
        return false;
    }

    alGenSources(1, &beepSource);
    if (alGetError() != AL_NO_ERROR) {
        cerr << "[ERROR] Failed to generate beep source."
            << endl;
        return false;
    }

    alSourcei(beepSource, AL_LOOPING, AL_FALSE); 
    alSourcef(beepSource, AL_GAIN, 1.0f); 

    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcef(source, AL_GAIN, currentVolume);

    cout << "[DEBUG] Sound initialized successfully."
        << endl;
    return true;
}

// beep should be temporary
void stopBeep(int value) {
    (void)value;
    alSourceStop(beepSource);
}

// playing the beep
void playBeep() {
    alSourcePlay(beepSource);
    alSourcei(beepSource, AL_LOOPING, AL_FALSE); 
    glutTimerFunc(1000, stopBeep, 0); 
}


// start the sound
void startBackgroundSound() {
    cout << "[DEBUG] Attempting to play background sound.."
        << endl;
    alSourcePlay(source);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        cerr << "[ERROR] Failed to play sound: "
            << alutGetErrorString(error) << endl;
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
    alDeleteSources(1, &beepSource);
    alDeleteBuffers(1, &beepBuffer);

    alutExit();
}

// drawing the vol up and vol down
void drawButton(float x1, float y1,
        float x2, float y2, const char* label) {
    glColor3f(0.2f, 0.6f, 0.8f); // Button color
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    // Border
    glColor3f(0.0f, 0.3f, 0.5f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    // Centered label
    float textWidth = glutBitmapLength(
            GLUT_BITMAP_HELVETICA_12,
            (const unsigned char*)label);
    float textX = x1 + ((x2 - x1) - textWidth) / 2;
    float textY = (y1 + y2) / 2 - 4;
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(textX, textY);
    for (const char* c = label; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}


bool isMuted = false;
float previousVolume = 1.0f;

// mute function toggle 
void toggleMute() {
    if (!isMuted) {
        previousVolume = currentVolume;
        currentVolume = 0.0f;
        isMuted = true;
    } else {
        currentVolume = previousVolume;
        isMuted = false;
    }
    alSourcef(source, AL_GAIN, currentVolume);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawButton(100.0f, 200.0f, 200.0f, 250.0f, "Vol Up");
    drawButton(210.0f, 200.0f, 310.0f, 250.0f, "Vol Down");
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

// function that could lunch the sound
void sayed()
{
    cout<<"the Game just started";
    initSound();
    startBackgroundSound();
}

// function that end the sound
void sayedend() {
     shutdownSound();
}

