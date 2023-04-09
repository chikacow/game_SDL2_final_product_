#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunction.h"
#include "BaseObject.h"

#define MAX_TILES 20


class TileMat : public BaseObject // la doi tuong tinh, ko co tinh nang rieng
{
public:

    TileMat() {;}
    ~TileMat() {;}
};

class GameMap
{
public:
    GameMap(){;}
    ~GameMap(){;}

    void LoadMap(char* name);   //HAm doc file text
    void LoadTiles(SDL_Renderer* screen);            // chon loai anh de load
    void DrawMap (SDL_Renderer* screen);
    Map getMap() const {return game_map_;}
    void SetMap(Map& map_data) {game_map_ = map_data;}

private:
    Map game_map_;
    TileMat tile_mat[MAX_TILES];  //so loai hinh anh
};



#endif // GAME_MAP_H
