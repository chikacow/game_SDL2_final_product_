
#include "TextObject.h"
#include "CommonFunction.h"
TextObject::TextObject()
{
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
    texture_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}
TextObject::~TextObject()
{

}
bool TextObject::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{

    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if (text_surface)
    {
        texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
        width_ = text_surface->w;
        height_ = text_surface->h;


        SDL_FreeSurface(text_surface);
    }
    return texture_ != NULL;
}

bool TextObject::LoadFromRenderTextMenu(TTF_Font* font, SDL_Renderer* screen)
{
    if (texture_ != NULL)
    {
        Free();
    }
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if (text_surface)
    {
        texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
       /* width_ = text_surface->w;
        height_ = text_surface->h;*/
        rect_.w = text_surface->w;
        rect_.h = text_surface->h;

        SDL_FreeSurface(text_surface);
    }
    return texture_ != NULL;
}

void TextObject::Free()
{
    if (texture_ != NULL)
    {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
}
void TextObject::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}
void TextObject::SetColor(int type)
{
    if (type == RED_TEXT)
    {
        SDL_Color color = {255, 0, 0};
        text_color_ = color;

    }
    else if (type == WHITE_TEXT)
    {
        SDL_Color color = {255, 255, 255};
        text_color_ = color;

    }
    else if (type == BLACK_TEXT)
    {
        SDL_Color color = {0, 0, 0};
        text_color_ = color;

    }
}
void TextObject::RenderText(SDL_Renderer* screen,int xp,
                            int yp,
                            SDL_Rect* clip /*= NULL*/,
                            double angle /*= 0.0*/,
                            SDL_Point* center /*= NULL*/,
                            SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
    SDL_Rect renderQuad =  {xp, yp, width_, height_};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle,center, flip);
}








void TextObject::RenderTextMenu(SDL_Renderer* screen,
                            SDL_Rect* clip /*= NULL*/,
                            double angle /*= 0.0*/,
                            SDL_Point* center /*= NULL*/,
                            SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)


{
    SDL_Rect renderQuad =  {rect_.x, rect_.y, rect_.w, rect_.h};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle,center, flip);
}


BaseObject gMenu;


