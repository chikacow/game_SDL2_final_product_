
#include "MainObject.h"

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_NONE; // set trang thai ban dau cua nhan vat
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    on_ground_ = false;
    map_x_= 0;
    map_y_ = 0;
    come_back_time_ = 0;

}
MainObject::~MainObject()
{
    ;

}
bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen); // load path thành 1 biến texture

    if (ret == true);
    {
        width_frame_ = rect_.w/8;
        height_frame_ = rect_.h/1;
    }
    return ret;
}
void MainObject::set_clips() // tuc la ban dau load ảnh sẽ ở vị trí 0 0 nên ko cần truyển ảnh vào mà setclip tự cắt trên màn hình, do mới load chứ chưa render lên ta không nhìn thấ quá trình cắt
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
        frame_clip_[3].h = height_frame_;

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
void MainObject::Show(SDL_Renderer* des)
{
   /*
    if (status_ == WALK_LEFT)
    {
        LoadImg("img//player_left.png", des);

    }
    else
    {
        LoadImg("img//player_right.png", des);
    }
*/
    UpdateImagePlayer(des);
    if (input_type_.left_ == 1 || input_type_.right_ == 1)
    {
        frame_++;
    }
    else
    {
        frame_ = 0;
    }
    if (frame_ >= 8)
    {
        frame_ = 0;
    }
    /*
    rect_.x = x_pos_;
    rect_.y = y_pos_;
*/
if (come_back_time_ == 0)
{


    rect_.x = x_pos_ - map_x_; // chuyển pos về rect
    rect_.y = y_pos_ - map_y_;

    SDL_Rect* current_clip = &frame_clip_[frame_];

    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}
}
void MainObject:: HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                UpdateImagePlayer(screen);

            }
            break;
        case SDLK_LEFT:
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ =0;
                UpdateImagePlayer(screen);

            }
            break;
        default:
            break;

        }
    }
    else if (events.type == SDL_KEYUP) // để tránh việc nhân vật di chuyển liên tục như khủng long, khi nhấn thì mới di chuyển
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                input_type_.right_ = 0;

            }
            break;
        case SDLK_LEFT:
            {
                input_type_.left_ = 0;

            }
            break;

        }

    }

    if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_LEFT)
        {
            input_type_.jump_ = 1;
        }
        else if (events.button.button == SDL_BUTTON_RIGHT)  // nhay cao
        {
            BulletObject* p_bullet = new BulletObject(); // khởi tạo mảng vecto p_bullet
            p_bullet -> LoadImg("img//player_bullet.png", screen);

            if (status_ == WALK_LEFT)
            {
                p_bullet -> set_bullet_direction(BulletObject::DIR_LEFT);
                p_bullet -> SetRect(this -> rect_.x , rect_.y += height_frame_*0.25);
            }
            else
            {
                p_bullet -> set_bullet_direction(BulletObject::DIR_RIGHT);
                p_bullet -> SetRect(this -> rect_.x + width_frame_-20, rect_.y += height_frame_*0.25);
            }

            //p_bullet -> SetRect(this -> rect_.x + width_frame_-20, rect_.y += height_frame_*0.3); // chinh cho dan ban ra tu nong sung
            p_bullet -> set_x_val(20); // set tốc độ bay của viên đạn theo chiều ngang
            p_bullet -> set_y_val(20); // set tốc độ bay của viên đạn theo chiều dọc
            p_bullet -> set_is_move(true); // set trang thái đạn đang bay

            p_bullet_list_.push_back(p_bullet); // giống như nạp băng đạn

        }
    }
}

