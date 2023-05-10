
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "CommonFunction.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    //Set kich thuoc anh
    void SetRect( const int& x, const int& y)
    {
        rect_.x = x;
        rect_.y = y;
    }
    SDL_Rect GetRect() const {return rect_;}
    void SetRectwh ( const int&w, const int& h)
    {
        rect_.w = w;
        rect_.h = h;
    }
    SDL_Texture* GetObject() const {return p_object_;}
    // ham load anh bitmap len p_object_ roi dung no de fill vao screen

   virtual bool LoadImg(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();
   // int ShowMenu(SDL_Surface* des, TTF_Font* font);

protected:
    SDL_Texture* p_object_;         // Bien luu tru image
    SDL_Rect rect_;                // Bien luu giu kich thuoc anh bao gồm x y w h


};

#endif // BASE_OBJECT_H
