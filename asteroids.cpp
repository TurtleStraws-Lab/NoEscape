//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2025
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glut.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "mgonzalez3.h"
#include "gregpala.h"
#include "ssayedmnasim.h"
#include "dvasquez4.h"
#include "jturallo.h"

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];
Maze maze;
Coin coin;
//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
extern void displayCredits();
//-----------------------------------------------------------------------------
extern bool buttonPressed[];


class Global {
    public:
        int xres, yres;
        char keys[65536];
        int mouse_cursor_on;
        int fullscreen = 0;
        Global() {
            xres = 840;  //640 original value
            yres = 680;  //480 original value
            memset(keys, 0, 65536);
            // mouse value 1 = true = mouse is a regular mouse.
            mouse_cursor_on = 1;
        }
} gl;

class Ship {
    public:
        Vec pos;
        Vec dir;
        Vec vel;
        Vec acc;
        float angle;
        float color[3];
    public:
        Ship() {
            pos[0] = (Flt)(gl.xres/2) - 305;
            pos[1] = (Flt)(gl.yres/2) + 215;
            pos[2] = 0.0f;
            VecZero(dir);
            VecZero(vel);
            VecZero(acc);
            angle = 0.0;
            color[0] = color[1] = color[2] = 1.0;
        }
};

class Bullet {
    public:
        Vec pos;
        Vec vel;
        float color[3];
        struct timespec time;
    public:
        Bullet() { }
};

class Asteroid {
    public:
        Vec pos;
        Vec vel;
        int nverts;
        Flt radius;
        Vec vert[8];
        float angle;
        float rotate;
        float color[3];
        struct Asteroid *prev;
        struct Asteroid *next;
    public:
        Asteroid() {
            prev = NULL;
            next = NULL;
        }
};

class Game {
    public:
        Ship ship;
        Asteroid *ahead;
        Bullet *barr;
        int nasteroids;
        int nbullets;
        struct timespec bulletTimer;
        struct timespec mouseThrustTimer;
        bool mouseThrustOn;
    public:
        Game() {
            ahead = NULL;
            barr = new Bullet[MAX_BULLETS];
            nasteroids = 0;
            nbullets = 0;
            mouseThrustOn = false;
            //build 10 asteroids...
            for (int j=0; j<10; j++) {
                Asteroid *a = new Asteroid;
                a->nverts = 8;
                a->radius = rnd()*80.0 + 40.0;
                Flt r2 = a->radius / 2.0;
                Flt angle = 0.0f;
                Flt inc = (PI * 2.0) / (Flt)a->nverts;
                for (int i=0; i<a->nverts; i++) {
                    a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
                    a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
                    angle += inc;
                }
                a->pos[0] = (Flt)(rand() % gl.xres);
                a->pos[1] = (Flt)(rand() % gl.yres);
                a->pos[2] = 0.0f;
                a->angle = 0.0;
                a->rotate = rnd() * 4.0 - 2.0;
                a->color[0] = 0.8;
                a->color[1] = 0.8;
                a->color[2] = 0.7;
                a->vel[0] = (Flt)(rnd()*2.0-1.0);
                a->vel[1] = (Flt)(rnd()*2.0-1.0);
                //std::cout << "asteroid" << std::endl;
                //add to front of linked list
                a->next = ahead;
                if (ahead != NULL)
                    ahead->prev = a;
                ahead = a;
                ++nasteroids;
            }
            clock_gettime(CLOCK_REALTIME, &bulletTimer);
        }
        ~Game() {
            delete [] barr;
        }
} g;