int Menu::ShowMenu(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* theme_sound[5])
{
    if (gMenu.LoadImg("img//background.png", des) == 0)
    {
        return 1;
    }

    const int MenuItemNum = 3;

    SDL_Rect posArr[MenuItemNum];

    posArr[0].x = SCREEN_WIDTH*0.5 - 170;
    posArr[0].y = 380;

    posArr[1].x = SCREEN_WIDTH*0.5 - 70;
    posArr[1].y = 450;

    posArr[2].x = SCREEN_WIDTH*0.5 - 210;
    posArr[2].y = 520;

    TextObject textMenu[MenuItemNum];

    textMenu[0].SetText("Play Game");
    textMenu[0].SetColor(TextObject::WHITE_TEXT);
    textMenu[0].SetRect(posArr[0].x, posArr[0].y);

    textMenu[1].SetText("Exit");
    textMenu[1].SetColor(TextObject::WHITE_TEXT);
    textMenu[1].SetRect(posArr[1].x, posArr[1].y);

    textMenu[2].SetText("How To Play");
    textMenu[2].SetColor(TextObject::WHITE_TEXT);
    textMenu[2].SetRect(posArr[2].x, posArr[2].y);

    TextObject explain;
    explain.SetText("BRAVE SOLDIER");
    explain.SetColor(TextObject::WHITE_TEXT);
    explain.SetRect(SCREEN_WIDTH*0.5 - 250, 150);
/*
    TextObject credit;
    credit.SetText("cck_22028065");
    credit.SetColor(TextObject::WHITE_TEXT);
    credit.SetRect(10, 600);
*/
    bool selected[MenuItemNum] = {0, 0};

    int xm = 0;
    int ym = 0;

    SDL_Event mEvent;
    while (true)
    {
        gMenu.Render(des);

        for (int i = 0; i < MenuItemNum; i++)
        {
            textMenu[i].LoadFromRenderTextMenu(font, des);
            textMenu[i].RenderTextMenu(des);
            // textMenu[i].CreateGameText(font, des);
        }
        explain.LoadFromRenderTextMenu(font, des);
        explain.RenderTextMenu(des);
/*
        credit.LoadFromRenderTextMenu(font, des);
        credit.RenderTextMenu(des);
*/
    while (SDL_PollEvent(&mEvent))
    {
        switch (mEvent.type)
    {
    case SDL_QUIT:
    return 1;
    case SDL_MOUSEMOTION:
    xm = mEvent.motion.x;
    ym = mEvent.motion.y;


    for (int i = 0; i < MenuItemNum; i++) {
    if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1)
    {
        if (selected[i] == false)
        {
            selected[i] = true;
            textMenu[i].SetColor(TextObject::RED_TEXT);
        }
    }
    else
    {
        if (selected[i] == true)
        {
            selected[i] = false;
            textMenu[i].SetColor(TextObject::WHITE_TEXT);
        }
    }
 }

    break;

    case SDL_MOUSEBUTTONDOWN:
    xm = mEvent.button.x;
    ym = mEvent.button.y;

    for (int i = 0; i < MenuItemNum; i++) {
    if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
        Mix_PlayChannel(-1, theme_sound[1], 0);
    return i;
    }
    }



    case SDL_KEYDOWN:
    if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
    return 1;
    }

    default:
    break;
    }

    }
    SDL_RenderPresent(des);
    }

    return 1;
    }

    BaseObject gRestart;
    int Menu::ShowRestart(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]) {
    if (gRestart.LoadImg("img//background.png", des) == 0) {
    return 1;
    }

    const int RestartItemNum = 2;
    SDL_Rect posArr[RestartItemNum];
    posArr[0].x = SCREEN_WIDTH*0.5 - 180;
    posArr[0].y = 200;

    posArr[1].x = SCREEN_WIDTH*0.5 - 70;
    posArr[1].y = 260;

    TextObject textMenu[RestartItemNum];
    textMenu[0].SetText("Play Again");//playagain
    textMenu[0].SetColor(TextObject::WHITE_TEXT);
    textMenu[0].SetRect(posArr[0].x, posArr[0].y);

    textMenu[1].SetText("EXIT");
    textMenu[1].SetColor(TextObject::WHITE_TEXT);
    textMenu[1].SetRect(posArr[1].x, posArr[1].y);

    TextObject reason;

    reason.SetText("NO LIVES LEFT");
    reason.SetColor(TextObject::WHITE_TEXT);
    reason.SetRect(SCREEN_WIDTH*0.5 - 150-125+40, 360);

    bool selected[RestartItemNum] = {0, 0};
    int xm = 0;
    int ym = 0;
    SDL_Event mEvent;
    while (true) {
    gRestart.Render(des);
    for (int i = 0; i < RestartItemNum; i++) {
    textMenu[i].LoadFromRenderTextMenu(font, des);
    textMenu[i].RenderTextMenu(des);

    reason.LoadFromRenderTextMenu(font, des);
    reason.RenderTextMenu(des);
 // textMenu[i].CreateGameText(font, des);
    }

    while (SDL_PollEvent(&mEvent)) {
    switch (mEvent.type)
    {
    case SDL_QUIT:
    return 1;
    case SDL_MOUSEMOTION:
    xm = mEvent.motion.x;
    ym = mEvent.motion.y;

    for (int i = 0; i < RestartItemNum; i++) {
    if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
    if (selected[i] == false) {
    selected[i] = true;
    textMenu[i].SetColor(TextObject::RED_TEXT);
    }
    }
    else {
    if (selected[i] == true) {
    selected[i] = false;
    textMenu[i].SetColor(TextObject::WHITE_TEXT);
    }
    }
    }

    break;

    case SDL_MOUSEBUTTONDOWN:
    xm = mEvent.button.x;
    ym = mEvent.button.y;
    for (int i = 0; i < RestartItemNum; i++) {
    if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
        Mix_PlayChannel(-1,general_sound[1], 0);
    return i;

    }
    }
    case SDL_KEYDOWN:
    if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
    return 1;
    }

    default:
    break;
    }

    }
    SDL_RenderPresent(des);
    }

    return 1;
    }

