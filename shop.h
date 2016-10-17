#ifndef SHOP_H
#define SHOP_H

#include "load_images.h"
#include "shop_page.h"
#include "settings.h"
//VECTOR
#include<vector>

class Shop
{
 private:
 char name[TEXT_LENGHT_MAX];
 bool changed_page=false;
 int number_of_pages,page_selected,page_click=0;
 int POSX=0,LAST_POSX=0;
 std::vector<Shop_Page> pages;

 public:
 Shop();
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
 char filename[TEXT_LENGHT_MAX]={NULL};

 public:
 void Set_filename(char *_filename);
 void Open(char *filename);
 void Reset();
 int Start(SDL_Surface *screen);
 int Start(SDL_Surface *screen,char *filename);
};

#endif //SHOP_H
