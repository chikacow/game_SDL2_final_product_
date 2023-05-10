//file .h khi bien dich ko can add vao project van se chay dc, co the la do da include ten file, con file cpp bat buoc phai add vao project
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
#include <vector>


static SDL_Window* g_window = NULL;  // thêm static để dùng được ở mọi chỗ
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Chunk* g_sound_bullet; // 2 tieng cho 2 loai dan
static Mix_Chunk* g_sound_player[10];
static Mix_Chunk* g_sound_exp[5];
static Mix_Chunk* g_theme_music[5];
static Mix_Chunk* g_general_music[5];

static SDL_Surface* g_img_menu = NULL;


//screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 64;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;

const int FRAME_PER_SECOND = 25; //fps, ở đây hiểu là 1 giây load được 25 khung hình

#define TILE_SIZE 64 // kích thước của 1 ô

#define MAX_MAP_X 400 //chiều ngang là 400 ô
#define MAX_MAP_Y 10  // chiều dọc là 10 ô
#define BLANK_TILE 0 // giá trị của ô trống = 0

#define STATE_MONEY 4
#define STATE_TRAP 5

#define NUM_LIFE 3 // nhan vat co 3 mang

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

namespace SDLCommonFunction
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);
    //int  ShowMenu (SDL_Renderer* des , TTF_Font* font);


}







#endif // COMMON_FUNCTION_H
