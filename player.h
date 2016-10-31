#ifndef PLAYER_H
#define PLAYER_H

#include "load_images.h"
#include "library.h"
#include "item.h"
#include "spell.h"
#include <vector>
#include <algorithm>
#include <bitset>

const int NUMBER_OF_ITEMS_IDS=100;

extern const int INVENTORY_MAX_NUMBER_OF_ITEMS;

const int NUMBER_OF_MAX_ACTIVE_BUFFS=4,NUMBER_OF_MAX_KEYS=10;

class Player
{
 private:
 char name[TEXT_LENGHT_MAX];
 int money,experience;
 int number_of_items,inventory_number_of_items,inventory_number_of_spells;
 int number_of_items_bought[NUMBER_OF_ITEMS_IDS];
 Item items_bought[NUMBER_OF_ITEMS_IDS];
 int equipped_items_ids[10]={0,0,0,0,0,0,NULL,NULL,0,NULL};
 Item equipped_items[10];
 int inventory_item_selected,inventory_item_click;
 int pos_last_y;
 int PLAYER_INFO_POSX=760,PLAYER_INFO_LAST_POSX=1130,SKIN_POSX=1130;
 ///Game
 int hp,mana,mental_health;
 int attack=0,defense=0,extra_money=0,spell_damage=0,spell_resistance=0,movement_speed=0,life_steal=0;
 int basic_hp=1000,basic_mana=100,basic_mental_health=100;
 int basic_attack=5,basic_defense=0,basic_spell_damage=10,basic_spell_resistance=0,basic_movement_speed=0,basic_life_steal=0;
 int map_positionX=0,map_positionY=0;
 int skin_size_w=40,skin_size_h=40;
 int velocityX=0,velocityY=0;
 SDL_Surface *skin=NULL,*name_image=NULL,*hp_image=NULL,*mana_image=NULL;
 SDL_Rect skin_image_position;
 SDL_Rect original_skin_image_position;
 bool is_blocked=false,can_attack=true;
 std::vector<Buff> active_buffs;
 std::vector<int> printable_item_buffs_id;
 int number_of_spells=0;
 Spell spells[4];
 std::bitset<NUMBER_OF_MAX_KEYS> keys;
 ///Game

 public:
 Player();
 void Clear(bool _delete=false);
 void Set_name(char *_name);
 void Set_PLAYER_INFO_POSX(int _x);
 void Set_PLAYER_INFO_LAST_POSX(int _x);
 void Set_SKIN_POSX(int _x);
 void Set_money(int _money);
 void Set_experience(int _experience);
 void Load();
 void Update();
 int Buy(int _item_id);
 void Sell(int _item_id);
 void Equip(int _item_id);
 void Unequip(int _item_id);
 bool Is_bought(int _item_id);
 bool Is_equipped(int _item_id);
 int Get_pos_last_y();
 int Get_PLAYER_INFO_POSX();
 int Get_PLAYER_INFO_LAST_POSX();
 int Get_money();
 int Get_experience();
 void Print_Character(int x,int y,SDL_Surface *_screen);
 void Print_items(int x,int y,SDL_Surface *_screen);
 void Print_Inventory(int x,int y,SDL_Surface *_screen,bool options=true,int type=0);
 void Print_Inventory_equipped_items(int x,int y,SDL_Surface *_screen,bool options=true,int type=1);
 int Start_inventory(int x,int y,SDL_Surface *_screen,SDL_Event *event,int type=0);

 ///Game
 void Set_hp(int _hp);
 void Set_mana(int _mana);
 void Set_mental_health(int _mental_health);
 void Set_map_position(int x,int y);
 void Set_skin_image_position(int x,int y);
 void Set_skin_image_position(SDL_Rect _skin_image_position);
 void Reset_skin_image_position();
 void Set_velocityX(int _velocityX);
 void Set_velocityY(int _velocityY);
 void Block();
 void Block_attack();
 void Unblock();
 void Unblock_attack();
 int Get_hp();
 int Get_mana();
 int Get_attack();
 int Get_defense();
 int Get_extra_money();
 int Get_spell_damage();
 int Get_spell_resistance();
 int Get_movement_speed();
 int Get_life_steal();
 int Get_map_positionX();
 int Get_map_positionY();
 int Get_velocityX();
 int Get_velocityY();
 int Get_skinW();
 int Get_skinH();
 void Set_skin(char *skin_name);
 void Load_skin();
 void Update_skin_image_position();
 void Print_name(int x,int y,SDL_Surface *_screen);
 void Print_name(SDL_Surface *_screen);
 void Print_hp(int x,int y,SDL_Surface *_screen);
 void Print_hp(SDL_Surface *_screen);
 void Print_mana(int x,int y,SDL_Surface *_screen);
 void Print_mana(SDL_Surface *_screen);
 void Print_skin(int x,int y,int mapX,int mapY,SDL_Surface *_screen);
 void Print_skin_free(int x,int y,SDL_Surface *_screen);
 void Equip_items();
 bool Is_blocked();
 bool Can_attack();
 void Move_X();
 void Move_Y();
 void Use_hp_potion();
 void Use_mana_potion();
 //void Cast(int spell);
 ///Game

 ///Buffs
 void Apply_buff(Buff *_buff);
 void Remove_buff(Buff *_buff);
 void Apply_all_buffs();
 void Apply_new_active_buff();
 void Apply_active_buffs();
 void Apply_item_buffs();
 void Add_buff(Buff _buff);
 void Get_transmitted_buffs(std::vector<Buff> *_buffs);
 void Print_buffs(int x,int y,SDL_Surface *_screen);
 ///Spells
 bool Pay_Spell(int spell_pos);
 Spell Get_Spell(int spell_pos);
 void Block_Spell(int spell_pos);
 void Decrease_Spell_time_blocked(int spell_pos);
 void Decrease_all_Spells_time_blocked();
 bool Spell_Is_blocked(int spell_pos);
 ///Keys
 void Add_key(int _key_id);
 void Remove_key(int _key_id);
 std::bitset<NUMBER_OF_MAX_KEYS> Get_keys();
};

#endif //PLAYER_H