void MainObject::HandleBullet(SDL_Renderer* des)
{
    for (int i = 0; i< p_bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = p_bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet -> get_is_move() == true) // nếu như viên đạn di chuyển
            {
                p_bullet -> HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT); // hàm xử lí chuyển động của iên đạn
                p_bullet -> Render(des); // render hình ảnh viên đạn lên màn hình, khi render liên tục thì sẽ thấy viên đạn di chuyển liên tục
            }
        }
    }
}
void MainObject::DoPlayer(Map& map_data)
{
    if (come_back_time_ == 0)
    {

    x_val_ = 0;
    y_val_ += GRAVITY_SPEED;

    if (y_val_ >= MAX_FALL_SPEED)
    {
        y_val_ = MAX_FALL_SPEED;
    }
    if(input_type_.left_ == 1)
    {
        x_val_ -= PLAYER_SPEED;
    }
    if (input_type_.right_ == 1)
    {
        x_val_ += PLAYER_SPEED;
    }

    if (input_type_.jump_ == 1)
    {
        if (on_ground_ == true)  // nhan vat phai dung tren mat dat moi nhay dc, neu ko dung thi se giong flappy bird
        {
        y_val_ -= PLAYER_JUMP_VAL;
        }

        input_type_.jump_ = 0; // nếu ko có thì chỉ cần bấm nhảy lần đầu để set jump = 1 là nhân vật sẽ tự nhảy ko ngừng
        on_ground_ = false; // nếu ko có sẽ giống flappy bird
    }
  /*  if (input_type_.jump_ == 2) // code nhảy cao
    {
        if (on_ground_ == true)
        {
            y_val_ -= 2*PLAYER_JUMP_VAL;
            //on_ground_ = false;
        }
        input_type_.jump_ = 0;
        on_ground_ = false;
    } */



    CheckMap(map_data);
    CenterEntityOnMap(map_data); // nhan vat di chuyen keo theo ban do di chuyen

    }
    if (come_back_time_ > 0)
    {
        come_back_time_ --;
        if (come_back_time_ == 0)  // reset lai trang thai
        {
            on_ground_ = false;
            if( x_pos_ >= 256)
            {
            //map_x_ -= 256;
            x_pos_ -= 256;  // 4 tiles
            }
            else
            {
                x_pos_ = 0;
            }
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
        }
    }
}

void MainObject::CenterEntityOnMap(Map& map_data) // hàm chỉ làm nhiệm vụ xử lí dữ liệu, phần render và hình ảnh sẽ được thực hiện ở hàm drawmap
{
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH/2);
    if (map_data.start_x_ < 0)
    {
        map_data.start_x_ = 0;
    }
    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)// xét ở rìa cuối bản đồ
    {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH; // luc nay nvat di duoc den cuoi map, ban do ngung di chuyen theo nhan vat
    }
    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT/2); // nma nhan vat ko bay nen ko qtrong lawm
    if (map_data.start_y_ < 0 )
    {
        map_data.start_y_ = 0;
    }
    else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
    {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}

void MainObject::CheckMap(Map& map_data) // bao gom xu li di chuyen nhan vat, check va cham của nhân vật với bản đồ
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
            if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                x_pos_ = x2*TILE_SIZE;
                x_pos_ -= width_frame_ +1;
                x_val_ = 0;
            }
        }
        else if (x_val_ < 0)
        {
            if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
            {
                x_pos_ = (x1+1) * TILE_SIZE;
                x_val_ = 0;
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
            if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= height_frame_ +1;
                y_val_ = 0;
                on_ground_ = true;

                if (status_ == WALK_NONE)
                {
                    status_ = WALK_RIGHT;
                }
            }

        }
        else if (y_val_ < 0) // khi nhan vat jump
        {
            //on_ground_ = false;
            if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
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

    if (y_pos_ > map_data.max_y_)
    {
        come_back_time_ = 60;

    }

}
void MainObject::UpdateImagePlayer( SDL_Renderer* des)
{
    if ( on_ground_ == true)
    {
        if (status_ == WALK_LEFT)
        {
            LoadImg ("img//player_left.png", des);
        }
        else
        {
            LoadImg ("img//player_right.png", des);
        }
    }
    else
    {

        if (status_ == WALK_LEFT)
        {
            LoadImg("img//jum_left.png", des);
        }
        else
        {
            LoadImg("img//jum_right.png", des);
        }

    }

}




















