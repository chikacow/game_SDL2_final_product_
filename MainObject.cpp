
#include "MainObject.h"

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 300; // de ne vien dan khi khoi tao
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
    money_count_ = 15;
    //numDie = 0;
    ifFall_ = false;
    bullet_limit_ = 10;
    check_hack_= false;
    real_bullet_limit_ = 0;
    bullet_timer_ = 0;
    time_hack_ = 0;
    double_jump_check_ = false;
    mute_ = false;
    on_ground_sound_ = true;
    jump_on_trap_ = false;
    super_power_control_ = 300;
    life_ = false;
    check_win_ = false;
    mute_count_ = 0;
    open_tutorial_ = false;
    open_pause_ = false;

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
        width_frame_ = rect_.w/8; // chiều rộng bằng 1/8 chiều rộng của ảnh load vào do ảnh load vào gồm nhiều frame
        height_frame_ = rect_.h/1; // chiều cao giữ nguyên
    }
    return ret;
}
void MainObject::set_clips() // cho biết nơi các frame được đặt trong ảnh
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
void MainObject::Show(SDL_Renderer* des) // load các frame liên tục, tạo hiệu ứng animation cho nhân vật
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
    UpdateImagePlayer(des); // lấy dữ liệu từ handleinputaction
    if (input_type_.left_ == 1 || input_type_.right_ == 1)
    {
        frame_++; // nếu nhân vật di chuyển thì cộng thêm 1 frame
    }
    else
    {
        frame_ = 0; // nếu ko di chuyển thì chuyển về frame 0, tức là ảnh nhân vật đứng im
    }
    if (frame_ >= 8) // nếu chạy hết 8 frame thì chạy lại từ đầu
    {
        frame_ = 0;
    }
    /*
    rect_.x = x_pos_;
    rect_.y = y_pos_;
*/
if (come_back_time_ == 0) // nhân vật rơi xuống vực xong rồi quay lại
{


    rect_.x = x_pos_ - map_x_; // chuyển pos về rect
    rect_.y = y_pos_ - map_y_;

    SDL_Rect* current_clip = &frame_clip_[frame_]; // frame hiện tại là frame nào

    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_}; // biến lưu thông số của frame

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad); // render phần ảnh ở vị trí frame đó trong ảnh lên màn hình
}
}
void MainObject:: HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* player_sound[10])
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym) // liên quan đến bàn phím
        {
        case SDLK_RIGHT: // nhấn phím phải
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                UpdateImagePlayer(screen);
                if (on_ground_ == true && mute_ == false)
                {
                Mix_PlayChannel(-1, player_sound[6],0);
                Mix_PlayChannel(-1,player_sound[7], 0);
                }




            }
            break;
        case SDLK_LEFT:  // nhấn phím trái
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ =0;
                UpdateImagePlayer(screen);
                if (on_ground_ == true && mute_ == false)
                {
                Mix_PlayChannel(-1, player_sound[6],0);
                Mix_PlayChannel(-1,player_sound[7], 0);
                }


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

    if (events.type == SDL_KEYDOWN) // khi nhay ko can keyup vi ben duoi da set khi nhay da set inputtypejump = 0 r
    {
        if (events.key.keysym.sym == SDLK_UP) // nếu click chuột trái thì nhảy
        {
            input_type_.jump_ = 1;

        }
        else if (events.key.keysym.sym == SDLK_j)
        {
            input_type_.jump_ = 2;
        }
        else if (events.key.keysym.sym == SDLK_m)
        {
            mute_ = true;
        }
        else if (events.key.keysym.sym == SDLK_n)
        {
            mute_ = false;
        }
        else if (events.key.keysym.sym == SDLK_SPACE)  // nhay cao , nếu click chuột phải // tai sao ko can keyup?
        {
            if (bullet_timer_ >= 80)  // kiem soat toc do ban dan_________________________________________
            {

            if (bullet_limit_ > 0)
            {


            BulletObject* p_bullet = new BulletObject(); // khởi tạo biến p_bullet, cấp phát mảng động
            p_bullet -> set_bullet_type (BulletObject::LASER_BULLET); // lam them an phim nao thi doi loai dan
            p_bullet -> LoadImgBullet(screen);
           // p_bullet -> LoadImg("img//player_bullet.png", screen); // load ảnh viên đạn


            if (status_ == WALK_LEFT) // nếu đang đi sang trái
            {
                p_bullet -> set_bullet_direction(BulletObject::DIR_LEFT); // chỉnh hướng bắn từ phải qua trái
                p_bullet -> SetRect(this -> rect_.x , rect_.y += height_frame_*0.25); // vị trí xuất phát là vị trí của nhân vật và thay đổi ở recty để bắn ra từ nóng súng
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

            if (mute_ == false)
            {
            Mix_PlayChannel(-1, player_sound[8], 0); // loop bat dau tu 0
            }
            bullet_limit_ --;
            }

            bullet_timer_ = 0; // kiem soat toc do ban dan______________________________________________

            }

        }
        else if (events.key.keysym.sym == SDLK_r)
        {
            //bullet_limit_ = 0;
            //bullet_limit_ += 10;
            if (money_count_ >= BULLET_PRICE)
            {
                money_count_ -= BULLET_PRICE;
                bullet_limit_ = 0;
                bullet_limit_ += 10;
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[0],0);
                }
            }
            else if (money_count_ < BULLET_PRICE)
            {
                Mix_PlayChannel(-1, player_sound[9], 0);
            }

            if (bullet_limit_ > MAX_BULLET)
            {
                bullet_limit_ = MAX_BULLET;
            }
        }
        else if (events.key.keysym.sym == SDLK_p)
        {
            open_pause_ = true;

        }
        else if (events.key.keysym.sym == SDLK_t)
        {
            open_tutorial_ = true;
        }
        else if (events.key.keysym.sym == SDLK_q)         // hack part_____________________________________________________
        {
            if (check_hack_ == false)
            {
                set_check_hack(true);
                real_bullet_limit_ = bullet_limit_;
                bullet_limit_ = 100;
            }
            else if (check_hack_ == true)
            {
                set_check_hack(false);
                bullet_limit_ = real_bullet_limit_;
            }
        }                                                    //____________________________________________________
    }
}

