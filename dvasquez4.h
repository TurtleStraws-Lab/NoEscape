
#ifndef DVASQUEZ4_H
#define DVASQUEZ4_H
//#ifndef STICKMAN_H
//#define STICKMAN_H
#include <GL/gl.h>
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];
#include <GL/gl.h>
class drawCircle
{
	private:
		float x,y,r;
		int num_seg;
	public:
		drawCircle(float x, float y, float r, int num_seg);
};

class DrawStickman {
	public:
		void Stickman(float r, float b, float a, float z);
	
};

#endif
