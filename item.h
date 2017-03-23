#ifndef ITEM_H
#define ITEM_H

#include "load_images.h"
#include "library.h"
///SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
///CSTRING
#include <cstring>
///CSTDIO
#include <cstdio>

#include "buff.h"

#ifndef DESCRIPTION_LINES_MAX_DEFINED
const int DESCRIPTION_LINES_MAX=8;
#define DESCRIPTION_LINES_MAX_DEFINED
#endif // DESCRIPTION_LINES_MAX_DEFINED

extern const char *type_name[11];

class Item
{
 private:
 int id,cost,spell_id;
 SDL_Surface *image,*inventory_image,*name_image,*price_image,*description_image[DESCRIPTION_LINES_MAX];
 int description_lines=0;
 char name[TEXT_LENGTH_MAX],description[4*TEXT_LENGTH_MAX];
 int attack,defense,extra_money,spell_damage,spell_resistance,mana,hp,movement_speed;
 int type;
 Buff buff;

 public:
 Item();
 void Clear(bool _delete=false);
 int Get_spell_id();
 int Get_id();
 int Get_cost();
 int Get_type();
 int Get_attack();
 int Get_defense();
 int Get_extra_money();
 int Get_spell_damage();
 int Get_spell_resistance();
 int Get_movement_speed();
 int Get_mana();
 int Get_hp();
 SDL_Surface *Get_image();
 SDL_Surface *Get_inventory_image();
 SDL_Surface *Get_skin();
 void Set_type(int _type);
 void Set_id(int _id);
 int Load();
 void Print(int x,int y,SDL_Surface *_screen,bool selected);
 void Print_description(int x,int y,SDL_Surface *_screen,bool selected);
 void Print_image(int x,int y,SDL_Surface *_screen);
 void Print_inventory_image(int x,int y,SDL_Surface *_screen);

 //BUFFS
 int Get_buff_id();
 Buff Get_Buff();
 void Set_Buff(Buff _buff);
 void Buff_decrease_remaining_duration();
 void Buff_Reset();
 bool Buff_Is_done();
 bool Buff_Is_printable();
};

bool Is_potion(Item _item);

bool Is_potion(int _item_id);

#endif // ITEM_H
