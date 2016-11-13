#ifndef SETTINGS_H
#define SETTINGS_H

#include "library.h"
#include "load_images.h"

#ifndef TEXT_LENGHT_MAX_DEFINED
const int TEXT_LENGTH_MAX=101;
#define TEXT_LENGHT_MAX_DEFINED
#endif // TEXT_LENGHT_MAX_DEFINED

extern int RESOLUTION_X,RESOLUTION_Y,RESOLUTION;
extern int DISPLAY_MODE,VOLUME,VOL;
extern bool POWER_SAVER;

void Load_Settings();
void Save_Settings();

void Set_RESOLUTION_X(int X);
void Set_RESOLUTION_Y(int Y);
void Set_DISPLAY_MODE(int MODE);

void Graphic_Settings(SDL_Surface *_screen);
void Graphic_Settings(SDL_Surface *_screen,SDL_Event *event);
void Graphic_Change_Resolution(int x,int y,SDL_Surface *_screen,SDL_Event *event);
void Graphic_Change_Display_Mode(int x,int y,SDL_Surface *_screen,SDL_Event *event);
void Graphic_Change_Volume(int x,int y,SDL_Surface *_screen,SDL_Event *event);
void Graphic_Power_Saver(int x,int y,SDL_Surface *_screen,SDL_Event *event);
bool Graphic_Back(int x,int y,SDL_Surface *_screen,SDL_Event *event);

#endif // SETTINGS_H
