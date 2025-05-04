//Author: Geneva Regpala
//
// Implementation of game screens (Loading, Menu, Credits)
// Working on credits fixes

#include <iostream>
#include <ctime>
#include <unistd.h>
#include "gregpala.h"

// Initialize screen manager as a global object with default dimensions
ScreenManager screenManager;


// Loading Screen Implementation

LoadingScreen::LoadingScreen() {
    progress = 0.0f;
    targetProgress = 0.0f;
    rotation = 0.0f;
    loadingDuration = 3.0f; // 3 second loading sequence
    finished = false;
    
    // Start the timer
    clock_gettime(CLOCK_REALTIME, &startTime);
}

void LoadingScreen::update() {
    // Update the rotation for animation
    rotation += 2.0f;
    if (rotation > 360.0f)
        rotation -= 360.0f;
    
    // Calculate elapsed time
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    double elapsed = (currentTime.tv_sec - startTime.tv_sec) + 
                     (currentTime.tv_nsec - startTime.tv_nsec) / 1.0e9;
    
    // Update progress based on elapsed time
    targetProgress = (elapsed / loadingDuration) * 100.0f;
    if (targetProgress > 100.0f)
        targetProgress = 100.0f;
    
    // Smooth progress bar
    progress += (targetProgress - progress) * 0.05f;
    
    // Check if loading is complete
    if (elapsed >= loadingDuration && progress >= 99.0f) {
        finished = true;
    }
}

void LoadingScreen::render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    int scrWidth = screenManager.getScreenWidth();
    int scrHeight = screenManager.getScreenHeight();
    
    // Draw title
    Rect r;
    r.bot = scrHeight - 120;
    r.left = scrWidth/2;
    r.center = 1;
    ggprint16(&r, 0, 0x00ffffff, "NO ESCAPE");
    
    // Draw loading text
    r.bot = scrHeight/2 + 50;
    r.left = scrWidth/2;
    r.center = 1;
    ggprint8b(&r, 0, 0x00ffff00, "LOADING...");
    
    // Draw progress percentage
    r.bot = scrHeight/2 - 60;
    r.left = scrWidth/2;
    r.center = 1;
    char progressText[32];
    snprintf(progressText, sizeof(progressText), "%.0f%%", progress);
    ggprint8b(&r, 0, 0x00ffff00, progressText);
    
    // Draw loading bar outline
    int barWidth = scrWidth * 0.6;
    int barHeight = 20;
    int barLeft = (scrWidth - barWidth) / 2;
    int barTop = scrHeight/2 - 20;
    
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(barLeft, barTop);
    glVertex2f(barLeft + barWidth, barTop);
    glVertex2f(barLeft + barWidth, barTop - barHeight);
    glVertex2f(barLeft, barTop - barHeight);
    glEnd();
    
    // Draw loading bar fill
    float fillWidth = (progress / 100.0f) * barWidth;
    glColor3f(0.2f, 0.7f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(barLeft, barTop);
    glVertex2f(barLeft + fillWidth, barTop);
    glVertex2f(barLeft + fillWidth, barTop - barHeight);
    glVertex2f(barLeft, barTop - barHeight);
    glEnd();
    
    
}

int LoadingScreen::handleMouse(int x, int y, int button) {
    // Unused parameters, suppressing warnings
    (void)x;
    (void)y;
    (void)button;
    return 0;
}

int LoadingScreen::handleKey(int key) {
    // Skip loading on space or enter
    if (key == 32 || key == 13) { // 32 is space, 13 is enter
        progress = 100.0f;
        finished = true;
        return 1;
    }
    return 0;
}


// Button Implementation

Button::Button(int x, int y, int width, int height, const char* text) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->text = text;
    this->hover = false;
}

void Button::render() {
    // Draw button background
    if (hover) {
        glColor3f(0.4f, 0.4f, 0.9f); // Highlight color
    } else {
        glColor3f(0.2f, 0.2f, 0.7f); // Normal color
    }
    
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();
    
    // Draw button border
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();
    
    // Draw button text
    Rect r;
    r.bot = y - height/2 - 4;
    r.left = x + width/2;
    r.center = 1;
    ggprint16(&r, 0, 0x00ffffff, text);
}

bool Button::isClicked(int mouseX, int mouseY, int screenHeight) {
    // Convert mouse Y coordinate (from top) to our coordinate system (from bottom)
    int adjustedY = screenHeight - mouseY;
    
    return (mouseX >= x && mouseX <= x + width &&
            adjustedY <= y && adjustedY >= y - height);
}

// Menu Screen Implementation

