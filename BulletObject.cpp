
#include "BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = SPHERE_BULLET;

}
BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet (SDL_Renderer* des)
{
    bool ret = false;
    if (bullet_type_ == SPHERE_BULLET)
    {
        ret = LoadImg("img//bullet_threat.png", des);
    }
     else if (bullet_type_ == LASER_BULLET)
    {
        ret = LoadImg("img//player_bullet1.png", des);
    }

    return ret;
}

void BulletObject::HandleMove(const int& x_border, const int& y_border) // truyền vào giới hạn phạm vi di chuyển của viên đạn
{
    if (bullet_direction_ == DIR_RIGHT) // nếu đạn bay sang phải
    {
    rect_.x += x_val_;  // rect_ lay tu baseobject theo tinh thua ke
    if (rect_.x > x_border)
    {
        is_move_ = false;
    }
    }

    else if (bullet_direction_ == DIR_LEFT)
    {
        rect_.x -= x_val_;  // rect_ lay tu baseobject theo tinh thua ke
    if (rect_.x < 0)
    //if (rect_.x += x_val_ > (x_border*2/3)-50)
    {
        is_move_ = false;
    }

    }

    else if (bullet_direction_ == DIR_UP)
    {
        rect_.y -= y_val_;
        if (rect_.y <0)
        {
            is_move_ = false;
        }
    }

    else if (bullet_direction_ == DIR_UP_LEFT)
    {
        rect_.x -= x_val_;
        if (rect_.x < 0)
        {
            is_move_ = false;
        }
        rect_.y -= y_val_;
        if (rect_.y < 0)
        {
            is_move_ = false;
        }
    }

    else if (bullet_direction_ == DIR_UP_RIGHT)
    {
        rect_.x += x_val_;
        if (rect_.x > x_border)
        {
            is_move_ = false;
        }
        rect_.y += y_val_;
        if (rect_.y < 0)
        {
            is_move_ = false;
        }
    }
}
