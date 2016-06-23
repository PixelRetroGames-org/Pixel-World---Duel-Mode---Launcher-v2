#ifndef ITEM_H
#define ITEM_H

#include "load_images.h"
#include "library.h"
///SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
///CSTRING
#include<cstring>
///CSTDIO
#include<cstdio>

#ifndef TEXT_LENGHT_MAX_DEFINED
const int TEXT_LENGHT_MAX=101;
#define TEXT_LENGHT_MAX_DEFINED
#endif // TEXT_LENGHT_MAX_DEFINED

extern const char *type_name[10];

class Item
{
 private:
 int id,cost;
 SDL_Surface *image,*name_image,*inventory_image;
 char name[TEXT_LENGHT_MAX],description[TEXT_LENGHT_MAX];
 int attack,defense,extra_money,fire_damage,fire_resistance,mana,hp;
 int type;

 public:
 Item();
 int Get_id();
 int Get_cost();
 int Get_type();
 void Set_type(int _type);
 void Set_id(int _id);
 int Load();
 void Print(int x,int y,SDL_Surface *_screen,bool selected);
 void Print_description(int x,int y,SDL_Surface *_screen,bool selected);
 void Print_image(int x,int y,SDL_Surface *_screen);
 void Print_inventory_image(int x,int y,SDL_Surface *_screen);
};

bool Is_potion(Item _item);

bool Is_potion(int _item_id);

#endif // ITEM_H
