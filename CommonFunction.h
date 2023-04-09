
#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <iostream>
#include <windows.h>
#include <string>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 64;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;

const int FRAME_PER_SECOND = 25; //fps

#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define BLANK_TILE 0

typedef struct Map
{
    int start_x_;  // Vi tri cua o trong file .dat (notepad)
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;

};

typedef struct Input
{
  int left_;
  int right_;
  int up_;
  int down_;
  int jump_;

};

#endif // COMMON_FUNCTION_H
