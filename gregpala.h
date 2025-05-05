//Author: Geneva Regpala
//
// gregpala.h
// Header file for the game screens (Loading, Menu, Credits)
//

#ifndef GREGPALA_H
#define GREGPALA_H

#include <GL/glx.h>
#include <cmath>
#include <vector>
#include "fonts.h"

// Enum for different game states
enum GameState {
    LOADING,
    MENU,
    CREDITS,
    GAME
};

// Base GameScreen class
class GameScreen {
public:
    virtual ~GameScreen() {}
    virtual void update() = 0;
    virtual void render() = 0;
    virtual int handleMouse(int x, int y, int button) = 0;
    virtual int handleKey(int key) = 0;
};

// Button class for menu items
class Button {
private:
    int x, y, width, height;
    const char* text;
    bool hover;

public:
    Button(int x, int y, int width, int height, const char* text);
    void render();
    bool isClicked(int mouseX, int mouseY, int screenHeight);
    void setHover(bool h) { hover = h; }
    bool getHover() { return hover; }
    void updatePosition(int newX, int newY, int newWidth, int newHeight);
};

// Loading Screen class
class LoadingScreen : public GameScreen {
private:
    float progress;
    float targetProgress;
    float rotation;
    struct timespec startTime;
    float loadingDuration; // seconds
    bool finished;

public:
    LoadingScreen();
    ~LoadingScreen() {}
    void update();
    void render();
    int handleMouse(int x, int y, int button);
    int handleKey(int key);
    bool isFinished() { return finished; }
};

// Menu Screen class
class MenuScreen : public GameScreen {
private:
    std::vector<Button> buttons;
    int selectedButton;
    float titlePulse;
    float titlePulseDir;

public:
    MenuScreen();
    ~MenuScreen() {}
    void update();
    void render();
    int handleMouse(int x, int y, int button);
    int handleKey(int key);
    void resize(int scrWidth, int scrHeight);
};

// Credits Screen class
class CreditsScreen : public GameScreen {
private:
    float scrollY;
    Button backButton;

public:
    CreditsScreen();
    ~CreditsScreen() {}
    void update();
    void render();
    int handleMouse(int x, int y, int button);
    int handleKey(int key);
    void resize(int scrWidth, int scrHeight);
};

// Game Screen Manager
class ScreenManager {
private:
    GameState currentState;
    LoadingScreen* loadingScreen;
    MenuScreen* menuScreen;
    CreditsScreen* creditsScreen;
    int screenWidth;
    int screenHeight;
void resize(int scrWidth, int scrHeight);
public:
    ScreenManager(int width = 640, int height = 480);
    ~ScreenManager();
    void update();
    void render();
    int handleMouse(int x, int y, int button);
    int handleKey(int key);
    GameState getState() { return currentState; }
    void setState(GameState state) { currentState = state; }
    int getScreenWidth() { return screenWidth; }
    int getScreenHeight() { return screenHeight; }
    void setScreenDimensions(int width, int height);
};

extern ScreenManager screenManager;

#endif // GREGPALA_Hvoid resize(int scrWidth, int scrHeight);void resize(int scrWidth, int scrHeight);