MenuScreen::MenuScreen() {
    int scrWidth = screenManager.getScreenWidth();
    int scrHeight = screenManager.getScreenHeight();
    
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = (scrWidth - buttonWidth) / 2;
    
    // Add Play button
    buttons.push_back(Button(buttonX, scrHeight/2 + 30, buttonWidth, buttonHeight, "PLAY"));
    
    // Add Credits button
    buttons.push_back(Button(buttonX, scrHeight/2 - 50, buttonWidth, buttonHeight, "CREDITS"));
    
    selectedButton = 0;
    titlePulse = 1.0f;
    titlePulseDir = -0.01f;
}
void MenuScreen::resize(int scrWidth, int scrHeight) {
    // Recalculate button positions and sizes when window is resized
    float scaleX = scrWidth / 640.0f;
    float scaleY = scrHeight / 480.0f;

    int buttonWidth = 200 * scaleX;
    int buttonHeight = 50 * scaleY;
    int buttonX = (scrWidth - buttonWidth) / 2;

    // Update Play button
    if (buttons.size() > 0) {
        buttons[0].updatePosition(buttonX, scrHeight/2 + (30 * scaleY), buttonWidth, buttonHeight);
    }

    // Update Credits button
    if (buttons.size() > 1) {
        buttons[1].updatePosition(buttonX, scrHeight/2 - (50 * scaleY), buttonWidth, buttonHeight);
    }
}

void MenuScreen::update() {
    // Update title pulsing animation
    titlePulse += titlePulseDir;
    if (titlePulse <= 0.8f || titlePulse >= 1.2f) {
        titlePulseDir = -titlePulseDir;
    }
    
    // Reset all button hover states
    for (auto &button : buttons) {
        button.setHover(false);
    }
    
    // Set hover for selected button
    if (selectedButton >= 0 && selectedButton < (int)buttons.size()) {
        buttons[selectedButton].setHover(true);
    }
}

void MenuScreen::render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    int scrWidth = screenManager.getScreenWidth();
    int scrHeight = screenManager.getScreenHeight();
    
    // Draw title with pulsing effect
    glPushMatrix();
    glTranslatef(scrWidth/2, scrHeight - 120, 0);
    glScalef(titlePulse, titlePulse, 1.0f);
    
    Rect r;
    r.bot = 0;
    r.left = 0;
    r.center = 1;
    ggprint16(&r, 0, 0x00ffffff, "NO ESCAPE");
    glPopMatrix();
    
    
    // Draw buttons
    for (auto &button : buttons) {
        button.render();
    }
    
    // Draw version and copyright
    r.bot = 30;
    r.left = scrWidth/2;
    r.center = 1;
    ggprint8b(&r, 0, 0x00ffff00, "Version 1.0");
    
    r.bot = 15;
    ggprint8b(&r, 0, 0x0088ff88, "2025 NO ESCAPE TEAM 3");
}

int MenuScreen::handleMouse(int x, int y, int button) {
    int scrHeight = screenManager.getScreenHeight();
    
    if (button == 1) { // Left click
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].isClicked(x, y, scrHeight)) {
                if (i == 0) { // Play button
                    screenManager.setState(GAME);
                    return 1;
                } else if (i == 1) { // Credits button
                    screenManager.setState(CREDITS);
                    return 1;
                }
            }
        }
    }
    
    // Update hover states
    for (size_t i = 0; i < buttons.size(); i++) {
        if (buttons[i].isClicked(x, y, scrHeight)) {
            selectedButton = i;
            break;
        }
    }
    
    return 0;
}

int MenuScreen::handleKey(int key) {
    // Up/Down to navigate buttons
    if (key == 65362) { // Up arrow
        selectedButton = (selectedButton - 1 + buttons.size()) % buttons.size();
        return 1;
    } else if (key == 65364) { // Down arrow
        selectedButton = (selectedButton + 1) % buttons.size();
        return 1;
    } else if (key == 32 || key == 13) { // Space or Enter to select
        if (selectedButton == 0) { // Play button
            screenManager.setState(GAME);
            return 1;
        } else if (selectedButton == 1) { // Credits button
            screenManager.setState(CREDITS);
            return 1;
        }
    }
    
    return 0;
}

// Credits Screen Implementation

CreditsScreen::CreditsScreen()
    : scrollY(0.0f),
      backButton((screenManager.getScreenWidth() - 100) / 2,
                 100 * (screenManager.getScreenHeight() / 480.0f),
                 100 * (screenManager.getScreenWidth() / 640.0f),
                 40 * (screenManager.getScreenHeight() / 480.0f),
                 "BACK") {
}
void CreditsScreen::resize(int scrWidth, int scrHeight) {
    // Recalculate back button position and size
    float scaleX = scrWidth / 640.0f;
    float scaleY = scrHeight / 480.0f;

    int buttonWidth = 100 * scaleX;
    int buttonHeight = 40 * scaleY;
    int buttonX = (scrWidth - buttonWidth) / 2;
    int buttonY = 100 * scaleY;

    backButton.updatePosition(buttonX, buttonY, buttonWidth, buttonHeight);
}

