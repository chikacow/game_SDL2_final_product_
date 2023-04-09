
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>

#include "CommonFunction.h"
#include "BaseObject.h"
#include "BulletObject.h""

#define GRAVITY_SPEED 1 // toc do roi
#define MAX_FALL_SPEED 100 // nhan vat roi co gia toc
#define PLAYER_SPEED 8 // toc do di chuyen

#define PLAYER_JUMP_VAL 20 // do nhay

class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();

    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 0,
        WALK_LEFT = 1,

    };

    bool LoadImg(std::string path, SDL_Renderer* screen); // Load anh
    void Show(SDL_Renderer* des); // show len man hinh
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen); // xu li di chyen
    void set_clips(); // xu li hieu ung animation
    void DoPlayer(Map& map_data);//cho
    void CheckMap(Map& map_data);
    void SetMapXY(const int map_x, const int map_y){map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap( Map& map_data);
    void UpdateImagePlayer( SDL_Renderer* des);

    void set_bullet_list( std::vector<BulletObject*> bullet_list);
    std::vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}

    void HandleBullet(SDL_Renderer* des);

private:

    std::vector<BulletObject*> p_bullet_list_;
    float x_val_;
    float y_val_;   // ko can lam do nhan vat ko can bay

    float x_pos_;
    float y_pos_; // pos trong position , vi tri

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8];

    Input input_type_;

    int frame_; // dang o frame nao

    int status_; // dang di chuyen sang phai hay trai

    bool on_ground_;

    int map_x_;
    int map_y_;  // 2 bien luu vi tri ban do  do khi nhan vat di chuyen thi ban do se di chuyen theo

    int come_back_time_;


};



#endif // MAIN_OBJECT_H

