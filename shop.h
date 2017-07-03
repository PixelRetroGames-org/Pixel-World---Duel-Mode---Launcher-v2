#ifndef SHOP_H
#define SHOP_H

#include "load_images.h"
#include "shop_page.h"
#include "settings.h"

#include <vector>
#include <cstring>

class Shop
{
 private:
 char name[TEXT_LENGTH_MAX];
 bool changed_page=false;
 int number_of_pages,page_selected,page_click=0;
 int POSX=0,LAST_POSX=0;
 std::vector<Shop_Page> pages;
 Timer *controller_timer;

 public:
 Shop();
 void Clear();
 int Get_shop_page_type();
 void Set_POSX(int _x);
 void Set_LAST_POSX(int _x);
 void Set_name(char *_name);
 void Load();
 void Print(Texture *_screen);
 void Reset();
 void Set_Controller_Timer(Timer *_controller_timer);
 inline int Start(Texture *_screen,SDL_Event *event);
};

class Shop_Screen
{
 private:
 Shop shop;
 Player player;
 char filename[TEXT_LENGTH_MAX]={NULL};
 Timer controller_timer;

 public:
 int Start(Texture *screen);
 int Start(Texture *screen,char *shop_name,char *player_name);
};

#endif //SHOP_H
