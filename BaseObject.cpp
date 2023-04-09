
#include "CommonFunction.h"
#include "BaseObject.h"

BaseObject::BaseObject()
{
    p_object_ = NULL;
    rect_.h = 0;
    rect_.w = 0;
    rect_.x = 0;
    rect_.y = 0;
}
BaseObject::~BaseObject()
{
    Free();


}
bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* new_texture = NULL;
    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if (load_surface != NULL)
    {

        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if (new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;

        }

        //SDL_FreeSurface(load_surface);
    }

    p_object_ = new_texture;
    return p_object_ != NULL;
}
void BaseObject::Render( SDL_Renderer* des, const SDL_Rect* clip)
{
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
   // SDL_Rect renderquad = {800, 300, 500, 300};

    SDL_RenderCopy(des, p_object_, clip, &renderquad); //day thong so p_object_ len des voi kich thuoc va vi tri trong renderquad
    // bản thân clip đã là con trỏ chưa địa chỉ rồi nên ko cần thêm dấu &
    // Hàm rendercopu để render texture với các tham số
    //SDL_RenderCopy(renderer, texture, &sourceRect, &desRect);
    //renderer: đối tượng được sử dụng để render
    // texture đối tượng texture muốn render
    // sourceRect: 1 phần nào đó của texture muốn render, nếu để NULL tức là render toàn bộ texture ( ở đây đã gắn sẵn clip = NULL)
    // desRect: hình chữ nhật xác định tọa độ mà texture muốn vẽ trên của sổ, nế chiều rộng và chiề cao lớn hơn hoặc nhỏ hơn texture muốn vẽ thì texture sẽ bị scale theo hình chữ nhật nà, nếu để NULL texture sẽ dược vẽ full lên màn hình
    // ở đây biến renderquad đóng vai trò desRect đã lưu chính xác thông số rect của p_object, tức là texture
}
void BaseObject::Free()
{
    if (p_object_ != NULL)
    {

    SDL_DestroyTexture(p_object_);
    p_object_ = NULL;
    rect_.w = 0;
    rect_.h = 0;
    }
}
























