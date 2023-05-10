
#include "ThreatObject.h"
#include "BaseObject.h"
#include "CommonFunction.h"

ThreatObject::ThreatObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = false;
    come_back_time_ = 0;
    frame_ = 0;

    animation_a_ = 0;
    animation_b_ = 0;
    input_type_.left_ = 0; // mac dinh la tinh
    type_move_ = STATIC_THREAT;
    colRight = false;
    colLeft = false;
   // strength_ = 0
}
ThreatObject::~ThreatObject()
{

}

bool ThreatObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret)
    {
        width_frame_ = rect_.w/THREAT_FRAME_NUM;
        height_frame_ = rect_.h/1;
    }
    return ret;
}
void ThreatObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0) //nếu loadimg thành công
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = width_frame_*2;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = width_frame_*3;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;  // vd: frame 4 sẽ ở vị trí trong ảnh sao cho nó cắt đc trọn vẹn frame thứ 4 trong ảnh

        frame_clip_[4].x = width_frame_*4;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = width_frame_;
        frame_clip_[4].h = height_frame_;

        frame_clip_[5].x = width_frame_*5;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = width_frame_;
        frame_clip_[5].h = height_frame_;

        frame_clip_[6].x = width_frame_*6;
        frame_clip_[6].y = 0;
        frame_clip_[6].w = width_frame_;
        frame_clip_[6].h = height_frame_;

        frame_clip_[7].x = width_frame_*7;
        frame_clip_[7].y = 0;
        frame_clip_[7].w = width_frame_;
        frame_clip_[7].h = height_frame_;
    }
}
void ThreatObject::Show(SDL_Renderer* des)
{
    if (come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_; // tinh vi tri thuc su
        rect_.y = y_pos_ - map_y_;

        frame_++;

        if (frame_ > 8)
        {
            frame_ = 0;
        }

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
    }

}

void ThreatObject::DoThreat(Map& gMap)
{
    if (come_back_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += THREAT_GRAVITY_SPEED;

        if (y_val_ >= THREAT_MAX_FALL_SPEED)
        {
            y_val_ = THREAT_MAX_FALL_SPEED;
        }

        if (input_type_.left_ == 1)
        {
            x_val_ -= THREAT_SPEED;
        }
        else if (input_type_.right_ ==1)
        {
            x_val_ += THREAT_SPEED;
        }

        CheckToMap(gMap);
    }
    else if (come_back_time_ > 0)
    {
        come_back_time_--;
        if (come_back_time_ == 0)
        {
            /*
            x_val_ = 0;
            y_val_ = 0;

            if (x_pos_ > 256)
            {
                x_pos_ -= 256;
                animation_a_ -= 256;
                animation_b_ -= 256;
            }
            else
            {
                x_pos_ = 0;
            }
            y_pos_ = 0;
            come_back_time_ = 0;
            input_type_.left_ = 1;
            */
            InitThreat();

        }
    }
}

void ThreatObject::InitThreat()
{
     x_val_ = 0;
     y_val_ = 0;

     if (x_pos_ > 256)
        {
            x_pos_ -= 256;
            animation_a_ -= 256;
            animation_b_ -= 256;
        }
     else
        {
            x_pos_ = 0;
        }
        y_pos_ = 0;
        come_back_time_ = 0;
        input_type_.left_ = 1;
}

void ThreatObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //Check horizontal
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + x_val_)/TILE_SIZE; // vi tri hien tai cua nhan vat o o thu bao nhieu
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE; // -1 de check va cham

    y1 = (y_pos_)/TILE_SIZE;
    y2 = (y_pos_  + height_min - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // dang di chuyen sang phai
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];



            if ((val1 != BLANK_TILE && val1 != STATE_MONEY && val1 != STATE_TRAP) || (val2 != BLANK_TILE && val2 != STATE_MONEY && val2 != STATE_TRAP))
            {
                x_pos_ = x2*TILE_SIZE;
                x_pos_ -= width_frame_ +1;
                x_val_ = 0;
                input_type_.right_ = 0;
                input_type_.left_ = 1;
                colRight = true;
            }


        }
        else if (x_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];


            if ((val1 != BLANK_TILE && val1 != STATE_MONEY && val1 != STATE_TRAP) ||(val2 != BLANK_TILE && val2 != STATE_MONEY && val2 != STATE_TRAP))
            {
                x_pos_ = (x1+1) * TILE_SIZE;
                x_val_ = 0;
                input_type_.left_ = 0;
                input_type_.right_ = 1;
                colLeft = true;
            }

        }
    }
    // Check vertical

    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min)/TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1 )/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0) // khi nhân vật rơi từ trên xuống
        {

            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];


            if ((val1 != BLANK_TILE && val1 != STATE_MONEY && val1 != STATE_TRAP) || (val2 != BLANK_TILE && val2 != STATE_MONEY && val2 != STATE_TRAP))
            {
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= height_frame_ +1;
                y_val_ = 0;
                on_ground_ = true;
            }


        }
        else if (y_val_ < 0) // khi nhan vat jump
        {
            //on_ground_ = false;

            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];


            if ((val1 != BLANK_TILE && val1 != STATE_MONEY && val1 != STATE_TRAP)|| (val2 != BLANK_TILE && val2 != STATE_MONEY && val2 != STATE_TRAP))
            {
                y_pos_ = (y1+1) * TILE_SIZE;
                y_val_ = 0;
            }


        }
    }
    x_pos_ += x_val_; // suy cho cùng thì xác định tọa độ của ảnh trên màn hình vẫn phải dùng biến rect_ kiểu SDL rect, dù xpos và ypos cũng có s nghĩa xác định tọa độ ảnh. Cuối cùng pos sẽ được xử lí để chuyển thành rect trong hàm show, khi đó mới truyền đc dữ liệu phù hợp và hàm render lên màn hình được
    y_pos_ += y_val_;


    if (x_pos_ < 0)
    {
        x_pos_ = 0;  //ko cho ra ngoai map
    }
    if (x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ ;
    }

    if (y_pos_ > map_data.max_y_) // nếu rơi xuống vực thẳm
    {
        come_back_time_ = 60;

    }


}

void ThreatObject::ImpMoveType(SDL_Renderer* screen)
{
    if (type_move_ == STATIC_THREAT)
    {
        ;// ko lam gi ca
    }
    else
    {
        if (on_ground_ == true) // chi khi nao dung tren mat dat thi moi di chuyen
        {
            if (x_pos_ > animation_b_ || colRight == true)
            {


                input_type_.left_ = 1;
                input_type_.right_ = 0;
               // input_type_.jump_ =1;
                LoadImg("img//threat_left.png", screen);  // threatleft
                colRight = false;

            }
            else if (x_pos_ < animation_a_ || colLeft == true )  // a__________b
            {

                input_type_.left_ = 0;
                input_type_.right_ = 1;
               // input_type_.jump_ =1;
                LoadImg("img//threat_left.png", screen);  //threatright
                colLeft = false;
            }
            //CheckToMap(gMap);
        }


        else // on_ground_ = false;
        {
            if (input_type_.left_ == 1)
            {
                LoadImg("img//threat_left.png", screen); // threatleft
            }

        }
    }
}

void ThreatObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
    if (p_bullet != NULL)
    {
        //bool ret =
        p_bullet -> set_bullet_type(BulletObject::SPHERE_BULLET);
        bool ret = p_bullet -> LoadImgBullet(screen);

        if (ret)
        {
        p_bullet -> set_is_move(true);


        p_bullet -> set_bullet_direction(BulletObject::DIR_LEFT);
        p_bullet -> SetRect(rect_.x + 5, rect_.y + 10);
        p_bullet -> set_x_val(18);
        bullet_list_.push_back(p_bullet);
        }
    }
}

void ThreatObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
    for (int i = 0; i< bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet -> get_is_move())
            {
                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
                if (bullet_distance < 300 && bullet_distance > 0 )
                {
                p_bullet -> HandleMove(x_limit, y_limit);
                p_bullet -> Render(screen);
                }
                else
                {
                    p_bullet->set_is_move(false);
                }
            }
            else
            {
                p_bullet -> set_is_move(true);
                p_bullet -> SetRect(rect_.x + 5, rect_.y + 10);

            }
        }
    }
}

void ThreatObject::RemoveBullet (const int& idx)
{
    int size = bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject* p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

SDL_Rect ThreatObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}



