BaseObject gRestartFall;

int Menu::ShowRestartFall(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]) {
 if (gRestartFall.LoadImg("img//background.png", des) == 0) {
 return 1;
 }

 const int RestartItemNum = 2;
 SDL_Rect posArr[RestartItemNum];
 posArr[0].x = SCREEN_WIDTH*0.5 - 180;
 posArr[0].y = 200;

 posArr[1].x = SCREEN_WIDTH*0.5 - 70;
 posArr[1].y = 260;

 TextObject textMenu[RestartItemNum];
 textMenu[0].SetText(" "); //Play again
 textMenu[0].SetColor(TextObject::WHITE_TEXT);
 textMenu[0].SetRect(posArr[0].x, posArr[0].y);

 textMenu[1].SetText("EXIT");
 textMenu[1].SetColor(TextObject::WHITE_TEXT);
 textMenu[1].SetRect(posArr[1].x, posArr[1].y);

 TextObject reason;

 reason.SetText("YOU FELT!");
 reason.SetColor(TextObject::WHITE_TEXT);
 reason.SetRect(SCREEN_WIDTH*0.5 - 200+40, 360);

 bool selected[RestartItemNum] = {0, 0};
 int xm = 0;
 int ym = 0;
 SDL_Event mEvent;
 while (true) {
 gRestart.Render(des);
 for (int i = 0; i < RestartItemNum; i++) {
 textMenu[i].LoadFromRenderTextMenu(font, des);
 textMenu[i].RenderTextMenu(des);

 reason.LoadFromRenderTextMenu(font, des);
 reason.RenderTextMenu(des);
 // textMenu[i].CreateGameText(font, des);
 }

 while (SDL_PollEvent(&mEvent)) {
 switch (mEvent.type)
 {
 case SDL_QUIT:
 return 1;
 case SDL_MOUSEMOTION:
 xm = mEvent.motion.x;
 ym = mEvent.motion.y;

 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
 if (selected[i] == false) {
 selected[i] = true;
 textMenu[i].SetColor(TextObject::RED_TEXT);
 }
 }
 else {
 if (selected[i] == true) {
 selected[i] = false;
 textMenu[i].SetColor(TextObject::WHITE_TEXT);
 }
 }
 }

 break;

 case SDL_MOUSEBUTTONDOWN:
 xm = mEvent.button.x;
 ym = mEvent.button.y;
 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
        Mix_PlayChannel(-1,general_sound[1], 0);
 return i;

 }
 }
 case SDL_KEYDOWN:
 if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
 return 1;
 }

 default:
 break;
 }

 }
 SDL_RenderPresent(des);
 }

 return 1;
}

BaseObject gRestartTimeOver;

