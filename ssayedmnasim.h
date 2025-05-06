#ifndef SSAYEDMNASIM_H
#define SSAYEDMNASIM_H
void volumeUp();
void volumeDown();
//void drawButton(float x1, float y1, float x2, float y2, const char* label, bool hovered = false);
void drawButton(float x1, float y1, float x2, float y2, const char* label);
bool initSound();
void startBackgroundSound();
void shutdownSound();

#endif
