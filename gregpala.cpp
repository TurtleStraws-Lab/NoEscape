// Author: Geneva Regpala
//
// Compiles but doesn't function properly
// Pushing code for lab 
// Will adjust the functionality soon
// 

#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <string>

class MenuScreen {
private:
    GLuint backgroundTexture;
    int windowWidth;
    int windowHeight;
    
    // Play button properties
    float playButtonX;
    float playButtonY;
    float playButtonWidth;
    float playButtonHeight;
    bool playButtonHovered;
    bool playButtonClicked;
    
    // Load JPG texture
    GLuint loadTexture(const char* filename) {
        GLuint texture;
        
        // Load image file
        FILE* file = fopen(filename, "rb");
        if (!file) {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            return 0;
        }
        
        // Get file size
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        // Allocate memory for file data
        unsigned char* data = new unsigned char[fileSize];
        fread(data, fileSize, 1, file);
        fclose(file);
        
        // Create OpenGL texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Load texture data
        // Note: In a real implementation, you would use a proper image loading library
        // like stb_image or use the JPG loading code to convert to raw pixels
        // For simplicity, we're just creating a placeholder texture
        unsigned char* pixels = new unsigned char[256 * 256 * 3];
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                pixels[(y * 256 + x) * 3 + 0] = (x + y) % 256;  // R
                pixels[(y * 256 + x) * 3 + 1] = x % 256;        // G
                pixels[(y * 256 + x) * 3 + 2] = y % 256;        // B
            }
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        delete[] pixels;
        delete[] data;
        
        return texture;
    }
    
    // Check if point is inside rectangle
    bool isPointInRectangle(float x, float y, float rectX, float rectY, float rectWidth, float rectHeight) {
        return (x >= rectX && x <= rectX + rectWidth && 
                y >= rectY && y <= rectY + rectHeight);
    }
    
public:
    MenuScreen(int windowWidth, int windowHeight) : 
        windowWidth(windowWidth), 
        windowHeight(windowHeight),
        playButtonHovered(false),
        playButtonClicked(false) {
        
        // Load background texture
        backgroundTexture = loadTexture("MenuScreen.jpg");
        
        // Setup play button
        playButtonWidth = 200;
        playButtonHeight = 80;
        playButtonX = (windowWidth - playButtonWidth) / 2;
        playButtonY = (windowHeight - playButtonHeight) / 2;
    }
    
    ~MenuScreen() {
        // Clean up textures
        glDeleteTextures(1, &backgroundTexture);
    }
    
    void resize(int width, int height) {
        windowWidth = width;
        windowHeight = height;
        
        // Recalculate button position
        playButtonX = (windowWidth - playButtonWidth) / 2;
        playButtonY = (windowHeight - playButtonHeight) / 2;
    }
    
    void mouseMove(int x, int y) {
        // OpenGL has y=0 at bottom, but most window systems have y=0 at top
        y = windowHeight - y;
        
        // Check if mouse is over play button
        playButtonHovered = isPointInRectangle(x, y, playButtonX, playButtonY, 
                                             playButtonWidth, playButtonHeight);
    }
    
    void mouseClick(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // OpenGL has y=0 at bottom, but most window systems have y=0 at top
            y = windowHeight - y;
            
            if (isPointInRectangle(x, y, playButtonX, playButtonY, 
                                  playButtonWidth, playButtonHeight)) {
                playButtonClicked = true;
            }
        }
    }
    
    void draw() {
        // Save current matrix
        glPushMatrix();
        
        // Set up orthographic projection for 2D rendering
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, windowWidth, 0, windowHeight);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Disable depth testing for 2D
        glDisable(GL_DEPTH_TEST);
        
        // Draw background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(windowWidth, 0);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(windowWidth, windowHeight);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0, windowHeight);
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
        
        // Draw play button
        if (playButtonHovered) {
            glColor4f(0.6f, 0.6f, 1.0f, 0.8f);  // Light blue when hovered
        } else {
            glColor4f(0.4f, 0.4f, 0.8f, 0.8f);  // Dark blue normally
        }
        
        glBegin(GL_QUADS);
            glVertex2f(playButtonX, playButtonY);
            glVertex2f(playButtonX + playButtonWidth, playButtonY);
            glVertex2f(playButtonX + playButtonWidth, playButtonY + playButtonHeight);
            glVertex2f(playButtonX, playButtonY + playButtonHeight);
        glEnd();
        
        // Draw button outline
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(playButtonX, playButtonY);
            glVertex2f(playButtonX + playButtonWidth, playButtonY);
            glVertex2f(playButtonX + playButtonWidth, playButtonY + playButtonHeight);
            glVertex2f(playButtonX, playButtonY + playButtonHeight);
        glEnd();
        
        // Draw "PLAY" text
        //
      
        float textX = playButtonX + playButtonWidth / 2.0f - 30.0f;
        float textY = playButtonY + playButtonHeight / 2.0f - 10.0f;
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(3.0f);
        
        // 'P'
        glBegin(GL_LINE_STRIP);
            glVertex2f(textX, textY);
            glVertex2f(textX, textY + 20.0f);
            glVertex2f(textX + 10.0f, textY + 20.0f);
            glVertex2f(textX + 15.0f, textY + 15.0f);
            glVertex2f(textX + 10.0f, textY + 10.0f);
            glVertex2f(textX, textY + 10.0f);
        glEnd();
        
        // 'L'
        textX += 20.0f;
        glBegin(GL_LINE_STRIP);
            glVertex2f(textX, textY + 20.0f);
            glVertex2f(textX, textY);
            glVertex2f(textX + 10.0f, textY);
        glEnd();
        
        // 'A'
        textX += 15.0f;
        glBegin(GL_LINE_STRIP);
            glVertex2f(textX, textY);
            glVertex2f(textX + 7.5f, textY + 20.0f);
            glVertex2f(textX + 15.0f, textY);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(textX + 3.0f, textY + 8.0f);
            glVertex2f(textX + 12.0f, textY + 8.0f);
        glEnd();
        
        // 'Y'
        textX += 20.0f;
        glBegin(GL_LINE_STRIP);
            glVertex2f(textX, textY + 20.0f);
            glVertex2f(textX + 7.5f, textY + 10.0f);
            glVertex2f(textX + 15.0f, textY + 20.0f);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(textX + 7.5f, textY + 10.0f);
            glVertex2f(textX + 7.5f, textY);
        glEnd();
        
        // Restore OpenGL state
        glEnable(GL_DEPTH_TEST);
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    
    bool isPlayClicked() const {
        return playButtonClicked;
    }
    
    void resetPlayClicked() {
        playButtonClicked = false;
    }
};

#endif // MENU_SCREEN_H