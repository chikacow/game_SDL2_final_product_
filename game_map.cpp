
#include "game_map.h"


#include <stdio.h>
#include <iostream>

void GameMap::LoadMap(char* name)
{
    FILE *fp = NULL;
    fopen_s(&fp, name, "rb"); //doc file kieu rb sau khi truyền con trỏ FILE và tên đường dẫn file
    if (fp == NULL)
    {
        return;
    }

    game_map_.max_x_ = 0; // gan gia tri o dau tien cua map la 0 0
    game_map_.max_y_ = 0;

    for (int i = 0; i<MAX_MAP_Y; i++)  // đọc theo chiều dọc
    {
        for (int j=0; j<MAX_MAP_X; j++) // đọc theo chiều ngang
        {
            fscanf(fp, "%d", &game_map_.tile[i][j]); // doc thong tin tu file text va luu no vao mang 2 chieu
            int val = game_map_.tile[i][j];  // val = số trong file text
            if (val > 0) // nếu ô vừa xét khác ô trống BlankTile
            {
                if(j> game_map_.max_x_)
                {
                    game_map_.max_x_ = j;  // lấy thông số về chiều ngang
                }

                if (i > game_map_.max_y_ )
                {
                    game_map_.max_y_ = i;  //lay thong so về chiều dọc, để đỡ phải tốn công load mấy ô trống
                }

                /*   0 0 0 0 0 0 0
                     0 0 0 0 1 1 0 .
                     0 2 3 0 4 0 0 .
                     1 1 1 1 1 1 0 .
                     1 1 1 1 1 1 0 .
                     0 0 0 0 0 0 0
                     . . . . . .
                     i và j là số dấu chấm
                     */
            }
        }
     }

    game_map_.max_x_ = (game_map_.max_x_ + 1)*TILE_SIZE; // +1 do lúc chạy vòng for thì i và j bắt đầu từ 0, nếu thiếu +1 sẽ bị thiết mất 1 ô
    game_map_.max_y_ = (game_map_.max_y_ + 1)*TILE_SIZE; // sau đây thì max_x_ và max_y_ sẽ là chiều dài và rộng của mảng theo giá trị

    game_map_.start_x_ = 0;
    game_map_.start_y_ = 0;

    game_map_.file_name_ = name; // gán tên file vào biến lưu trữ tên file trong game_map_
    fclose(fp); // đóng file

}

void GameMap::LoadTiles(SDL_Renderer* screen)
{
    char file_img[30]; // de luu cac ki tu co trong ten file anh
    FILE* fp = NULL;

    for (int i = 0; i < MAX_TILES; i++)
    {
        sprintf_s(file_img, "map/%d.png", i);
        fopen_s(&fp, file_img, "rb");
        if (fp == NULL)
        {
            continue;
        }
        fclose(fp);

        tile_mat[i].LoadImg(file_img, screen); //luu hinh anh vao mang tile mat, mới chỉ load thôi chứ chưa render lên màn hình
    }
}
/*
void GameMap :: DrawMap (SDL_Renderer* screen) //print image vao cac tiles
{
    int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;

    int map_x = 0;
    int map_y = 0;

    map_x = game_map_.start_x_ / TILE_SIZE;
    x1= (game_map_.start_x_ % TILE_SIZE) * (-1); //print lui lai 1 tile
    x2=x1 + SCREEN_WIDTH + (x1==0 ? 0: TILE_SIZE); //neu x1=0 thi khong + con x1 khac 0 thi cong them 1 tile

    map_y = game_map_.start_y_ / TILE_SIZE;
    y1= (game_map_.start_y_ % TILE_SIZE) * (-1);
    y2=y1 + SCREEN_HEIGHT + (y1==0 ? 0: TILE_SIZE);

    for (int i = y1 ; i < y2 ; i+=TILE_SIZE)
    {
        map_x = game_map_.start_x_ / TILE_SIZE;
        for (int j = x1 ; j < x2 ; j+=TILE_SIZE)
        {
            int val = game_map_.tile [map_y] [map_x];
            if (val > 0 )
            {
                tile_mat[val].SetRect( j , i);
                tile_mat[val].Render (screen);
            }
            map_x++;
        }
        map_y++;
    }
}


*/
void GameMap::DrawMap(SDL_Renderer* screen)  // ham render map len man hinh
{
    int map_x = 0;
    int x1 = 0;
    int x2 = 0;


    int map_y = 0;
    int y1 = 0;
    int y2 = 0;


    map_x= game_map_.start_x_/TILE_SIZE;
    map_y = game_map_.start_y_/TILE_SIZE;
    x1 = (game_map_.start_x_%TILE_SIZE)*(-1); // thêm -1 để xét lùi lại 1 ô, thà thừa còn hơn thiếu, nếu ko thì khi nhân vật di chuyển sang phải map sẽ bị thiếu
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    y1 = (game_map_.start_y_%TILE_SIZE)*(-1);
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    for (int i = y1; i < y2; i+= TILE_SIZE) // cộng nguyên cả lượng tilesize = 64 nên khi xét ta xét số đẹp 1 2 3, trùng vs kiểu tọa độ trong file text
    {
        map_x = game_map_.start_x_/TILE_SIZE;
        for (int j = x1; j< x2; j += TILE_SIZE)
        {
            int val = game_map_.tile[map_y][map_x];
            if (val > 0)  // nếu ô không phải ô trống
            {
                tile_mat[val].SetRect(j, i); // set vị trí của ô
                tile_mat[val].Render(screen); // render ô tile lên màn hình
            }
            map_x++;
        }
        map_y++;
    } // chạy xong vòng for tức là đã vẽ xong taats cả các ô được lưu trong mảng tile
    // biến i, j có ý nghĩa xác định tọa độ của ô để render lên màn hình, còn mapx mapy là tọa độ của ô trong mảng

}

