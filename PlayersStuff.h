
#ifndef PLAYERS_STUFF_H_
#define PLAYERS_STUFF_H_

#include "CommonFunction.h"
#include "BaseObject.h"

class PlayersStuff : public BaseObject
{
public:
    PlayersStuff();
    ~PlayersStuff();

    void SetNum(const int& num) {number_ = num;}
    void AddPos(const int& xPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);

    void InitCrease();
    void Decrease();

private:
    int number_;
    std::vector<int> pos_list_;
};


class PlayerMoney : public BaseObject
{
public:
    PlayerMoney();
    ~PlayerMoney();

    void Init(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void SetPos(const int& x, const int& y) { x_pos_ = x; y_pos_ = y;}

private:
    int x_pos_;
    int y_pos_;
};


class SoundStatus : public BaseObject
{
public:
    SoundStatus();
    ~SoundStatus();

    void InitOn(SDL_Renderer* screen);
    void InitOff(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void SetPos(const int& x, const int& y) { x_pos_ = x; y_pos_ = y;}

private:
    int x_pos_;
    int y_pos_;
};

class SuperPower : public BaseObject
{
public:
    SuperPower();
    ~SuperPower();

    void Init(SDL_Renderer* screen);
    void Show (SDL_Renderer* screen);
    void SetPos(const int& x, const int& y) {x_pos_ = x; y_pos_ = y;}

private:
    int x_pos_;
    int y_pos_;
};
#endif // PLAYERS_STUFF_H
