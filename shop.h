#ifndef SHOP_H
#define SHOP_H

#include "load_images.h"
#include "shop_page.h"
#include "settings.h"
//VECTOR
#include <vector>

class Shop
{
 private:
 char name[TEXT_LENGTH_MAX];
 bool changed_page=false;
 int number_of_pages,page_selected,page_click=0;
 int POSX=0,LAST_POSX=0;
 std::vector<Shop_Page> pages;

 public:
 Shop();
 void Clear();
 int Get_shop_page_type();
 void Set_POSX(int _x);
 void Set_LAST_POSX(int _x);
 void Set_name(char *_name);
 void Load();
 void Print(SDL_Surface *_screen);
 void Reset();
 inline int Start(SDL_Surface *_screen,SDL_Event *event);
};

class Shop_Screen
{
 private:
 Shop shop;
 Player player;
 char filename[TEXT_LENGTH_MAX]={NULL};

 public:
 int Start(SDL_Surface *screen);
 int Start(SDL_Surface *screen,char *shop_name,char *player_name);
};

#endif //SHOP_H
