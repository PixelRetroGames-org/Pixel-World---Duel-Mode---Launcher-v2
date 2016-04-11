#ifndef MENU_H
#define MENU_H

//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
//LIBRARY
#include "library.h"
#include "load_images.h"
//CSTRING
#include<cstring>

const int TEXT_LENGHT_MAX=100,NUMBER_OF_OPTIONS_MAX=10;

class Menu_Option
{
 private:
 char text[TEXT_LENGHT_MAX],font_name[TEXT_LENGHT_MAX];
 SDL_Color color;
 int font_size;
 SDL_Rect screen_pos,background_pos;
 public:
 void Load(FILE *where);
 void Set_text(char *_text);
 void Set_screen_pos(int x,int y);
 void Set_background_pos(int x,int y);
 void Set_color(Uint8 r,Uint8 g,Uint8 b);
 void Set_font_name(char *_font_name);
 void Set_font_size(int _size);
 void Set_size();
 SDL_Rect Get_screen_pos();
 SDL_Rect Get_background_pos();
 void Print_text(SDL_Surface *_screen,bool selected,bool click);
};

class Menu
{
 private:
 int number_of_options,selector_position,click_position;
 Menu_Option options[NUMBER_OF_OPTIONS_MAX];
 public:
 void Load(const char *filename);
 void Set_number_of_options(int _noptions);
 void Set_option(int _pos,Menu_Option _x);
 void Print_options(SDL_Surface *_screen);
 int Start(SDL_Surface *_screen);
};

#endif //MENU_H
