
#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunction.h"
#include "BaseObject.h"

class TextObject
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };

   // bool LoadFromFile(std::string path);
    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    void Free();
    void SetRect(const int& xp, const int& yp) {rect_.x = xp; rect_.y = yp;}

    void  SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);

    void RenderText(SDL_Renderer* screen, int xp,
                    int yp,
                    SDL_Rect* clip = NULL,
                    double angle = 0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);
    void RenderTextMenu(SDL_Renderer* screen,
                    SDL_Rect* clip = NULL,
                    double angle = 0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

    bool LoadFromRenderTextMenu(TTF_Font* font, SDL_Renderer* screen);
    int GetWidth() const {return width_;}
    int GetHeight() const {return height_;}
    void SetText (const std::string& text) { str_val_ = text; }
    std::string GetText() {return str_val_;}
    SDL_Rect GetRect() {return rect_;}

private:
    std::string str_val_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    int width_;
    int height_;
    SDL_Rect rect_;
};

class WinData
{

public:
    WinData();
    ~WinData();

    std::string money_win_;
    std::string mark_win_;
    std::string time_win_;
};

namespace Menu {
    // handle menu
    int ShowMenu(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* theme_sound[5]);
    int ShowRestart(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]);
    int ShowRestartFall(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]);
    int ShowRestartTimeOver(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]);
    int ShowMenuWin(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5], WinData data_win);
    int ShowTutorial(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]);
    int ShowPause(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]);
}




#endif // TEXT_OBJECT_H_


