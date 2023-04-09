
#ifndef BULLET_OBJECT_H
#define BULLET_OBJECT_H

#include "BaseObject.h"
#include "CommonFunction.h"

class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
        DIR_UP = 22,
        DIR_UP_LEFT = 23,
        DIR_UP_RIGHT = 24,
        DIR_DOWN_LEFT = 25,
        DIR_DOWN_RIGHT = 26,
    };


    void set_x_val(const int& xVal) {x_val_ = xVal;}
    void set_y_val(const int& yVal) {y_val_ = yVal;}

    int get_x_val() {return x_val_;}
    int get_y_val() {return y_val_;}

    void set_is_move(const bool& isMove) {is_move_ = isMove;}
    bool get_is_move(){return is_move_;}

    void set_bullet_direction (const unsigned int& bulletDir) {bullet_direction_ = bulletDir;}
    int get_bullet_direction() {return bullet_direction_;}

    void HandleMove(const int& x_border, const int& y_border); // hàm xử lí di chuyển của viên đạn

private:
    int x_val_;  //luong gia tri thay doi bao nhieu
    int y_val_;
    bool is_move_; // loi dung de xoa di
    unsigned int bullet_direction_;
};

#endif // BULLET_OBJECT_H
