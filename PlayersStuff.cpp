
#include "PlayersStuff.h"
#include "CommonFunction.h"
#include "MainObject.h"
PlayersStuff::PlayersStuff()
{
    number_ = 0;
}
PlayersStuff::~PlayersStuff()
{

}
void PlayersStuff::AddPos(const int& xp)
{
    pos_list_.push_back(xp);
}

void PlayersStuff::Init(SDL_Renderer* screen)
{

    bool ret = LoadImg("img//player_life.png", screen);
    if (ret == true)
    {
        number_ = 3;
        if (pos_list_.size() > 0)
        {
            pos_list_.clear();
        }

        AddPos(20);
        AddPos(60);
        AddPos(100);
    }
}

void PlayersStuff::Show(SDL_Renderer* screen)
{
    for ( int i = 0; i < pos_list_.size(); i++)
    {
        rect_.x = pos_list_.at(i);
        rect_.y = 0;
        Render(screen);
    }
}

void PlayersStuff::Decrease()
{
    number_ --;
    pos_list_.pop_back();
}

void PlayersStuff::InitCrease()
{
    number_ ++;
    if (number_ > 5)
    {
        number_ = 5;
    }
    int last_pos = pos_list_.back();
    last_pos += 40;
    pos_list_.push_back(last_pos);
}

PlayerMoney::PlayerMoney()
{
    x_pos_ = 0;
    y_pos_ = 0;
}
PlayerMoney::~PlayerMoney()
{

}
void PlayerMoney::Init(SDL_Renderer* screen)
{
    bool ret = LoadImg("img//money.png", screen);
}
void PlayerMoney::Show(SDL_Renderer* screen)
{
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    Render(screen);
}

SoundStatus::SoundStatus()
{
    x_pos_ = 0;
    y_pos_ = 0;
}
SoundStatus::~SoundStatus()
{

}

void SoundStatus::InitOn(SDL_Renderer* screen)
{
    bool ret = LoadImg("img//onsound.png", screen);
}

void SoundStatus::InitOff(SDL_Renderer* screen)
{
    bool ret = LoadImg("img//offsound.png", screen);
}
void SoundStatus::Show(SDL_Renderer* screen)
{
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    Render(screen);
}

SuperPower::SuperPower()
{
    x_pos_ = 0;
    y_pos_ = 0;
}
SuperPower::~SuperPower()
{

}

void SuperPower::Init(SDL_Renderer* screen)
{
    bool ret = LoadImg("img//super_power.png", screen);
}

void SuperPower::Show(SDL_Renderer* screen)
{
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    Render(screen);
}

