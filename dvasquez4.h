
#ifndef DVASQUEZ4_H
#define DVASQUEZ4_H
#include <GL/gl.h>
#include <X11/Xlib.h>
typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];
#include <GL/gl.h>
//class Game;
extern "C" void updateShipAngle(float angle);
extern "C" void updateStickmanWalk(float angle);
class drawCircle {
    private:
        float x,y,r;
        int num_seg;
    public:
        drawCircle(float x, float y, float r, int num_seg);
};

class calcAngle {
    private:
        float x, y, a, l;
    public:
        calcAngle(float x, float y, float a, float l);

};

class DrawShip {

    public:
        void Ship(float a, float b, float c,float d);

};

class DrawStickman {

    public:
        void Stickman(float r, float b, float a,float d);

};
void resetStickman();
void updateWalkingLegs();
void Movement(Display *display,XEvent &event);
void MovementWalking(Display *display,XEvent &event);
#endif
