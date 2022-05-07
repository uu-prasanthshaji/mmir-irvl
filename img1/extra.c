#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <graphics.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void initwindow(int x, int y)
{
  if(y>=0x10000 || x>=0x10000) {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    return;
  }
  int gd = USER;
  int gm = (x<<16)+y;
  initgraph(&gd, &gm, NULL);
}

void putrgbpixel(int x, int y, uint8_t* color)
{
  uint8_t new_color = (color[0] & 0xe0) | ((color[1] & 0xe0)>>3) | ((color[2] & 0xc0) >> 6);
  if(new_color) {
    uint8_t old_color = _fgcolor;
    _fgcolor = new_color;
    bar(x, y, x, y);
    _fgcolor = old_color;
  }
}

void put8pixel(int x, int y, uint8_t color)
{
  if(color) {
    uint8_t old_color = _fgcolor;
    _fgcolor = color;
    bar(x, y, x, y);
    _fgcolor = old_color;
  }
}

void delayandclose(float d)
{
  delay(d);
  closegraph();
}

void showrgbimage(int width, int height, uint8_t* img, float delay)
{
  initwindow(width, height);
  for(int j=0; j<height; j++) {
    for(int i=0; i<width; i++) {
      putrgbpixel(i, j, img+(i+j*width)*3);
    }
  }
  delayandclose(delay);
}

void show8image(int width, int height, uint8_t* img, float delay)
{
  initwindow(width, height);
  for(int j=0; j<height; j++) {
    for(int i=0; i<width; i++) {
      put8pixel(i, j, img[i+j*width]);
    }
  }
  delayandclose(delay);
}

void showscale8image(int width, int height, uint8_t* img, float delay)
{
  int scale = 1;
  {
    int t_width = width;
    int t_height = height;
    while(t_width > 800 || t_height > 600) {
      scale++;
      t_width = width/scale;
      t_height = height/scale;
    }
  }
  int swidth = width/scale;
  int sheight = height/scale;
  int scalearea = scale*scale;
  initwindow(swidth, sheight);
  for(int j=0; j<sheight; j++) {
    for(int i=0; i<swidth; i++) {
      int red = 0;
      int green = 0;
      int blu = 0;
      for(int k=0; k<scale; k++) {
        for(int l=0; l<scale; l++) {
          uint8_t color = img[i*scale+l+(j*scale+k)*width];
          red += (color & 0xe0) >> 5;
          green += (color & 0x1c) >> 2;
          blu += (color & 0x3);
        }
      }
      uint8_t color = ((red/scalearea) << 5) | ((green/scalearea) << 2) | (blu/scalearea);
      put8pixel(i, j, color);
    }
  }
  delayandclose(delay);
}