void MainObject::HandleBullet(SDL_Renderer* des)
{
    for (int i = 0; i< p_bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = p_bullet_list_.at(i); // xét viên đạn tại vị trí i
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
void MainObject::DoPlayer(Map& map_data, Mix_Chunk* player_sound[10])
{
    if (come_back_time_ == 0)
    {
    set_ifFall(false);

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

        if (mute_ == false)
        {
        Mix_PlayChannel(-1, player_sound[4], 0);
        }
        input_type_.jump_ = 0; // nếu ko có thì chỉ cần bấm nhảy lần đầu để set jump = 1 là nhân vật sẽ tự nhảy ko ngừng
        on_ground_ = false; // nếu ko có sẽ giống flappy bird
        on_ground_sound_ = true;
    }
    if (input_type_.jump_ == 2) // code nhảy cao
    {
        if (on_ground_ == true && super_power_control_ >= 300)
        {
            y_val_ -= 2*PLAYER_JUMP_VAL;
            //on_ground_ = false;
        }
        input_type_.jump_ = 0;
        on_ground_ = false;
        if (mute_ == false)
        {
        Mix_PlayChannel(-1, player_sound[1],0);
        }
        on_ground_sound_ = true;

        super_power_control_ = 0;

    }



    CheckMap(map_data, player_sound);
    CenterEntityOnMap(map_data); // nhan vat di chuyen keo theo ban do di chuyen

    }
    if (come_back_time_ > 0)
    {
        come_back_time_ --;
        if (come_back_time_ == 0)  // reset lai trang thai
        {
            on_ground_ = false;
            if( x_pos_ >= 256) // để tránh lỗi nếu nhân vật ở đầu map
            {
            //map_x_ -= 256;
            x_pos_ -= 256;  // lùi lại 4 tiles
            }
            else
            {
                x_pos_ = 0;
            }
            y_pos_ = 0; // rơi từ đâu xuống
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

void MainObject::CheckMap(Map& map_data, Mix_Chunk* player_sound[10]) // bao gom xu li di chuyen nhan vat, check va cham của nhân vật với bản đồ
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

            if (val1 == STATE_LIFE || val2 == STATE_LIFE)
            {
                map_data.tile[y1][x2] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x2] = 0;
                life_ = true;
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
            }
            if (val1 == STATE_WIN || val2 == STATE_WIN)
            {
                map_data.tile[y1][x2] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x2] = 0;
                check_win_ = true;

            }

            if (val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y1][x2] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x2] = 0;
                increase_money();
                {
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
                }
            }
            if (val1 == STATE_TRAP || val2 == STATE_TRAP)
            {

                //mix
                jump_on_trap_ = true;
            }
            else
            {

                if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
                    {
                        x_pos_ = x2*TILE_SIZE;
                        x_pos_ -= width_frame_ +1;
                        x_val_ = 0;

                        if (val1 == STATE_TRAP || val2 == STATE_TRAP)
            {

                //mix
                jump_on_trap_ = true;
            }
            if (val1 == 6 || val2 == 6)
            {

                //mix
                jump_on_trap_ = true;
            }
                    }

            }

        }
        else if (x_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if (val1 == STATE_LIFE || val2 == STATE_LIFE)
            {
                map_data.tile[y1][x1] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x1] = 0;
                life_ = true;
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
            }
            if (val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y1][x1] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x1] = 0;
                increase_money();
                {
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
                }

            }
            if (val1 == STATE_WIN || val2 == STATE_WIN)
            {
                map_data.tile[y1][x1] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x1] = 0;
                check_win_ = true;

            }
            else
            {
                if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
                {
                    x_pos_ = (x1+1) * TILE_SIZE;
                    x_val_ = 0;

                    if (val1 == STATE_TRAP || val2 == STATE_TRAP)
            {

                //mix
                jump_on_trap_ = true;
            }
            if (val1 == 6 || val2 == 6)
            {

                //mix
                jump_on_trap_ = true;
            }
                }
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
//de nhan vat co chan khi di tu thung xuong
            if (y_val_ > 2*GRAVITY_SPEED)
            {
                on_ground_ = false;
                on_ground_sound_ = true;
            }

            if (val1 == STATE_LIFE || val2 == STATE_LIFE)
            {
                map_data.tile[y2][x1] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x2] = 0;
                life_ = true;
                {
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
                }
            }
            if (val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                increase_money();
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
            }
            if (val1 == STATE_WIN || val2 == STATE_WIN)
            {
                map_data.tile[y2][x1] = 0; // thanhf oo trong, bien mat
                map_data.tile[y2][x2] = 0;
                check_win_ = true;

            }
            else
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
                    if (on_ground_sound_ == true)
                    {

                    if (mute_ == false)
                    {
                    Mix_PlayChannel(-1, player_sound[5], 0);
                    }
                    on_ground_sound_ = false;
                    }
                    if (val1 == 6 || val2 == 6)
            {

                //mix
                jump_on_trap_ = true;
            }
                }
                if (val1 == STATE_TRAP || val2 == STATE_TRAP)
            {

                //mix
                jump_on_trap_ = true;
            }


            }
        }
        else if (y_val_ < 0) // khi nhan vat jump
        {
            //on_ground_ = false;

            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];

            if (val1 == STATE_LIFE || val2 == STATE_LIFE)
            {
                map_data.tile[y1][x1] = 0; // thanhf oo trong, bien mat
                map_data.tile[y1][x2] = 0;
                life_ = true;
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
            }
            if (val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                increase_money();
                if (mute_ == false)
                {
                Mix_PlayChannel(-1,player_sound[2],0);
                }
            }
            if (val1 == STATE_WIN || val2 == STATE_WIN)
            {
                map_data.tile[y1][x1] = 0; // thanhf oo trong, bien mat
                map_data.tile[y1][x2] = 0;
                check_win_ = true;

            }
            else
            {
                if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
                {
                    y_pos_ = (y1+1) * TILE_SIZE;
                    y_val_ = 0;

                    if (val1 == STATE_TRAP || val2 == STATE_TRAP)
            {

                //mix
                jump_on_trap_ = true;
            }
            if (val1 == 6 || val2 == 6)
            {

                //mix
                jump_on_trap_ = true;
            }
                }
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

    if (y_pos_ > SCREEN_HEIGHT + 120)//map_data.max_y_) // nếu rơi xuống vực thẳm // trong TH nay screenheight va mapdatamaxy la = nhau
    {
        //plus_numDie();
        set_ifFall(true);
        come_back_time_ = 60;

    }



}
void MainObject::UpdateImagePlayer(SDL_Renderer* des)
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

void MainObject::increase_money()
{
    money_count_++;
}

void MainObject::RemoveBullet(const int& idx)
{
    int size = p_bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject* p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}


SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;

}



















