#include <SDL.h>

#include <SDL_image.h>
#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayersStuff.h"
#include "Geometric.h"
#include "BossObject.h"

using namespace SDLCommonFunction;
BaseObject g_background;
TTF_Font* font_time = NULL;
TTF_Font* font_menu = NULL;

bool InitData() // hàm khởi tạo
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0)
        return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Game cua ccK", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
            success = false;
        else{
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR,
                                    RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
        if (TTF_Init() == -1)
        {
            success = false;
        }
        font_time = TTF_OpenFont("font//dlxfont.ttf", 15);
        font_menu = TTF_OpenFont("font//dlxfont.ttf", 40);
        if (font_time == NULL || font_menu == NULL)
        {
            success = false;
        }

    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }
    else
    {
        // read file wav audio
        g_sound_player[8] = Mix_LoadWAV("music//snd_rifle_shot.wav");

        g_sound_player[0] = Mix_LoadWAV("music//player_bullet_load.wav");
        g_sound_player[2] = Mix_LoadWAV("music//player_coin_eating.wav");
        g_sound_player[1] = Mix_LoadWAV("music//player_double_jump.wav");
        g_sound_player[3] = Mix_LoadWAV("music//player_hurt.wav");
        g_sound_player[4] = Mix_LoadWAV("music//player_jump.wav");
        g_sound_player[5] = Mix_LoadWAV("music//player_landing.wav");
        g_sound_player[6] = Mix_LoadWAV("music//step1.wav");
        g_sound_player[7] = Mix_LoadWAV("music//step2.wav");
        //g_sound_player[8] = Mix_LoadWAV("music//player_fly.wav");
        g_sound_player[9] = Mix_LoadWAV("music//failed.wav");

        g_general_music[0] = Mix_LoadWAV("music//snd_danger.wav");
        g_general_music[1] = Mix_LoadWAV("music//click.wav");

        g_theme_music[0] = Mix_LoadWAV("music//main theme.wav");
        g_theme_music[1] = Mix_LoadWAV("music//win music.wav");
        g_theme_music[2] = Mix_LoadWAV("music//lose.wav");

        g_sound_exp[0] = Mix_LoadWAV("music//exp.wav");




        if (g_sound_player[8] == NULL || g_theme_music == NULL) // them that may cai ben tren
        {
            return false;
        }
    }
    return success;
}
bool LoadBackground() // hàm load ảnh background
{
    bool ret = g_background.LoadImg("img//background.png", g_screen); // lấy thông tin từ biến class g_background
    if (ret == false)
    {
        return false;
    }
    return true;
}

void close() // hàm đóng khi kết thúc chương trình
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