void CreditsScreen::update() {
    // Auto-scroll credits
    /*scrollY -= 0.5f;
    
    // Reset scroll when credits have scrolled through
    if (scrollY < -600) {
        scrollY = screenManager.getScreenHeight();
    }*/
}
// working on fixes
void CreditsScreen::render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    int scrWidth = screenManager.getScreenWidth();
    int scrHeight = screenManager.getScreenHeight();
    
    // Draw title
    Rect r;
    r.bot = scrHeight - (80 * scaleY);
    r.left = scrWidth/2;
    r.center = 1;
    ggprint16(&r, 0, 0x00ffffff, "CREDITS");
    
    // Draw credits
    float baseY = scrHeight - (120 * scaleY);
    float lineSpacing = 30.0f * scaleY;
    
    r.bot = baseY;
    r.left = scrWidth/2;
    r.center = 1;
    ggprint16(&r, 0, 0x00ffff00, "NO ESCAPE");
    
    r.bot = baseY - lineSpacing;
    ggprint8b(&r, 0, 0x00ffffff, "Programming");
    
    r.bot = baseY - lineSpacing * 2;
    ggprint8b(&r, 0, 0x0088ff88, "Loading Screen/Menu");
    
    r.bot = baseY - lineSpacing * 3;
    ggprint8b(&r, 0, 0x00ffffff, "Geneva Regpala");
    
    r.bot = baseY - lineSpacing * 4;
    ggprint8b(&r, 0, 0x0088ff88, "Maze, Collision, Movement");
    
    r.bot = baseY - lineSpacing * 5;
    ggprint8b(&r, 0, 0x00ffffff, "Moises Gonzalez");
    
    r.bot = baseY - lineSpacing * 6;
    ggprint8b(&r, 0, 0x0088ff88, "Backgrounds and Cutscene");
    
    r.bot = baseY - lineSpacing * 7;
    ggprint8b(&r, 0, 0x00ffffff, "Eve Turallo");
    
    r.bot = baseY - lineSpacing * 8;
    ggprint8b(&r, 0, 0x0088ff88, "Ship/Stickman");

    r.bot = baseY - lineSpacing * 9;
    ggprint8b(&r, 0, 0x0088ff88, "Devin Vasquez");
    
    r.bot = baseY - lineSpacing * 10;
    ggprint8b(&r, 0, 0x00ffffff, "Sound");
    
    r.bot = baseY - lineSpacing * 11;
    ggprint8b(&r, 0, 0x0088ff88, "Sayed Jalal Sayed M Nasim");
    
    // Draw back button
    backButton.render();
}

int CreditsScreen::handleMouse(int x, int y, int button) {
    int scrHeight = screenManager.getScreenHeight();
    
    if (button == 1) { // Left click
        if (backButton.isClicked(x, y, scrHeight)) {
            screenManager.setState(MENU);
            return 1;
        }
    }
    
    // Update hover state
    backButton.setHover(backButton.isClicked(x, y, scrHeight));
    
    return 0;
}

int CreditsScreen::handleKey(int key) {
    // Esc or Backspace to go back to menu
    if (key == 27 || key == 8) { // 27 is Esc, 8 is Backspace
        screenManager.setState(MENU);
        return 1;
    }
    return 0;
}

// Screen Manager Implementation

ScreenManager::ScreenManager(int width, int height) {
    currentState = LOADING;
    screenWidth = width;
    screenHeight = height;
    loadingScreen = new LoadingScreen();
    menuScreen = new MenuScreen();
    creditsScreen = new CreditsScreen();
}

ScreenManager::~ScreenManager() {
    delete loadingScreen;
    delete menuScreen;
    delete creditsScreen;
}

void ScreenManager::setScreenDimensions(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    // Resize the menu and credits screens to match new dimensions
    menuScreen->resize(width, height);
    creditsScreen->resize(width, height);
}


void ScreenManager::update() {
    switch (currentState) {
        case LOADING:
            loadingScreen->update();
            if (loadingScreen->isFinished()) {
                currentState = MENU;
            }
            break;
        case MENU:
            menuScreen->update();
            break;
        case CREDITS:
            creditsScreen->update();
            break;
        case GAME:
            // Game update is handled in the main game loop
            break;
    }
}

void ScreenManager::render() {
    switch (currentState) {
        case LOADING:
            loadingScreen->render();
            break;
        case MENU:
            menuScreen->render();
            break;
        case CREDITS:
            creditsScreen->render();
            break;
        case GAME:
            // Game rendering is handled in the main game rendering function
            break;
    }
}

int ScreenManager::handleMouse(int x, int y, int button) {
    switch (currentState) {
        case LOADING:
            return loadingScreen->handleMouse(x, y, button);
        case MENU:
            return menuScreen->handleMouse(x, y, button);
        case CREDITS:
            return creditsScreen->handleMouse(x, y, button);
        case GAME:
            // Game mouse handling is done in main game loop
            return 0;
    }
    return 0;
}

int ScreenManager::handleKey(int key) {
    switch (currentState) {
        case LOADING:
            return loadingScreen->handleKey(key);
        case MENU:
            return menuScreen->handleKey(key);
        case CREDITS:
            return creditsScreen->handleKey(key);
        case GAME:
            // Game key handling is done in main game loop
            // But we can handle ESC to return to menu
            if (key == 27) { // ESC
                currentState = MENU;
                return 1;
            }
            return 0;
    }
    return 0;
}
