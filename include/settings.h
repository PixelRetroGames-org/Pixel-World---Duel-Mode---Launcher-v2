#ifndef SETTINGS_H
#define SETTINGS_H

#include "library.h"
#include "texture.h"
#include "load_images.h"
#include "errors.h"
#include "controller.h"

#include <cstdio>

extern int DISPLAY_RESOLUTION_W,DISPLAY_RESOLUTION_H;

extern int RESOLUTION_W,RESOLUTION_H,RESOLUTION;
extern int DISPLAY_MODE,VOLUME,VOL;
extern bool POWER_SAVER,AUTO_ATTACK;

void Load_Settings();
void Save_Settings();

void Set_RESOLUTION_W(int X);
void Set_RESOLUTION_H(int Y);
void Set_DISPLAY_MODE(int MODE);

void Validate_RESOLUTION();

void Reset_Saves();

void Graphic_Settings(Texture *_screen);
void Graphic_Settings(Texture *_screen,SDL_Event *event);
void Graphic_Change_Resolution(int x,int y,Texture *_screen,SDL_Event *event);
void Graphic_Change_Display_Mode(int x,int y,Texture *_screen,SDL_Event *event);
void Graphic_Change_Volume(int x,int y,Texture *_screen,SDL_Event *event);
void Graphic_Power_Saver(int x,int y,Texture *_screen,SDL_Event *event);
void Graphic_Auto_Attack(int x,int y,Texture *_screen,SDL_Event *event);
void Graphic_Reset_Saves(int x,int y,Texture *_screen,SDL_Event *event);
bool Graphic_Back(int x,int y,Texture *_screen,SDL_Event *event);

void Get_Display_Resolution();
#endif // SETTINGS_H