//X Windows variables
class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GLXContext glc;
    public:
        Display *getDisplay() {return dpy;}
        Window getWindow() const { return win; }
        X11_wrapper() { }
        X11_wrapper(int w, int h) {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
            XSetWindowAttributes swa;
            setup_screen_res(gl.xres, gl.yres);
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL) {
                std::cout << "\n\tcannot connect to X server" << std::endl;
                exit(EXIT_FAILURE);
            }
            Window root = DefaultRootWindow(dpy);
            XWindowAttributes getWinAttr;
            XGetWindowAttributes(dpy, root, &getWinAttr);
            //int fullscreen = 0;
            gl.xres = w;
            gl.yres = h;
            if (!w && !h) {
                //Go to fullscreen.
                gl.xres = getWinAttr.width;
                gl.yres = getWinAttr.height;
                //When window is fullscreen, there is no client window
                //so keystrokes are linked to the root window.
                XGrabKeyboard(dpy, root, False,
                        GrabModeAsync, GrabModeAsync, CurrentTime);
                gl.fullscreen=1;
            }
            XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
            if (vi == NULL) {
                std::cout << "\n\tno appropriate visual found\n" << std::endl;
                exit(EXIT_FAILURE);
            } 
            Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
            swa.colormap = cmap;
            swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
                StructureNotifyMask | SubstructureNotifyMask;
            unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
            if (gl.fullscreen) {
                winops |= CWOverrideRedirect;
                swa.override_redirect = True;
            }
            win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
                    vi->depth, InputOutput, vi->visual, winops, &swa);
            //win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
            //vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
            set_title();
            glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
            show_mouse_cursor(0);
        }
        ~X11_wrapper() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void set_title() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "No Escape");
        }
        void check_resize(XEvent *e) {
            //The ConfigureNotify is sent by the
            //server if the window is resized.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            if (xce.width != gl.xres || xce.height != gl.yres) {
                //Window size did change.
                reshape_window(xce.width, xce.height);
            }
        }
        void reshape_window(int width, int height) {
            //window has been resized.
            setup_screen_res(width, height);
            glViewport(0, 0, (GLint)width, (GLint)height);
            glMatrixMode(GL_PROJECTION); glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            glOrtho(0, gl.xres, 0, gl.yres, -1.0f, 1.0f);
            set_title();

            screenManager.setScreenDimensions(width, height);
        }
        void setup_screen_res(const int w, const int h) {
            gl.xres = w;
            gl.yres = h;
        }
        void swapBuffers() {
            glXSwapBuffers(dpy, win);
        }
        bool getXPending() {
            return XPending(dpy);
        }
        XEvent getXNextEvent() {
            XEvent e;
            XNextEvent(dpy, &e);
            return e;
        }
        void set_mouse_position(int x, int y) {
            XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
        }
        void show_mouse_cursor(const int onoff) {
            //	printf("show_mouse_cursor(%i)\n", onoff); fflush(stdout);
            if (onoff) {
                //this removes our own blank cursor.
                XUndefineCursor(dpy, win);
                return;
            }
            //vars to make blank cursor
            Pixmap blank;
            XColor dummy;
            char data[1] = {0};
            Cursor cursor;
            //make a blank cursor
            blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
            if (blank == None)
                std::cout << "error: out of memory." << std::endl;
            cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
            XFreePixmap(dpy, blank);
            //this makes the cursor. then set it using this function
            XDefineCursor(dpy, win, cursor);
            //after you do not need the cursor anymore use this function.
            //it will undo the last change done by XDefineCursor
            //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
        }
} x11(gl.xres, gl.yres);
// ---> for fullscreen x11(0, 0);

//function prototypes
void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

