#include <SDL.h>

#include <SDL_image.h>
#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"

BaseObject g_background;

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

int main(int argc, char* argv[])
{
    ImpTimer fps_timer;

    if (InitData() == false) // khởi tạo của sổ màn hình
        {
            return -1;
        }
    if (LoadBackground() == false) // load ảnh background
        {
            return -1;
        }
    GameMap game_map; // biến xử lí bản đồ
    game_map.LoadMap("map//map01.dat");  // đọc file text và lưu vào 1 mảng
    game_map.LoadTiles(g_screen); // load các loại tiles vào 1 mảng

    MainObject p_player; // biến xử lí nhân vật
    p_player.LoadImg("img//player_right.png", g_screen); // load ảnh nhân vật lên màn hình với chiều dài, chiều rộng và tọa độ 0,0, cung cấp thông tin về kích thước ảnh
    p_player.set_clips(); // cắt ảnh thành từng frame rồi lưu từng frame vào 1 mảng rect

    bool is_quit = false; // biến để kiểm tra chương trình có bị tắt ko
    while(!is_quit) // khi chưa tắt
    {
        fps_timer.start();

        while(SDL_PollEvent(&g_event) != 0) // vòng lặp xử lí sự kiện như nhấn bàn phím, nhấn chuốt, nhấn nút quit, tức là khi có sự kiện xảy ra thì vòng while này sẽ chạy, còn ko thì tiếp tục chạ phần khác
        {
            if (g_event.type == SDL_QUIT) // khi nhấn vào nút x thì chương trình sẽ tắt, nếu ko có hàm này thì nút quit thành vô dụng
            {
                is_quit = true;
            }
            p_player.HandleInputAction(g_event, g_screen); // khi bấm bàn phím để nhân vật di chuyển thì hàm này sẽ được thực hiện
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR); // Set the colour using for drawing operations
        SDL_RenderClear(g_screen);  // Xoa những gì vừa vẽ, tuy nhiên ta chưa vẽ gì nên 2 hàm này ko quá cần thiết, có thể bỏ đi

        g_background.Render(g_screen, NULL); // khi nãy vừa loadimg background, bây h ta render lên screen, để NULL cho đầy đủ, trong hướng dẫn ko bảo để NULL
        //game_map.DrawMap(g_screen);

        Map map_data = game_map.getMap(); // map_data = game_map_



        p_player.HandleBullet(g_screen); //xử lí di chuyển và load ảnh đạn lên màn hình, dữ liệu được lấy từ handleinputaction trong vòng while events


        p_player.SetMapXY(map_data.start_x_, map_data.start_y_); // cái này chưa cần lắm
        p_player.DoPlayer(map_data); // bao gồm cả check map và di chuyển map theo nhân vật, sau khi check di chuyển map thì dữ liệu sẽ đc thực thi ở lệnh drawmap bên dưới, dữ liệu về xpos và ypos sẽ được xử lí ở hàm show ngay dưới
        p_player.Show(g_screen); //xử lí xpos ypos, frameclip, ve cơ bản hàm này khá giống render trong baseobject

        game_map.SetMap(map_data); // chưa dùng tới
        game_map.DrawMap(g_screen); // hàm render map lên màn hình

        SDL_RenderPresent(g_screen); // cập nhật và vẽ lên g_screen, khi nà thay vì màn hình đen thì ta sẽ thấy hình ảnh đầy đủ

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND; //ms

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
                SDL_Delay(delay_time);

        }

    }
    close(); // khi kết thúc vòng while đồng nghĩa với việc chương trình kết thúc, hàm close giải phóng bộ nhớ

    return 0;
}

