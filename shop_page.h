#ifndef SHOP_PAGE_H
#define SHOP_PAGE_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include <cstdio>
#include <vector>
#include <cstring>

#include "load_images.h"
#include "item.h"
#include "player.h"

class Shop_Page
{
 private:
 int POSX=0,LAST_POSX=0;
 int type=0;
 char name[TEXT_LENGTH_MAX];
 SDL_Surface* image=NULL,*name_image=NULL;
 int item_ids[100];
 std::vector<Item> items;
 int item_selected,item_click;
 int number_of_items,number_of_lines,number_of_columns;
 int title_size;

 public:
 Shop_Page();
 void Clear(bool _delete=false);
 void Set_POSX(int _x);
 void Set_LAST_POSX(int _x);
 void Set_name(char* _name);
 void Load();
 int Get_title_size();
 int Get_type();
 void Print(int x,int y,SDL_Surface* _screen);
 void Print_Title(int x,int y,SDL_Surface* _screen,bool selected,bool click);
 void Reset();
 int Start(SDL_Surface* _screen,SDL_Event* event);
};

#endif // SHOP_PAGE_H
