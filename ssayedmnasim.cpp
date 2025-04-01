// Author: Sayed Jalal Sayed M Nasim
// date: 02/11/2025
//
//
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <iostream>
#include "ssayedmnasim.h"

using namespace std;


static ALuint buffer;
static ALuint source;

bool initSound() {
    alutInit(0, NULL);

    buffer = alutCreateBufferFromFile("sounds/background.wav");
    if (buffer == AL_NONE) {
        std::cerr << "Error loading sound file.\n";
        return false;
    }

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcef(source, AL_GAIN, 0.5f); // adjust volume if needed

    return true;
}

void startBackgroundSound() {
    alSourcePlay(source);
}

void shutdownSound() {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alutExit();
}

void sayed()
{
   // Rect r;
   // r.left =10;
   // r.center =0;

   // ggprint8b(&r, 16, 0x00ff0000, "Sayed Jalal Sayed M Nasim");

    cout<<"the Game just started";
   
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
