#ifndef SHOP_PAGE_H
#define SHOP_PAGE_H

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
 Texture *image=NULL,*name_image=NULL;
 int item_ids[100];
 std::vector<Item> items;
 int item_selected,item_click;
 int number_of_items,number_of_lines,number_of_columns;
 int title_size;
 Timer *controller_timer;

 public:
 Shop_Page();
 void Clear(bool _delete=false);
 void Set_POSX(int _x);
 void Set_LAST_POSX(int _x);
 void Set_name(char *_name);
 void Load();
 int Get_title_size();
 int Get_type();
 void Print(int x,int y,Texture *_screen);
 void Print_Title(int x,int y,Texture *_screen,bool selected,bool click);
 void Reset();
 void Set_Controller_Timer(Timer *_controller_timer);
 int Start(Texture *_screen,SDL_Event *event);
};

#endif // SHOP_PAGE_H
