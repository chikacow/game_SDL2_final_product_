
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>

#include "CommonFunction.h"
#include "BaseObject.h"
#include "BulletObject.h""

#define GRAVITY_SPEED 1 // toc do roi
#define MAX_FALL_SPEED 100 // nhan vat roi co gia toc
#define PLAYER_SPEED 12 // toc do di chuyen //8

#define PLAYER_JUMP_VAL 19 // do nhay

#define MAX_BULLET 10 // dc nap toi da 5 vien dan

#define BULLET_PRICE 3
#define STATE_LIFE 8

#define STATE_WIN 7

class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();

    enum WalkType // định nghĩa các biến chuyển động
    {
        WALK_NONE = 0,
        WALK_RIGHT = 0,
        WALK_LEFT = 1,

    };

    bool LoadImg(std::string path, SDL_Renderer* screen); // Load anh từ đường dẫ
    void Show(SDL_Renderer* des); // show len man hinh, liên quan đến frame
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* player_sound[10]); // xu li di chyen
    void set_clips(); // xu li hieu ung animation
    void DoPlayer(Map& map_data, Mix_Chunk* player_sound[10]); // xử lí di chuyển nhân vật bao gồm checkmap và centerentityonmap
    void CheckMap(Map& map_data, Mix_Chunk* player_sound[10]); // check va chạm và thay đổi thông số tọa độ giúp nhân di chuyển
    void SetMapXY(const int map_x, const int map_y){map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap( Map& map_data); // di chuyển bản đồ theo nhân vật
    void UpdateImagePlayer( SDL_Renderer* des); // set ảnh theo đúng trạng thái của nhân vật

    void set_bullet_list( std::vector<BulletObject*> bullet_list); //
    std::vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}

    void HandleBullet(SDL_Renderer* des);

    void RemoveBullet (const int& idx);

    void increase_money();

    int get_frame_width() const {return width_frame_;}
    int get_frame_height() const {return height_frame_;}

    SDL_Rect GetRectFrame();

    void set_comeback_time(const int& cb_time) {come_back_time_ = cb_time;}
   // int get_numDie() {return numDie;}
    //void plus_numDie() {numDie++;}
    void set_ifFall(bool iF) {ifFall_ = iF;}
    bool get_ifFall() {return ifFall_;}

    int get_money_count() {return money_count_;}

    void set_check_hack(bool cH) {check_hack_ = cH;}
    bool get_check_hack() {return check_hack_;}

    void set_bullet_timer() {bullet_timer_ += 20;}
    int get_bullet_timer() {return bullet_timer_;}

    void set_time_hack(Uint32 tH) {time_hack_ = tH;}
    Uint32 get_time_hack() {return time_hack_;}
    void set_mute(bool Mute) {mute_ = Mute;}
    bool get_mute() {return mute_;}
    bool get_jump_on_trap() {return jump_on_trap_;}
    void set_jump_on_trap(bool jot) {jump_on_trap_ = jot;}
    int get_super_power_control() {return super_power_control_;}
    void set_super_power_control() {super_power_control_ ++;}
    bool get_life() {return life_;}
    void set_life(bool l) { life_ = l;}
    void set_on_ground(bool l) {on_ground_ = l;}
    bool get_check_win() {return check_win_;}
    void set_check_win(bool cw) {check_win_ = cw;}
    void set_open_tutorial(bool s) {open_tutorial_ = s;}
    bool get_open_tutorial() {return open_tutorial_;}
    bool get_open_pause() {return open_pause_;}
    void set_open_pause(bool p) {open_pause_ = p;}

private:

    std::vector<BulletObject*> p_bullet_list_; // coi như băng đạn
    float x_val_;
    float y_val_;   // ko can lam do nhan vat ko can bay

    float x_pos_;
    float y_pos_; // pos trong position , vi tri

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8]; // mảng dữ liệu lưu trữ các biến kiểu sdl_rect

    Input input_type_; // biến cấu trức lưu trữ thông tin khi tương tác bàn phím và chuột

    int frame_; // dang o frame nao

    int status_; // dang di chuyen sang phai hay trai

    bool on_ground_; // đang ở trên không trung hay mặt đất

    int map_x_;
    int map_y_;  // 2 bien luu vi tri ban do  do khi nhan vat di chuyen thi ban do se di chuyen theo

    int come_back_time_; // dùng khi xử lí rơi xuống vực thẳm

    int money_count_;

    bool ifFall_;
    //int numDie;

    int bullet_limit_;

    bool check_hack_;

    int real_bullet_limit_;

    int bullet_timer_;

    Uint32 time_hack_;
    bool double_jump_check_;
    bool mute_;

    bool on_ground_sound_;
    bool jump_on_trap_;
    int super_power_control_;

    bool life_;

    bool check_win_;
    int mute_count_;

    bool open_tutorial_;
    bool open_pause_;




};



#endif // MAIN_OBJECT_H