int Menu::ShowRestartTimeOver(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]) {
 if (gRestartTimeOver.LoadImg("img//background.png", des) == 0) {
 return 1;
 }

 const int RestartItemNum = 2;
 SDL_Rect posArr[RestartItemNum];
 posArr[0].x = SCREEN_WIDTH*0.5 - 180;
 posArr[0].y = 200;

 posArr[1].x = SCREEN_WIDTH*0.5 - 70;
 posArr[1].y = 260;

 TextObject textMenu[RestartItemNum];
 textMenu[0].SetText("Play Again");
 textMenu[0].SetColor(TextObject::WHITE_TEXT);
 textMenu[0].SetRect(posArr[0].x, posArr[0].y);

 textMenu[1].SetText("EXIT");
 textMenu[1].SetColor(TextObject::WHITE_TEXT);
 textMenu[1].SetRect(posArr[1].x, posArr[1].y);

 TextObject reason;

 reason.SetText("SO SLOWWWW!");
 reason.SetColor(TextObject::WHITE_TEXT);
 reason.SetRect(SCREEN_WIDTH*0.5 - 200, 360);

 bool selected[RestartItemNum] = {0, 0};
 int xm = 0;
 int ym = 0;
 SDL_Event mEvent;
 while (true) {
 gRestartTimeOver.Render(des);
 for (int i = 0; i < RestartItemNum; i++) {
 textMenu[i].LoadFromRenderTextMenu(font, des);
 textMenu[i].RenderTextMenu(des);

 reason.LoadFromRenderTextMenu(font, des);
 reason.RenderTextMenu(des);
 // textMenu[i].CreateGameText(font, des);
 }

 while (SDL_PollEvent(&mEvent)) {
 switch (mEvent.type)
 {
 case SDL_QUIT:
 return 1;
 case SDL_MOUSEMOTION:
 xm = mEvent.motion.x;
 ym = mEvent.motion.y;

 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
 if (selected[i] == false) {
 selected[i] = true;
 textMenu[i].SetColor(TextObject::RED_TEXT);
 }
 }
 else {
 if (selected[i] == true) {
 selected[i] = false;
 textMenu[i].SetColor(TextObject::WHITE_TEXT);
 }
 }
 }

 break;

 case SDL_MOUSEBUTTONDOWN:
 xm = mEvent.button.x;
 ym = mEvent.button.y;
 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
        Mix_PlayChannel(-1,general_sound[1], 0);
 return i;

 }
 }
 case SDL_KEYDOWN:
 if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
 return 1;
 }

 default:
 break;
 }

 }
 SDL_RenderPresent(des);
 }

 return 1;
}

BaseObject gMenuWin;