//==========================================================================
// M A I N
//==========================================================================
int main()
{
    int argc = 1;
    char *argv[1] = {(char *)"Asteroids"};
    glutInit(&argc, argv);  // REQUIRED for glutBitmapCharacter

    logOpen();
    init_opengl();
    // added by sayed
    initSound();
    startBackgroundSound();

    GC gc = XCreateGC(x11.getDisplay(), x11.getWindow(), 0, NULL);
    XSetForeground(x11.getDisplay(), gc, WhitePixel(x11.getDisplay(), 0));
    play_cutscene(x11.getDisplay(), x11.getWindow(), gc);

    srand(time(NULL));
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    x11.set_mouse_position(200, 200);
    x11.show_mouse_cursor(gl.mouse_cursor_on);
    int done=0;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            check_mouse(&e);
            done = check_keys(&e);
            if (screenManager.getState() == GAME) {
                if (buttonPressed[0]) {
                    MovementWalking(x11.getDisplay(),e);
                } else if (buttonPressed[1] || buttonPressed[2]) {
                    Movement(x11.getDisplay(),e);
                }
            }
        }
            clock_gettime(CLOCK_REALTIME, &timeCurrent);
            timeSpan = timeDiff(&timeStart, &timeCurrent);
            timeCopy(&timeStart, &timeCurrent);
            physicsCountdown += timeSpan;
            // Update the screen manager
            screenManager.update();

            // Only update physics if in game state
            if (screenManager.getState() == GAME) {
                while (physicsCountdown >= physicsRate) {
                    physics();
                    physicsCountdown -= physicsRate;
                }
            }
            render();
            x11.swapBuffers();
        }
        cleanup_fonts();
        logClose();
        shutdownSound();
        return 0;
    }
    extern "C" void updateShipAngle(float angle) {
        g.ship.angle = angle;
    }
    void init_opengl(void)
    {
        //OpenGL initialization
        glViewport(0, 0, gl.xres, gl.yres);
        //Initialize matrices
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        //This sets 2D mode (no perspective)
        glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
        //
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_FOG);
        glDisable(GL_CULL_FACE);
        //
        //Clear the screen to black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        //Do this to allow fonts
        glEnable(GL_TEXTURE_2D);
        initialize_fonts();

        screenManager.setScreenDimensions(gl.xres, gl.yres);
    }

    void normalize2d(Vec v)
    {
        Flt len = v[0]*v[0] + v[1]*v[1];
        if (len == 0.0f) {
            v[0] = 1.0;
            v[1] = 0.0;
            return;
        }
        len = 1.0f / sqrt(len);
        v[0] *= len;
        v[1] *= len;
    }

    void check_mouse(XEvent *e)
    {
        //Did the mouse move?
        //Was a mouse button clicked?
        static int savex = 0;
        static int savey = 0;
        //
        static int ct=0;
        //std::cout << "m" << std::endl << std::flush;
        if (screenManager.getState() == GAME) {
            if (e->type == ButtonRelease) {
                return;
            }
            if (screenManager.getState() == GAME && e->type == ButtonPress) {
                int mx = e->xbutton.x;
                int my = gl.yres - e->xbutton.y; // flip Y for OpenGL coordinates

                if (mx >= 10 && mx <= 110 && my >= 60 && my <= 90) {
                    volumeUp();
                }
                if (mx >= 10 && mx <= 110 && my >= 10 && my <= 40) {
                    volumeDown();
                }
            }
            if (e->type == ButtonPress) {
                if (e->xbutton.button==1) {
                    //Left button is down
                    //a little time between each bullet
                    struct timespec bt;
                    clock_gettime(CLOCK_REALTIME, &bt);
                    double ts = timeDiff(&g.bulletTimer, &bt);
                    if (ts > 0.1) {
                        timeCopy(&g.bulletTimer, &bt);
                        //shoot a bullet...
                        if (g.nbullets < MAX_BULLETS) {
                            Bullet *b = &g.barr[g.nbullets];
                            timeCopy(&b->time, &bt);
                            b->pos[0] = g.ship.pos[0];
                            b->pos[1] = g.ship.pos[1];
                            b->vel[0] = g.ship.vel[0];
                            b->vel[1] = g.ship.vel[1];
                            //convert ship angle to radians
                            Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
                            //convert angle to a vector
                            Flt xdir = cos(rad);
                            Flt ydir = sin(rad);
                            b->pos[0] += xdir*20.0f;
                            b->pos[1] += ydir*20.0f;
                            b->vel[0] += xdir*6.0f + rnd()*0.1;
                            b->vel[1] += ydir*6.0f + rnd()*0.1;
                            b->color[0] = 1.0f;
                            b->color[1] = 1.0f;
                            b->color[2] = 1.0f;
                            ++g.nbullets;
                        }
                    }
                }
                if (e->xbutton.button==3) {
                    //Right button is down
                }
            }
            //keys[XK_Up] = 0;
            if (savex != e->xbutton.x || savey != e->xbutton.y) {
                // the mouse moved
                int xdiff = savex - e->xbutton.x;
                int ydiff = savey - e->xbutton.y;
                savex = e->xbutton.x;
                savey = e->xbutton.y;
                if (++ct < 10)
                    return;		
                //std::cout << "savex: " << savex << std::endl << std::flush;
                //std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
                //std::flush;
                //
                // If mouse cursor is on, it does not control the ship.
                // It's a regular mouse.
                if (gl.mouse_cursor_on)
                    return;
                //printf("mouse move "); fflush(stdout);
                if (xdiff > 0) {
                    //std::cout << "xdiff: " << xdiff << std::endl << std::flush;
                    g.ship.angle += 0.05f * (float)xdiff;
                    if (g.ship.angle >= 360.0f)
                        g.ship.angle -= 360.0f;
                }
                else if (xdiff < 0) {
                    //std::cout << "xdiff: " << xdiff << std::endl << std::flush;
                    g.ship.angle += 0.05f * (float)xdiff;
                    if (g.ship.angle < 0.0f)
                        g.ship.angle += 360.0f;
                }
                if (ydiff > 0) {
                    //apply thrust
                    //convert ship angle to radians
                    Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
                    //convert angle to a vector
                    Flt xdir = cos(rad);
                    Flt ydir = sin(rad);
                    g.ship.vel[0] += xdir * (float)ydiff * 0.01f;
                    g.ship.vel[1] += ydir * (float)ydiff * 0.01f;
                    Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
                            g.ship.vel[1]*g.ship.vel[1]);
                    if (speed > 10.0f) {
                        speed = 10.0f;
                        normalize2d(g.ship.vel);
                        g.ship.vel[0] *= speed;
                        g.ship.vel[1] *= speed;
                    }
                    g.mouseThrustOn = true;
                    clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
                }
                x11.set_mouse_position(200,200);
                savex = 200;
                savey = 200;
            }
        }
        else {
            // Handle mouse for UI screens
            if (e->type == ButtonPress || e->type == ButtonRelease) {
                if (screenManager.handleMouse(e->xbutton.x, e->xbutton.y, e->xbutton.button)) {
                    return;
                }
            }

            // Track mouse position for menu hover effects
            if (savex != e->xbutton.x || savey != e->xbutton.y) {
                savex = e->xbutton.x;
                savey = e->xbutton.y;

                // Pass mouse movement to screen manager
                screenManager.handleMouse(savex, savey, 0);
            }
        }
    }

    int check_keys(XEvent *e)
    {
        static int shift=0;
        if (e->type != KeyRelease && e->type != KeyPress) {
            //not a keyboard event
            return 0;
        }
        int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
        //Log("key: %i\n", key);
        if (e->type == KeyRelease) {
            gl.keys[key] = 0;
            if (key == XK_Shift_L || key == XK_Shift_R)
                shift = 0;
            return 0;
        }
        if (e->type == KeyPress) {
            //std::cout << "press" << std::endl;
            gl.keys[key]=1;
            if (key == XK_Shift_L || key == XK_Shift_R) {
                shift = 1;
                return 0;
            }
        }
        if (screenManager.handleKey(key)) {
            return 0;
        }

        // Only process these keys in game state
        if (screenManager.getState() == GAME) {
            (void)shift;
            switch (key) {
                case XK_Escape:
                    screenManager.setState(MENU);
                    return 1;
                case XK_m:
                    gl.mouse_cursor_on = !gl.mouse_cursor_on;
                    x11.show_mouse_cursor(gl.mouse_cursor_on);
                    break;
                case XK_s:
                    break;
                case XK_Down:
                    break;
                case XK_equal:
                    break;
                case XK_minus:
                    break;
            }
        }
        return 0;
    }

    void deleteAsteroid(Game *g, Asteroid *node)
    {
        //Remove a node from doubly-linked list
        //Must look at 4 special cases below.
        if (node->prev == NULL) {
            if (node->next == NULL) {
                //only 1 item in list.
                g->ahead = NULL;
            } else {
                //at beginning of list.
                node->next->prev = NULL;
                g->ahead = node->next;
            }
        } else {
            if (node->next == NULL) {
                //at end of list.
                node->prev->next = NULL;
            } else {
                //in middle of list.
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
        }
        delete node;
        node = NULL;
    }

    void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
    {
        //build ta from a
        ta->nverts = 8;
        ta->radius = a->radius / 2.0;
        Flt r2 = ta->radius / 2.0;
        Flt angle = 0.0f;
        Flt inc = (PI * 2.0) / (Flt)ta->nverts;
        for (int i=0; i<ta->nverts; i++) {
            ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
            ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
            angle += inc;
        }
        ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
        ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
        ta->pos[2] = 0.0f;
        ta->angle = 0.0;
        ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
        ta->color[0] = 0.8;
        ta->color[1] = 0.8;
        ta->color[2] = 0.7;
        ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
        ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
        //std::cout << "frag" << std::endl;
    }

    void physics()
    {

        Flt d0,d1,dist;
        //Update ship position
        g.ship.pos[0] += g.ship.vel[0];
        g.ship.pos[1] += g.ship.vel[1];
        //Check for collision with window edges
        if (g.ship.pos[0] < 0.0) {
            g.ship.pos[0] += (float)gl.xres;
        }
        else if (g.ship.pos[0] > (float)gl.xres) {
            g.ship.pos[0] -= (float)gl.xres;
        }
        else if (g.ship.pos[1] < 0.0) {
            g.ship.pos[1] += (float)gl.yres;
        }
        else if (g.ship.pos[1] > (float)gl.yres) {
            g.ship.pos[1] -= (float)gl.yres;
        }

        //Update bullet positions
        struct timespec bt;
        clock_gettime(CLOCK_REALTIME, &bt);
        int i = 0;
        while (i < g.nbullets) {
            Bullet *b = &g.barr[i];
            //How long has bullet been alive?
            double ts = timeDiff(&b->time, &bt);
            if (ts > 2.5) {
                //time to delete the bullet.
                memcpy(&g.barr[i], &g.barr[g.nbullets-1],
                        sizeof(Bullet));
                g.nbullets--;
                //do not increment i.
                continue;
            }
            //move the bullet
            b->pos[0] += b->vel[0];
            b->pos[1] += b->vel[1];
            //Check for collision with window edges
            if (b->pos[0] < 0.0) {
                b->pos[0] += (float)gl.xres;
            }
            else if (b->pos[0] > (float)gl.xres) {
                b->pos[0] -= (float)gl.xres;
            }
            else if (b->pos[1] < 0.0) {
                b->pos[1] += (float)gl.yres;
            }
            else if (b->pos[1] > (float)gl.yres) {
                b->pos[1] -= (float)gl.yres;
            }
            ++i;
        }
        //
        //Update asteroid positions
        Asteroid *a = g.ahead;
        while (a) {
            a->pos[0] += a->vel[0];
            a->pos[1] += a->vel[1];
            //Check for collision with window edges
            if (a->pos[0] < -100.0) {
                a->pos[0] += (float)gl.xres+200;
            }
            else if (a->pos[0] > (float)gl.xres+100) {
                a->pos[0] -= (float)gl.xres+200;
            }
            else if (a->pos[1] < -100.0) {
                a->pos[1] += (float)gl.yres+200;
            }
            else if (a->pos[1] > (float)gl.yres+100) {
                a->pos[1] -= (float)gl.yres+200;
            }
            a->angle += a->rotate;
            a = a->next;
        }
        //
        //Asteroid collision with bullets?
        //If collision detected:
        //     1. delete the bullet
        //     2. break the asteroid into pieces
        //        if asteroid small, delete it
        a = g.ahead;
        while (a) {
            //is there a bullet within its radius?
            int i=0;
            while (i < g.nbullets) {
                Bullet *b = &g.barr[i];
                d0 = b->pos[0] - a->pos[0];
                d1 = b->pos[1] - a->pos[1];
                dist = (d0*d0 + d1*d1);
                if (dist < (a->radius*a->radius)) {
                    //std::cout << "asteroid hit." << std::endl;
                    //this asteroid is hit.
                    if (a->radius > MINIMUM_ASTEROID_SIZE) {
                        //break it into pieces.
                        Asteroid *ta = a;
                        buildAsteroidFragment(ta, a);
                        int r = rand()%10+5;
                        for (int k=0; k<r; k++) {
                            //get the next asteroid position in the array
                            Asteroid *ta = new Asteroid;
                            buildAsteroidFragment(ta, a);
                            //add to front of asteroid linked list
                            ta->next = g.ahead;
                            if (g.ahead != NULL)
                                g.ahead->prev = ta;
                            g.ahead = ta;
                            g.nasteroids++;
                        }
                    } else {
                        a->color[0] = 1.0;
                        a->color[1] = 0.1;
                        a->color[2] = 0.1;
                        //asteroid is too small to break up
                        //delete the asteroid and bullet
                        Asteroid *savea = a->next;
                        deleteAsteroid(&g, a);
                        a = savea;
                        g.nasteroids--;
                    }
                    //delete the bullet...
                    memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
                    g.nbullets--;
                    if (a == NULL)
                        break;
                }
                i++;
            }
            if (a == NULL)
                break;
            a = a->next;
        }
        //---------------------------------------------------
        //check keys pressed now
        /*
           if (gl.keys[XK_Left]) {
           g.ship.angle += 4.0;
           if (g.ship.angle >= 360.0f)
           g.ship.angle -= 360.0f;
           g.ship.vel[1] = 4.0f;
           }
           if (gl.keys[XK_Right]) {
           g.ship.angle -= 4.0;
           if (g.ship.angle < 0.0f)
           g.ship.angle += 360.0f;
           g.ship.vel[0]= 4.0f;
           }

           if (gl.keys[XK_Up]) {
        //apply thrust
        //convert ship angle to radians
        //Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
        //convert angle to a vector
        //Flt xdir = cos(rad);
        //Flt ydir = sin(rad);
        Flt xdir = 0.0f;
        Flt ydir = 1.0f;

        g.ship.vel[0] += xdir*0.02f;
        g.ship.vel[1] += ydir*0.02f;
        Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
        g.ship.vel[1]*g.ship.vel[1]);
        if (speed > 10.0f) {
        speed = 10.0f;
        normalize2d(g.ship.vel);
        g.ship.vel[0] *= speed;
        g.ship.vel[1] *= speed;
        }
        }*/
        //static bool initialized = false;
        //----------------------------------------------
        //Movement Added by Sayed Jalal
        g.ship.vel[0] = 0.0f;
        g.ship.vel[1] = 0.0f;

        // Static movement along X and Y axis
        if (gl.keys[XK_Up]) {
            g.ship.vel[1] = 1.0f;
        }

        if (gl.keys[XK_Down]) {
            g.ship.vel[1] = -1.0f;
        }

        if (gl.keys[XK_Left]) {
            g.ship.vel[0] = -1.0f;
        }

        if (gl.keys[XK_Right]) {
            g.ship.vel[0] = 1.0f;
        }
        //-----------------------------------------------
        float newX = g.ship.pos[0] + g.ship.vel[0];
        float newY = g.ship.pos[1] + g.ship.vel[1];

        if (!maze.isWall(newX, g.ship.pos[1], 14.5, gl.xres, gl.yres)) {
            g.ship.pos[0] = newX;
        } else {
            g.ship.vel[0] = 0.0f; // Stop X movement
        }

        // Try moving vertically
        if (!maze.isWall(g.ship.pos[0], newY, 14.5, gl.xres, gl.yres)) {
            g.ship.pos[1] = newY;
        } else {
            g.ship.vel[1] = 0.0f; // Stop Y movement
        }



        if (gl.keys[XK_space]) {
            //a little time between each bullet
            struct timespec bt;
            clock_gettime(CLOCK_REALTIME, &bt);
            double ts = timeDiff(&g.bulletTimer, &bt);
            if (ts > 0.1) {
                timeCopy(&g.bulletTimer, &bt);
                if (g.nbullets < MAX_BULLETS) {
                    //shoot a bullet...
                    //Bullet *b = new Bullet;
                    Bullet *b = &g.barr[g.nbullets];
                    timeCopy(&b->time, &bt);
                    b->pos[0] = g.ship.pos[0];
                    b->pos[1] = g.ship.pos[1];
                    b->vel[0] = g.ship.vel[0];
                    b->vel[1] = g.ship.vel[1];
                    //convert ship angle to radians
                    Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
                    //convert angle to a vector
                    Flt xdir = cos(rad);
                    Flt ydir = sin(rad);
                    b->pos[0] += xdir*20.0f;
                    b->pos[1] += ydir*20.0f;
                    b->vel[0] += xdir*6.0f + rnd()*0.1;
                    b->vel[1] += ydir*6.0f + rnd()*0.1;
                    b->color[0] = 1.0f;
                    b->color[1] = 1.0f;
                    b->color[2] = 1.0f;
                    g.nbullets++;
                }
            }
        }
        if (g.mouseThrustOn) {
            //should thrust be turned off
            struct timespec mtt;
            clock_gettime(CLOCK_REALTIME, &mtt);
            double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
            //std::cout << "tdif: " << tdif << std::endl;
            if (tdif < -0.3)
                g.mouseThrustOn = false;
        }
    }

    static bool initialized = false;

    void render()
    {
        if (screenManager.getState() != GAME) {
            screenManager.render();
            return;
        }


        Rect r;
        mgonzalez3 obj;
        /* if (!initialized) {
           maze.generate(45, 33);
           initialized = true;
           } */
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* if (!initialized) {
           maze.generateWithExit(45, 33);
           initialized = true;
           } */

        if (!initialized) {
            if (buttonPressed[0]) {
                maze.generate(45, 33); // Basic maze
            } else if (buttonPressed[1]) {
                maze.generateWithExit(45, 33); // Maze with coin/exit logic
            } else if (buttonPressed[2]) {
                maze.Level3(45, 33); // Level 3 maze
            }
            initialized = true;
        }
        if (buttonPressed[0] == true) {
            buttonPressed[1] = false;
            buttonPressed[2] = false;
            maze.render(gl.xres, gl.yres); 
        }

        if (buttonPressed[2] == true) {
            buttonPressed[0] = false;
            buttonPressed[1] = false;
            maze.renderLevel3(gl.xres, gl.yres); 
        }

        if (buttonPressed[1] == true) {
            buttonPressed[0] = false;
            buttonPressed[2] = false;
            maze.render(gl.xres, gl.yres); 
            if (gl.fullscreen == 0) {
                //g.ship.pos[0] = gl.xres / 2;
                //g.ship.pos[1] = gl.yres / 2;
                coin.coinCollect((gl.xres/2) + 305, (gl.xres /2) + 305, 
                        (gl.xres/2) - 305, (gl.xres/2) - 305, (gl.yres/2)-215, 
                        (gl.yres/2)+215, (gl.yres/2) + 215, 
                        (gl.yres/2) - 215, g.ship.pos[0], g.ship.pos[1]); 
            }
            if (gl.fullscreen == 1) {
                //g.ship.pos[0] = gl.xres / 2;
                //g.ship.pos[1] = gl.yres / 2;
                coin.coinCollect((gl.xres/2) + 305, (gl.xres /2) + 305, 
                        (gl.xres/2) - 305, (gl.xres/2) - 305, 
                        (gl.yres/2) - 215, (gl.yres/2)+215, (gl.yres/2) + 215, 
                        (gl.yres/2) - 215, g.ship.pos[0], g.ship.pos[1]); 
            }
        } 
        if (!buttonPressed[0]) {
            resetStickman();
        }
        if (buttonPressed[0]) {
            DrawStickman o;
            o.Stickman(g.ship.pos[0],g.ship.pos[1],g.ship.pos[2],g.ship.angle);
        } else {
            resetStickman();
        }
        if (buttonPressed[1]) {
            DrawShip o2;
            o2.Ship(g.ship.pos[0],g.ship.pos[1],g.ship.pos[2],g.ship.angle);
        }
        if (buttonPressed[2]) {
            DrawShip o2;
            o2.Ship(g.ship.pos[0],g.ship.pos[1],g.ship.pos[2],g.ship.angle);
        } 

        obj.lighting(g.ship.pos[0],g.ship.pos[1], 100.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        r.bot = gl.yres - 15;
        r.left = 10;
        r.center = 0;
        //ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
        ggprint8b(&r, 16, 0x00ff0000, "Coins Collected: %i", coinsCollected);
        //ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);
        //-------------------------------------------------------------------------
        //Draw the ship
        //obj.lighting(g.ship.pos[0],g.ship.pos[1], 100.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        //o2.Ship(g.ship.pos[0],g.ship.pos[1],g.ship.pos[2],g.ship.angle);
        //
        drawButton(10, 60, 90, 85, "Vol Up");
        drawButton(10, 25, 90, 50, "Vol Down");
        drawVolumeBar(currentVolume);
    }
