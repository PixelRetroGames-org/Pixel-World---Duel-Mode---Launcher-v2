#ifndef MENU_H
#define MENU_H

#include "library.h"
#include "texture.h"
#include "load_images.h"
#include "load_audio_effects.h"
#include "settings.h"
#include "controller.h"

#include <cstring>

const int NUMBER_OF_OPTIONS_MAX=10;

class Menu_Option
{
 private:
 char text[TEXT_LENGTH_MAX],font_name[TEXT_LENGTH_MAX];
 SDL_Color color;
 Texture *text_image=NULL;
 int font_size;
 SDL_Rect screen_pos;
 public:
 void Clear();
 void Load(FILE *where);
 void Set_text(char *_text);
 void Set_color(Uint8 r,Uint8 g,Uint8 b);
 void Set_font_name(char *_font_name);
 void Set_font_size(int _size);
 SDL_Rect Get_screen_pos();
 void Print_text(Texture *_screen,bool selected,bool click);
};

class Menu
{
 private:
 int number_of_options,selector_position=-1,click_position=-1;
 Menu_Option options[NUMBER_OF_OPTIONS_MAX];
 Texture *background=NULL,*title=NULL;
 SDL_Rect title_position;
 bool is_main_menu=false;
 public:
 void Clear();
 void Load(const char *filename);
 void Set_number_of_options(int _noptions);
 void Set_option(int _pos,Menu_Option _x);
 void Print_options(Texture *_screen);
 int Start(Texture *_screen,bool *reload=NULL);
};

#endif //MENU_H