int Menu::ShowMenuWin(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5], WinData data_win) {
 if (gMenuWin.LoadImg("img//background.png", des) == 0) {
 return 1;
 }

 const int RestartItemNum = 2;
 SDL_Rect posArr[RestartItemNum];
 posArr[0].x = SCREEN_WIDTH*0.5 - 180;
 posArr[0].y = 200-120;

 posArr[1].x = SCREEN_WIDTH*0.5 - 70;
 posArr[1].y = 260-120;

 TextObject textMenu[RestartItemNum];
 textMenu[0].SetText("PLAY AGAIN");
 textMenu[0].SetColor(TextObject::WHITE_TEXT);
 textMenu[0].SetRect(posArr[0].x, posArr[0].y);

 textMenu[1].SetText("EXIT");
 textMenu[1].SetColor(TextObject::WHITE_TEXT);
 textMenu[1].SetRect(posArr[1].x, posArr[1].y);

 TextObject reason;

 reason.SetText("CONGRATULATIONS, YOU WON");
 reason.SetColor(TextObject::WHITE_TEXT);
 reason.SetRect(SCREEN_WIDTH*0.5 - 450, 360-120);

 TextObject winData[3];
 winData[0].SetText(data_win.money_win_);
 winData[0].SetColor(TextObject::WHITE_TEXT);
 winData[0].SetRect(SCREEN_WIDTH*0.5 - 300 + 80, 400);

 winData[1].SetText(data_win.mark_win_);
 winData[1].SetColor(TextObject::WHITE_TEXT);
 winData[1].SetRect(SCREEN_WIDTH*0.5 - 450, 460);

 winData[2].SetText(data_win.time_win_);
 winData[2].SetColor(TextObject::WHITE_TEXT);
 winData[2].SetRect(SCREEN_WIDTH*0.5 - 300 + 80, 520);

 TextObject winInfo[3];

 winInfo[0].SetText("Coin: ");
 winInfo[0].SetColor(TextObject::WHITE_TEXT);
 winInfo[0].SetRect(SCREEN_WIDTH*0.5 - 450, 400);

 winInfo[2].SetText("Time: ");
 winInfo[2].SetColor(TextObject::WHITE_TEXT);
 winInfo[2].SetRect(SCREEN_WIDTH*0.5 - 450, 520);

 winInfo[1].SetText("*Scores: ");
 winInfo[1].SetColor(TextObject::WHITE_TEXT);
 winInfo[1].SetRect(SCREEN_WIDTH*0.5 - 550, 325);



 bool selected[RestartItemNum] = {0, 0};
 int xm = 0;
 int ym = 0;
 SDL_Event mEvent;
 while (true) {
 gMenuWin.Render(des);
 for (int i = 0; i < RestartItemNum; i++) {
 textMenu[i].LoadFromRenderTextMenu(font, des);
 textMenu[i].RenderTextMenu(des);

 reason.LoadFromRenderTextMenu(font, des);
 reason.RenderTextMenu(des);

 winData[0].LoadFromRenderTextMenu(font, des);
 winData[0].RenderTextMenu(des);

 winData[1].LoadFromRenderTextMenu(font, des);
 winData[1].RenderTextMenu(des);

 winData[2].LoadFromRenderTextMenu(font, des);
 winData[2].RenderTextMenu(des);

 winInfo[0].LoadFromRenderTextMenu(font, des);
 winInfo[0].RenderTextMenu(des);

 winInfo[2].LoadFromRenderTextMenu(font, des);
 winInfo[2].RenderTextMenu(des);

 winInfo[1].LoadFromRenderTextMenu(font, des);
 winInfo[1].RenderTextMenu(des);



 // textMenu[i].CreateGameText(font, des);
 }

 while (SDL_PollEvent(&mEvent)) {
 switch (mEvent.type)
 {
 case SDL_QUIT:
 return 1;
 case SDL_MOUSEMOTION:
 xm = mEvent.motion.x;
 ym = mEvent.motion.y;

 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
 if (selected[i] == false) {
 selected[i] = true;
 textMenu[i].SetColor(TextObject::RED_TEXT);
 }
 }
 else {
 if (selected[i] == true) {
 selected[i] = false;
 textMenu[i].SetColor(TextObject::WHITE_TEXT);
 }
 }
 }

 break;

 case SDL_MOUSEBUTTONDOWN:
 xm = mEvent.button.x;
 ym = mEvent.button.y;
 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
       // Mix_PlayChannel(-1,general_sound[1], 0);
 return i;

 }
 }
 case SDL_KEYDOWN:
 if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
 return 1;
 }

 default:
 break;
 }

 }
 SDL_RenderPresent(des);
 }

 return 1;
}

BaseObject gTutorial;

int Menu::ShowTutorial(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]) {
 if (gTutorial.LoadImg("img//tutorial.png", des) == 0) {
 return 1;
 }

 const int RestartItemNum = 2;
 SDL_Rect posArr[RestartItemNum];
 posArr[0].x = SCREEN_WIDTH*0.5 + 250 + 30;
 posArr[0].y = 10;

 posArr[1].x = SCREEN_WIDTH*0.5 - 70;
 posArr[1].y = 260;

 TextObject textMenu[RestartItemNum];
 textMenu[0].SetText("CONTINUE");
 textMenu[0].SetColor(TextObject::WHITE_TEXT);
 textMenu[0].SetRect(posArr[0].x, posArr[0].y);


 TextObject reason;

 reason.SetText("TUTORIAL");
 reason.SetColor(TextObject::WHITE_TEXT);
 reason.SetRect(SCREEN_WIDTH*0.5 - 600, 10);

 bool selected[RestartItemNum] = {0, 0};
 int xm = 0;
 int ym = 0;
 SDL_Event mEvent;
 while (true) {
 gTutorial.Render(des);
 for (int i = 0; i < RestartItemNum; i++) {
 textMenu[i].LoadFromRenderTextMenu(font, des);
 textMenu[i].RenderTextMenu(des);

 reason.LoadFromRenderTextMenu(font, des);
 reason.RenderTextMenu(des);
 // textMenu[i].CreateGameText(font, des);
 }

 while (SDL_PollEvent(&mEvent)) {
 switch (mEvent.type)
 {
 case SDL_QUIT:
 return 1;
 case SDL_MOUSEMOTION:
 xm = mEvent.motion.x;
 ym = mEvent.motion.y;

 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
 if (selected[i] == false) {
 selected[i] = true;
 textMenu[i].SetColor(TextObject::RED_TEXT);
 }
 }
 else {
 if (selected[i] == true) {
 selected[i] = false;
 textMenu[i].SetColor(TextObject::WHITE_TEXT);
 }
 }
 }

 break;

 case SDL_MOUSEBUTTONDOWN:
 xm = mEvent.button.x;
 ym = mEvent.button.y;
 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
        Mix_PlayChannel(-1,general_sound[1], 0);

 return i;

 }
 }
 case SDL_KEYDOWN:
 if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
 return 1;
 }

 default:
 break;
 }

 }
 SDL_RenderPresent(des);
 }

 return 1;
}

