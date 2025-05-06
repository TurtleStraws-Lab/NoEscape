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
    LEVELS,  // New state for levels menu
    GAME
};

// Enum for tracking selected level
enum GameLevel {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3
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
    
    // New members for visual enhancements
    struct FloatingPixel {
        float x, y;
        float dx, dy;     // Direction vectors for movement
        float speed;
        float color[3];
        float size;       // Property for variable pixel sizes
    };
    static const int NUM_PIXELS = 500;  // Increased from 100 to 500
    FloatingPixel pixels[NUM_PIXELS];
    
    float titleY; // For dropping title animation
    float titleTargetY; // Target Y position for title
    float shipAngle; // For ship rotation animation

public:
    LoadingScreen();
    ~LoadingScreen() {}
    void update();
    void render();
    int handleMouse(int x, int y, int button);
    int handleKey(int key);
    bool isFinished() { return finished; }
    void resize(int scrWidth, int scrHeight);
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

// New Levels Screen class
class LevelsScreen : public GameScreen {
private:
    std::vector<Button> levelButtons;
    Button backButton;
    int selectedButton;
    float titlePulse;
    float titlePulseDir;

public:
    LevelsScreen();
    ~LevelsScreen() {}
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
    GameLevel selectedLevel;
    LoadingScreen* loadingScreen;
    MenuScreen* menuScreen;
    CreditsScreen* creditsScreen;
    LevelsScreen* levelsScreen; // New screen
    int screenWidth;
    int screenHeight;
    
    // Added boolean variables for level selection
    bool isLevel1Active;
    bool isLevel2Active;
    bool isLevel3Active;

public:
    ScreenManager(int width = 640, int height = 480);
    ~ScreenManager();
    void update();
    void render();
    int handleMouse(int x, int y, int button);
    int handleKey(int key);
    GameState getState() { return currentState; }
    void setState(GameState state) { currentState = state; }
    GameLevel getSelectedLevel() { return selectedLevel; }
    void setSelectedLevel(GameLevel level); // Changed to non-inline
    int getScreenWidth() { return screenWidth; }
    int getScreenHeight() { return screenHeight; }
    void setScreenDimensions(int width, int height);
    
    // Added accessor methods for level activation
    bool isLevelOneActive() const { return isLevel1Active; }
    bool isLevelTwoActive() const { return isLevel2Active; }
    bool isLevelThreeActive() const { return isLevel3Active; }
};

extern ScreenManager screenManager;

#endif // GREGPALA_H
