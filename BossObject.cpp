
#include "BossObject.h"
#include "BaseObject.h"
#include "CommonFunction.h"

BossObject::BossObject()
{
    frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 1000;
    y_pos_ = 1000;
    width_frame_ = 0;
    height_frame_ = 0;
    think_time_ = 0;
    map_x_ = 1000;
    map_y_ = 1000;
}

BossObject::~BossObject()
{

}
bool BossObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret == true)
    {
        width_frame_ = rect_.w/FRAME_NUM_32;
        height_frame_ = rect_.h/1;
    }
    return ret;
}

void BossObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < FRAME_NUM_32; i++)
        {
            frame_clip_[i].x = width_frame_*i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void BossObject::Show(SDL_Renderer* des)
{
    if (think_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_; // tinh vi tri thuc su
        rect_.y = y_pos_ - map_y_;

        frame_++;

        if (frame_ > 32)
        {
            frame_ = 0;
        }

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

        if (currentClip != NULL)
        {
            renderQuad.w = currentClip->w;
            renderQuad.h = currentClip->h;
        }

        SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);

    }
}
void BossObject::DoBoss(Map& g_map)
{
    if (think_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

        if (y_val_ >= 10) // = Max_fall_speed
        {
            y_val_ = MAX_FALL_SPEED;
        }

        CheckMap(g_map);
        //CenterEntityOnMap(g_map);
    }

    if (think_time_ > 0)
    {
        think_time_ --;
        if (think_time_ == 0)
        {
            InitBoss();
        }
    }

}

void BossObject::InitBoss()
{
    x_val_ = 0;
    y_val_ = 0;

    if (x_pos_ > 256)
    {
        x_pos_ -= 256;
    }
    else
    {
        //x_pos_ = 0;
    }
    y_pos_ = 0;
    think_time_ = 0;

}


void BossObject::CheckMap(Map& g_map)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    on_ground_ = 0;

    // Check horizontal
    int height_min = height_frame_; //min between height_frame and tile_size

    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min -1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0)
        {
            if ((g_map.tile[y1][x2] != BLANK_TILE || g_map.tile[y2][x2] != BLANK_TILE))
            {
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            }
        }
        else if (x_val_ < 0)
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y2][x1] != BLANK_TILE))
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }

    }
}

void BossObject::CenterEntityOnMap(Map& g_map)
{
    g_map.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

    if (g_map.start_x_ < 0)
    {
        g_map.start_x_ = 0;
    }
    else if (g_map.start_x_ + SCREEN_WIDTH >= g_map.max_x_)
    {
        g_map.start_x_ = g_map.max_x_ - SCREEN_WIDTH;
    }

    g_map.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
    if (g_map.start_y_ < 0)
    {
        g_map.start_y_ = 0;
    }
    else if (g_map.start_y_ + SCREEN_HEIGHT >= g_map.max_y_)
    {
        g_map.start_y_ = g_map.max_y_ - SCREEN_HEIGHT;
    }
}


void BossObject::InitBullet(SDL_Renderer* screen)
{
    BulletObject* p_bullet = new BulletObject();
    bool ret = p_bullet -> LoadImg("img//boss_bullet.png", screen);
    if (ret == true)
    {
        p_bullet->set_bullet_direction(BulletObject::DIR_LEFT);
        p_bullet->set_is_move(true);
        p_bullet->SetRect(rect_.x -50, rect_.y + height_frame_ - 30);
        p_bullet->set_x_val(10);
        bullet_list_.push_back(p_bullet);
    }
}


void BossObject::MakeBullet(SDL_Renderer* des, const int& x_limit, const int& y_limit)
{
    if(frame_ == 18)
    {
        InitBullet(des);
    }
    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move())
            {
                p_bullet -> HandleMove(x_limit, y_limit);
                p_bullet -> Render(des);
            }
            else
            {
                p_bullet -> Free();
                bullet_list_.erase(bullet_list_.begin() + i);
            }
        }
    }
}

SDL_Rect BossObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = rect_.w / 32;
    rect.h = rect_.h / 1;

    return rect;
}

void BossObject::RemoveBullet()
{
    int size = bullet_list_.size();

    bullet_list_.clear();
        for (int i = 0; i <  size; i ++)
        {
BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
        }


}



















