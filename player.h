#ifndef PLAYER_H
#define PLAYER_H

#include "load_images.h"
#include "library.h"
#include "item.h"

const int NUMBER_OF_ITEMS_IDS=100;

extern const int INVENTORY_MAX_NUMBER_OF_ITEMS;

class Player
{
 private:
 char name[TEXT_LENGHT_MAX];
 int money,experience;
 int number_of_items,inventory_number_of_items;
 int number_of_items_bought[NUMBER_OF_ITEMS_IDS];
 Item items_bought[NUMBER_OF_ITEMS_IDS];
 int equipped_items_ids[10]={0,0,0,0,0,0,NULL,NULL,0,NULL};
 Item equipped_items[10];
 int inventory_item_selected,inventory_item_click;
 int pos_last_y;
 int basic_attack=5,basic_defense=0,basic_spell_damage=0,basic_spell_resistance=0,basic_movement_speed=10;
 SDL_Rect skin_image_position;
 public:
 int PLAYER_INFO_POSX=760,PLAYER_INFO_LAST_POSX=1240,SKIN_POSX=1130;
 Player();
 void Set_PLAYER_INFO_POSX(int _x);
 void Set_PLAYER_INFO_LAST_POSX(int _x);
 void Set_SKIN_POSX(int _x);
 void Set_name(char *_name);
 void Load();
 void Update();
 int Buy(int _item_id);
 void Sell(int _item_id);
 void Equip(int _item_id);
 void Unequip(int _item_id);
 bool Is_bought(int _item_id);
 bool Is_equipped(int _item_id);
 int Get_PLAYER_INFO_POSX();
 int Get_PLAYER_INFO_LAST_POSX();
 int Get_SKIN_POSX();
 int Get_pos_last_y();
 void Print_Character(int x,int y,SDL_Surface *_screen);
 void Print_Inventory(int x,int y,SDL_Surface *_screen);
 int Start_inventory(int x,int y,SDL_Surface *_screen,SDL_Event *event);
};

#endif //PLAYER_H