std::vector <ThreatObject*> MakeThreatList()
{
    std::vector<ThreatObject*> list_threats;

    ThreatObject* moving_threat = new ThreatObject[30];

    for(int i = 0; i < 30; i++)
    {

        ThreatObject* p_threat = (moving_threat + i);
        if (p_threat != NULL)
        {
            p_threat -> LoadImg("img//threat_left.png", g_screen);  //threatleft
            p_threat -> set_clips();
            p_threat -> set_type_move(ThreatObject:: MOVE_IN_SPACE_THREAT);

            p_threat -> set_x_pos (500 + 500*i);
            p_threat -> set_y_pos (200);

            int pos1 = p_threat -> get_x_pos() - 60;
            int pos2 = p_threat -> get_x_pos() + 60;

            p_threat ->  SetAnimationPos(pos1, pos2);
            p_threat -> set_input_left(1);

           // p_threat -> set_strength(1);

            BulletObject* p_bullet = new BulletObject();
            p_threat -> InitBullet(p_bullet, g_screen);





            list_threats.push_back(p_threat); // list nay cung kiem soat ca tinh va dong

        }

    }


    ThreatObject* threat_object = new ThreatObject[30]; // tao 20 doi tuong // updated 30

    for (int i = 0 ; i < 30; i++)
    {
        ThreatObject* p_threat = (threat_object + i);

        if (p_threat != NULL)
        {
            p_threat -> LoadImg("img//threat_level.png", g_screen);  //threatlevel
            p_threat -> set_clips();
            p_threat -> set_x_pos(700 + i*1200);
            p_threat -> set_y_pos(250);

            p_threat -> set_type_move(ThreatObject::STATIC_THREAT);

           // p_threat -> set_strength(2);

           if (i >= 15)
           {
            BulletObject* p_bullet = new BulletObject();
            p_threat -> InitBullet(p_bullet, g_screen);
           }





            list_threats.push_back(p_threat); // list nay cung kiem soat ca tinh va dong


           // list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

int main(int argc, char* argv[])
{
    //int bullet_control = 0;
    int theme_sound_control = 0;
    int sound_danger_control = 0;
    int num_die = 0; // so lan chet
    int boss_hp = BOSS_HP;  // mau cua boss

    ImpTimer fps_timer; // khai báo biến đếm thời gian

    if (InitData() == false) // khởi tạo của sổ màn hình
        {
            return -1;
        }
    if (LoadBackground() == false) // load ảnh background, nếu muốn load background nhiều layers thì gọi nhiều lần hàm này?
        {
            return -1;
        }
    GameMap game_map; // biến xử lí bản đồ
    game_map.LoadMap("map//map01.dat");  // đọc file text và lưu vào 1 mảng
    game_map.LoadTiles(g_screen); // load các loại tiles vào 1 mảng

    MainObject p_player; // biến xử lí nhân vật
    p_player.LoadImg("img//player_right.png", g_screen); // load ảnh nhân vật từ đường dẫn lên màn hình với chiều dài, chiều rộng và tọa độ 0,0, cung cấp thông tin về kích thước ảnh
    p_player.set_clips(); // cắt ảnh thành từng frame rồi lưu từng frame vào 1 mảng rect, cung cấp thông tin về vị trí, chiều dài chiều rộng của các frame để khi cắt sẽ biết vị trí cần cắt

    PlayersStuff player_power;
    player_power.Init(g_screen);

    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH*0.5 - 300, 8); // player_power ko can set vi tri anh vi no da set trong ham init r

    SuperPower super_power;
    super_power.Init(g_screen);
    super_power.SetPos(SCREEN_WIDTH*0.5 - 400 - 30, 4);

/*

    SoundStatus sound_status;
    if (p_player.get_mute() == false)
    {
    sound_status.InitOn(g_screen);
    }
    if (p_player.get_mute() == true)
    {
        sound_status.InitOff(g_screen);
    }
    sound_status.SetPos(SCREEN_WIDTH*0.5 + 210, 6);
*/


    std::vector <ThreatObject*> threat_list = MakeThreatList();


    //Boss
    BossObject boss_object;
    bool ret = boss_object.LoadImg("img//boss_object.png", g_screen);
    boss_object.set_clips();
    int xPosBoss = MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*0.5;
    boss_object.set_x_pos(xPosBoss+200);// set vi tri cho boss
    boss_object.set_y_pos(SCREEN_HEIGHT - 3*TILE_SIZE - boss_object.get_height_frame() - 10);


    ExplosionObject exp_boss;
    bool tRet2 = exp_boss.LoadImg("img//exp3.png", g_screen);
    if (!tRet2) return -1;
    exp_boss.set_clips();

    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
    if (!tRet) return -1;
    exp_threat.set_clips();

    ExplosionObject exp_main;
    bool tRet1 = exp_main.LoadImg("img//exp3.png", g_screen);
    if (!tRet1) return -1;
    exp_main.set_clips();


    //int num_die = 0; // so lan chet

    TextObject time_game; // text hien thi thoi gian
    time_game.SetColor(TextObject::WHITE_TEXT);

    TextObject mark_game;
    mark_game.SetColor(TextObject::RED_TEXT);
    UINT mark_value = 0; // ban quai zat

    TextObject money_game;
    money_game.SetColor(TextObject::BLACK_TEXT);

    TextObject game_sound;
    game_sound.SetColor(TextObject::WHITE_TEXT);

    int  t = 1;
    bool is_quit = false; // biến để kiểm tra chương trình có bị tắt ko

    //show menu
    int ret_menu = Menu::ShowMenu(g_screen, font_menu, g_general_music);
    if (ret_menu == 1)
    {
        is_quit = true;
    }
    else if (ret_menu == 2)
    {
        while(t == 1)
        {

                        int retT = Menu::ShowTutorial(g_screen, font_menu, g_general_music);
                        if (retT == 0)
                        {
                            int retMenu1 = Menu::ShowMenu(g_screen, font_menu, g_general_music);
                            if (retMenu1 == 1)
                            {
                                is_quit = true;
                                t = 2;
                            }
                            else if (retMenu1 == 2)
                            {
                                t = 1;
                            }
                            else
                            {
                                t = 2;
                            }
                        }




        }

    }





       // Mix_PlayChannel(-1,g_theme_music[0],1);

       // if (p_player.get_mute() == true)
        //{
          //  Mix_PauseMusic();
        //}

    while(!is_quit) // khi chưa tắt
    {
        //Mix_PlayChannel(-1,g_sound_player[8],100);
        fps_timer.start(); // bắt đầu đếm thời gian

        while(SDL_PollEvent(&g_event) != 0) // vòng lặp xử lí sự kiện như nhấn bàn phím, nhấn chuốt, nhấn nút quit, tức là khi có sự kiện xảy ra thì vòng while này sẽ chạy, còn ko thì tiếp tục chạ phần khác
        {
            if (g_event.type == SDL_QUIT) // khi nhấn vào nút x thì chương trình sẽ tắt, nếu ko có hàm này thì nút quit thành vô dụng
            {
                is_quit = true;
            }

            p_player.HandleInputAction(g_event, g_screen, g_sound_player);// khi bấm bàn phím hay chuột để nhân vật di chuyển thì hàm này sẽ được thực hiện

        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR); // Set the colour using for drawing operations
        SDL_RenderClear(g_screen);  // Xoa những gì vừa vẽ, tuy nhiên ta chưa vẽ gì nên 2 hàm này ko quá cần thiết, có thể bỏ đi

        g_background.Render(g_screen, NULL); // khi nãy vừa loadimg background, bây h ta render lên screen, để NULL cho đầy đủ tức là load full ảnh,( trong hướng dẫn ko bảo để NULL)
        //game_map.DrawMap(g_screen);

        Map map_data = game_map.getMap(); // map_data = game_map_

//sound
        sound_danger_control ++;
        if (sound_danger_control == 100)
        {
            if (p_player.get_mute() == false)
            {
            Mix_PlayChannel(-1,g_general_music[0] , 0);
            }
            sound_danger_control = 0;
        }


        p_player.HandleBullet(g_screen); //xử lí di chuyển và load ảnh đạn lên màn hình, dữ liệu được lấy từ handleinputaction trong vòng while events


        p_player.SetMapXY(map_data.start_x_, map_data.start_y_); // cái này chưa cần lắm
        p_player.DoPlayer(map_data, g_sound_player); // bao gồm cả check map và di chuyển map theo nhân vật, sau khi check di chuyển map thì dữ liệu sẽ đc thực thi ở lệnh drawmap bên dưới, dữ liệu về xpos và ypos sẽ được xử lí ở hàm show ngay dưới

        if (p_player.get_life() == true)
        {
            if (num_die > 0)
            {
            num_die --;
            player_power.InitCrease();
            p_player.set_life(false);
            }
        }
        // tu che//////////////////////////////////////////////////////////////////////////////
        if (p_player.get_ifFall() == true)
        {
            /*
            if (num_die <= NUM_LIFE )
                    {
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        //SDL_Delay(500);

                        player_power.Decrease();
                        player_power.Render(g_screen);

                        p_player.set_ifFall(false);
                        continue;
                    }
*/

/*
            if (MessageBox(NULL, "You felt!", "Info", MB_OK | MB_ICONSTOP) == IDOK) // chichachicha
            {
                is_quit = true;
                break;
            }
*/
            if (p_player.get_mute() == false)
            {
                Mix_PlayChannel(-1, g_theme_music[2], 0);
            }

            int retFall = Menu::ShowRestartFall(g_screen, font_menu, g_theme_music);
            if (retFall == 1)
            {
                is_quit = true;
            }
            else
            {
                // leen nhacj
            }
           // p_player.set_ifFall(false);
        }


SoundStatus sound_status;
    if (p_player.get_mute() == false)
    {
    sound_status.InitOn(g_screen);
    }
    if (p_player.get_mute() == true)
    {
        sound_status.InitOff(g_screen);
    }
    sound_status.SetPos(SCREEN_WIDTH*0.5 + 210, 6); // de nhan dc du lieu tu handle inputaction

        if (p_player.get_open_tutorial() == true)
        {
            int retTutorial = Menu::ShowTutorial(g_screen, font_menu, g_general_music);
            if (retTutorial == 1)
            {
                is_quit = true;
            }
            p_player.set_open_tutorial(false);
        }





        p_player.Show(g_screen); //xử lí xpos ypos, frameclip, ve cơ bản hàm này khá giống render trong baseobject

        game_map.SetMap(map_data); // chưa dùng tới
        game_map.DrawMap(g_screen); // hàm render map lên màn hình


        //Draw geometric
        GeometricFormat rectangel_size(0, 0, SCREEN_WIDTH, 40);
        ColorData color_data(188, 64, 64); // lay mau tuy y trong paint
        Geometric::RenderRectangle(rectangel_size, color_data, g_screen);

        GeometricFormat outLineSize(1,1, SCREEN_WIDTH - 1, 38);
        ColorData color_data2(255, 255, 255);

        Geometric::RenderOutLine(outLineSize, color_data2, g_screen);

        player_power.Show(g_screen);

        player_money.Show(g_screen); // khai baos, init o phia tren roi show o duoi nay la xong, chi kho o phan dinh nghia lop va ham

        sound_status.Show(g_screen);

        p_player.set_super_power_control();


        if (p_player.get_super_power_control() >= 200)
        {
            super_power.Show(g_screen);
        }

        for (int i = 0; i < threat_list.size(); i++)
        {
            ThreatObject* p_threat = threat_list.at(i);

            if (p_threat != NULL)
            {
                p_threat -> SetMapXY( map_data.start_x_, map_data.start_y_);
                p_threat -> ImpMoveType(g_screen);
                p_threat -> DoThreat(map_data);
                p_threat -> MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat -> Show(g_screen);



                SDL_Rect rect_player = p_player.GetRectFrame();

                bool bCol1 = false;

                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                for (int jj = 0; jj < tBullet_list.size(); ++jj)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);

                    if (pt_bullet)
                    {
                        bCol1 = SDLCommonFunction::CheckCollision(pt_bullet->GetRect(), rect_player);  // dan quai vs nguoi

                        if (bCol1 == true)
                        {
                           // p_threat -> RemoveBullet(jj); // xoa di de sau do quai van tiep tuc ban dan
                            break;
                        }
                    }
                }

                SDL_Rect rect_threat = p_threat->GetRectFrame();

                bool bCol2 = SDLCommonFunction::CheckCollision(rect_player, rect_threat);  // nguoi vs quai

                //if (bCol2 == true)
                //{
                 //   p_threat -> shor_1();
                //}
                if ((bCol1 || bCol2 ||p_player.get_jump_on_trap() == true)&& p_player.get_check_hack() == false)
                {
                    if (bCol1 || bCol2)
                    {


                    int width_exp_frame = exp_main.get_frame_width();
                    int height_exp_frame = exp_main.get_frame_height();
                    for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                    {
                        int x_pos = p_player.GetRect().x + p_player.get_frame_width()*0.5 - width_exp_frame*0.5; // vi tri dat tam anh vu no
                        int y_pos = p_player.GetRect().y + p_player.get_frame_height()*0.5 - height_exp_frame*0.5;

                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }

                    if (p_player.get_mute() == false)
                    {
                        Mix_PlayChannel(-1, g_sound_exp[0], 0);
                    }

                    }
                    if (p_player.get_jump_on_trap() == true)
                    {
                        if (p_player.get_mute() == false)
                        {
                        Mix_PlayChannel(-1, g_sound_player[3], 0);
                        }
                    }

                    num_die++;
                    if (num_die <= NUM_LIFE )
                    {
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(1000);
                        p_player.set_jump_on_trap(false);

                        player_power.Decrease();
                        player_power.Render(g_screen);

                        continue;
                    }
                    else
                    {
                        /*
                        if (MessageBox(NULL, "GAME OVER", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                        {
                            p_threat->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                        */
                        if (p_player.get_mute() == false)
            {
                Mix_PlayChannel(-1, g_theme_music[2], 0);
            }
                        int retResstart1 = Menu::ShowRestart(g_screen, font_menu, g_general_music);
                        p_player.set_check_win(false);
                        if (retResstart1 == 1)
                        {
                            is_quit = true;
                        }
                        else
                        {

                        }
                    }
                }

            }
        }

        // boss col
        bool bColBossBulletvsPlayer = false;


        SDL_Rect rect_player = p_player.GetRectFrame();
        std::vector<BulletObject*> boss_Bullet_list = boss_object.get_bullet_list();
        for (int jj = 0; jj < boss_Bullet_list.size(); ++jj)
            {
                BulletObject* boss_bullet = boss_Bullet_list.at(jj);

                if (boss_bullet)
                {
                    bColBossBulletvsPlayer = SDLCommonFunction::CheckCollision(boss_bullet->GetRect(), rect_player);  // dan quai vs nguoi

                    if (bColBossBulletvsPlayer == true)
                    {
                        // p_threat -> RemoveBullet(jj); // xoa di de sau do quai van tiep tuc ban dan
                        break;
                    }
                }
            }
        SDL_Rect rect_boss = boss_object.GetRectFrame();
        bool bColBoss = false; //= SDLCommonFunction::CheckCollision(rect_boss, rect_player);

        if ((bColBossBulletvsPlayer || bColBoss) && p_player.get_check_hack() == false)
                {
                    int width_exp_frame = exp_main.get_frame_width();
                    int height_exp_frame = exp_main.get_frame_height();
                    for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                    {
                        int x_pos = p_player.GetRect().x + p_player.get_frame_width()*0.5 - width_exp_frame*0.5; // vi tri dat tam anh vu no
                        int y_pos = p_player.GetRect().y + p_player.get_frame_height()*0.5 - height_exp_frame*0.5;

                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }


                    num_die++;
                    if (num_die <= NUM_LIFE )
                    {
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(1000);

                        player_power.Decrease();
                        player_power.Render(g_screen);

                        continue;
                    }
                    else
                    {
                        /*
                        if (MessageBox(NULL, "GAME OVER", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                        {
                            boss_object.Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                        */
                        if (p_player.get_mute() == false)
            {
                Mix_PlayChannel(-1, g_theme_music[2], 0);
            }
                        int retResstart2 = Menu::ShowRestart(g_screen, font_menu, g_general_music);
                        p_player.set_check_win(false);
                        if (retResstart2 == 1)
                        {
                            is_quit = true;
                        }
                        else
                        {

                        }
                    }
                }


        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();
        //bullet_control++;
        std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
        for (int r = 0; r < bullet_arr.size(); r++)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if (p_bullet != NULL)
            {
                for (int t = 0; t < threat_list.size(); t++)
                {
                    ThreatObject* obj_threat = threat_list.at(t);
                    if (obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();
                        tRect.h = obj_threat->get_height_frame();

                        SDL_Rect bRect = p_bullet->GetRect();

                        bool bCol = SDLCommonFunction::CheckCollision(bRect, tRect);

                        if (bCol == true)  // ban quai zat
                        {
                            mark_value++;
                            //p_threat -> shot_1();
                            for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                            {
                                int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5; // vi tri dat tam anh vu no
                                int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                            }

                            if (p_player.get_mute() == false)
                    {
                        Mix_PlayChannel(-1, g_sound_exp[0], 0);
                    }



                            p_player.RemoveBullet(r);
                            //if (p_threat -> get_strength() <= 0)
                            //{
                            obj_threat -> Free();
                            threat_list.erase(threat_list.begin() + t);
                            //}
                        }
                    }
                }
                SDL_Rect boss_rect = boss_object.GetRectFrame();
                SDL_Rect bRect = p_bullet->GetRect();

                bool bColPlayerBulletvsBoss = SDLCommonFunction::CheckCollision(boss_rect, bRect);
                if (bColPlayerBulletvsBoss == true)
                {
                    boss_hp --;

                    for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                        {
                            int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5; // vi tri dat tam anh vu no
                            int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;

                            exp_threat.set_frame(ex);
                            exp_threat.SetRect(x_pos, y_pos);
                            exp_threat.Show(g_screen);
                        }
                    p_player.RemoveBullet(r);
                    if (boss_hp == 0)
                    {
                    mark_value += 100;

                    boss_object.Free();
                    //boss_object.RemoveBullet();
/*
                    if (MessageBox(NULL, "You Wonnnn!", "Congratulations", MB_OK | MB_ICONSTOP) == IDOK)
                    {
                        is_quit = true;
                        break;
                    }
                    */
                    }



                }


            }
        }
       /* if (num_die > NUM_LIFE) // nhan vat roi xuong vuc la di luon
        {
            if (MessageBox(NULL, "GAME OVER", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                        {
                            //p_threat->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
        }*/


        if (p_player.get_open_pause() == true)
        {
            int retPause = Menu::ShowPause(g_screen, font_menu, g_general_music);
            p_player.set_open_pause(false);
            if (retPause == 1)
            {
                is_quit = true;
            }
        }
/*

        if (p_player.get_check_win() == true)
        {
            // (p_player.get_mute() == false)
            //{


                int retWin = Menu::ShowMenuWin(g_screen, font_menu, g_general_music, win_data);
                p_player.set_check_win(false);
                if (retWin == 1)
                {
                    is_quit = true;
                }
                else
                {

                    if (p_player.get_mute() == false)
                    {
                        Mix_PlayChannel(-1, g_theme_music[1],0);
                    }

                }
            //}
        }
*/
        // Show game time
        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        //Uint32 val_time = 300 - time_val;
        Uint32 val_time = 600;
        val_time -= time_val;


        if (val_time <= 0 && p_player.get_check_hack() == false)
        {
            /*
            if (MessageBox(NULL, "Too Slowwww", "Info", MB_OK | MB_ICONSTOP) == IDOK)
            {
                is_quit = true;
                break;

            }
            */
            if (p_player.get_mute() == false)
            {
                Mix_PlayChannel(-1, g_theme_music[2], 0);
            }
            int retResstartTimeOver = Menu::ShowRestartTimeOver(g_screen, font_menu, g_general_music);
            p_player.set_check_win(false);
            if (retResstartTimeOver == 1)
            {
                is_quit = true;
            }
            else
            {
                //sound
            }

        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15); // vi tri cua text tren screen
        }
        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("Mark: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 50, 15);

        int money_count = p_player.get_money_count();
        std::string money_str = std::to_string(money_count);

        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 250, 15);

        WinData win_data;
        win_data.mark_win_ = strMark;
        win_data.money_win_ = money_str;
        win_data.time_win_ = std::to_string(600 - val_time);

        std::string sound_str("Sound: ");
        game_sound.SetText(sound_str);
        game_sound.LoadFromRenderText(font_time, g_screen);
        game_sound.RenderText(g_screen, SCREEN_WIDTH*0.5 + 120, 15);

        if (p_player.get_check_win() == true)
        {
            // (p_player.get_mute() == false)
            //{

            if (p_player.get_mute() == false)
                    {
                        Mix_PlayChannel(-1, g_theme_music[1],0);
                    }
                int retWin = Menu::ShowMenuWin(g_screen, font_menu, g_general_music, win_data);
                p_player.set_check_win(false);
                if (retWin == 1)
                {
                    is_quit = true;
                }


        }
            //}


       // show boss
        int val = MAX_MAP_X*TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x);// de khi den cuoi map boss ms xuat hien // vi tri ban do dang duoc cuon den
        if (val <= SCREEN_WIDTH)
        {


        boss_object.SetMapXY(map_data.start_x_, map_data.start_y_);
        boss_object.DoBoss(map_data);
        //boss_object.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
        boss_object.Show(g_screen);
        }

        SDL_RenderPresent(g_screen); // cập nhật và vẽ lên g_screen, khi này thay vì màn hình đen thì ta sẽ thấy hình ảnh đầy đủ

        int real_imp_time = fps_timer.get_ticks(); // trả về thời gian chạy 1 frame kể từ lúc bắt đầu vòng while
        int time_one_frame = 1000/FRAME_PER_SECOND; //ms, 1s = 1000 ms, ý là thời gian load 1 frame

        p_player.set_bullet_timer(); // kiem soat toc do ban

        if (real_imp_time <= time_one_frame)// nếu như chương trình chạy nhanh quá mức, vượt quá thời gian quy định là time_one_frame
        {
            int delay_time = time_one_frame- real_imp_time; // để cân bằng
            if (delay_time >= 0)
                SDL_Delay(delay_time);// dừng 1 khoảng thời gian rồi mới chạy tiếp, giúp chương trình ko bị chạy quá nhanh

        }

    }

    for (int i = 0; i < threat_list.size(); i++)
    {
        ThreatObject* p_threat = threat_list. at(i);
        if (p_threat)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threat_list.clear();

    close(); // khi kết thúc vòng while đồng nghĩa với việc chương trình kết thúc, hàm close giải phóng bộ nhớ

    return 0;
}