BaseObject gPause;

int Menu::ShowPause(SDL_Renderer* des, TTF_Font* font, Mix_Chunk* general_sound[5]) {
 if (gPause.LoadImg("img//background.png", des) == 0) {
 return 1;
 }

 const int RestartItemNum = 2;
 SDL_Rect posArr[RestartItemNum];
 posArr[0].x = SCREEN_WIDTH*0.5 - 180;
 posArr[0].y = 200;

 posArr[1].x = SCREEN_WIDTH*0.5 - 90;
 posArr[1].y = 260;

 TextObject textMenu[RestartItemNum];
 textMenu[0].SetText("CONTINUE");
 textMenu[0].SetColor(TextObject::WHITE_TEXT);
 textMenu[0].SetRect(posArr[0].x, posArr[0].y);

 textMenu[1].SetText("EXIT");
 textMenu[1].SetColor(TextObject::WHITE_TEXT);
 textMenu[1].SetRect(posArr[1].x, posArr[1].y);

 TextObject reason;

 reason.SetText("PAUSE");
 reason.SetColor(TextObject::WHITE_TEXT);
 reason.SetRect(SCREEN_WIDTH*0.5 - 100, 400);

 bool selected[RestartItemNum] = {0, 0};
 int xm = 0;
 int ym = 0;
 SDL_Event mEvent;
 while (true) {
 gPause.Render(des);
 for (int i = 0; i < RestartItemNum; i++) {
 textMenu[i].LoadFromRenderTextMenu(font, des);
 textMenu[i].RenderTextMenu(des);

 reason.LoadFromRenderTextMenu(font, des);
 reason.RenderTextMenu(des);
 // textMenu[i].CreateGameText(font, des);
 }

 while (SDL_PollEvent(&mEvent)) {
 switch (mEvent.type)
 {
 case SDL_QUIT:
 return 1;
 case SDL_MOUSEMOTION:
 xm = mEvent.motion.x;
 ym = mEvent.motion.y;

 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
 if (selected[i] == false) {
 selected[i] = true;
 textMenu[i].SetColor(TextObject::RED_TEXT);
 }
 }
 else {
 if (selected[i] == true) {
 selected[i] = false;
 textMenu[i].SetColor(TextObject::WHITE_TEXT);
 }
 }
 }

 break;

 case SDL_MOUSEBUTTONDOWN:
 xm = mEvent.button.x;
 ym = mEvent.button.y;
 for (int i = 0; i < RestartItemNum; i++) {
 if (SDLCommonFunction::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
        Mix_PlayChannel(-1,general_sound[1], 0);
 return i;

 }
 }
 case SDL_KEYDOWN:
 if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
 return 1;
 }

 default:
 break;
 }

 }
 SDL_RenderPresent(des);
 }

 return 1;
}


WinData::WinData()
{

}
WinData::~WinData()
{

}

