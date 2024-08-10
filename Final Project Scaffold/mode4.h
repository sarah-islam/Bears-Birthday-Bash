#ifndef GBA_MODE4_H
#define GBA_MODE4_H

#include "gba.h"

#define FRONTBUFFER ((volatile unsigned short*) 0x6000000) // Beginning of front buffer
#define BACKBUFFER  ((volatile unsigned short*) 0x600A000) // Beginning of back buffer

#define REG_BG2CNT (*(volatile unsigned short*) 0x400000C) // Background 2 control register

#define SET_MODE4() (REG_DISPCTL = MODE(4) & BG2_ENABLE) // Sets REG_DISPCTL to mode 4

void flipPage();
void setPixel4(int x, int y, unsigned char colorIndex);
void drawRect4(int x, int y, int width, int height, volatile unsigned char colorIndex);
void fillScreen4(volatile unsigned char colorIndex);
void drawImage4(int x, int y, int width, int height, const unsigned short *image);
void drawFullscreenImage4(const unsigned short *image);

#endif