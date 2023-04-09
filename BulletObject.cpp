
#include "BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;

}
BulletObject::~BulletObject()
{

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
