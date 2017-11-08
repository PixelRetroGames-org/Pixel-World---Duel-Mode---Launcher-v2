#ifndef PLAYER_H
#define PLAYER_H

#include "load_images.h"
#include "library.h"
#include "texture.h"
#include "item.h"
#include "spell.h"

#include <vector>
#include <algorithm>
#include <bitset>
#include <map>
#include <cstring>

const int NUMBER_OF_ITEMS_IDS=100;

extern const int INVENTORY_MAX_NUMBER_OF_ITEMS,INVENTORY_MAX_NUMBER_OF_POTIONS;

const int NUMBER_OF_MAX_ACTIVE_BUFFS=4;

class Player
{
 private:
 char name[TEXT_LENGTH_MAX];
 int id;
 int money,experience;
 int number_of_items,inventory_number_of_items,inventory_number_of_spells;
 std::map<int,int> number_of_items_bought;
 std::map<int,Item> items_bought;
 int equipped_items_ids[10]={0,0,0,0,0,0,NULL,NULL,0,NULL};
 Item equipped_items[10];
 int inventory_item_selected,inventory_item_click;
 int inventory_item_selected_position;
 int pos_last_y;
 int PLAYER_INFO_POSX=760,PLAYER_INFO_LAST_POSX=1130,SKIN_POSX=1130;
 ///Game
 double hp,mana,mental_health;
 int attack=0,defense=0,extra_money=0,spell_damage=0,spell_resistance=0,movement_speed=0,life_steal=0;
 double basic_hp=100.0,basic_mana=100.0,basic_mental_health=100.0;
 int basic_attack=5,basic_defense=0,basic_spell_damage=10,basic_spell_resistance=0,basic_movement_speed=2,basic_life_steal=0;
 int map_positionX=0,map_positionY=0;
 int skin_size_w=PIXELS_PER_INGAME_UNIT,skin_size_h=PIXELS_PER_INGAME_UNIT;
 int velocityX=0,velocityY=0;
 Texture *skin=NULL,*name_image=NULL,*hp_image=NULL,*mana_image=NULL;
 SDL_Rect skin_image_position;
 SDL_Rect original_skin_image_position;
 bool is_blocked=false,can_attack=true;
 std::vector<Buff> active_buffs;
 std::vector<int> printable_item_buffs_id;
 int number_of_spells=0;
 Spell spells[4];
 std::bitset<NUMBER_OF_MAX_KEYS> keys,progress;
 bool is_immortal=false;
 int number_of_wins=0;
 Timer *controller_timer;
 Timer item_sell_timer;
 bool item_sell_timer_started=false;
 ///AI
 Timer spell_timer[4],attack_timer;
 int counter=-1;

 public:
 Player();
 void Clear(bool _delete=false);
 void Set_name(char *_name);
 void Set_PLAYER_INFO_POSX(int _x);
 void Set_PLAYER_INFO_LAST_POSX(int _x);
 void Set_SKIN_POSX(int _x);
 void Set_money(int _money);
 void Set_experience(int _experience);
 void Set_id(int _id);
 void Set_Controller_Timer(Timer *_controller_timer);
 void Set_inventory_item_selected_position(int _inventory_item_selected_position,int type);
 void Add_Default_keys();
 void Load();
 void Fast_Reload();
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
 int Get_inventory_number_of_items();
 int Get_inventory_number_of_spells();
 void Print_Character(int x,int y,Texture *_screen);
 void Print_items(int x,int y,Texture *_screen);
 void Print_Inventory(int x,int y,Texture *_screen,bool options=true,int type=0,bool allow_sales=true);
 void Print_Inventory_equipped_items(int x,int y,Texture *_screen,bool options=true,int type=1,bool allow_sales=true);
 inline int Get_next_inventory_item_id(int n,int type);
 int Start_inventory(int x,int y,Texture *_screen,SDL_Event *event,int type=0,bool allow_sales=true);

 ///Game
 void Set_hp(double _hp);
 void Set_mana(double _mana);
 void Set_mental_health(double _mental_health);
 void Set_map_position(int x,int y);
 void Set_skin_image_position(int x,int y);
 void Set_skin_image_position(SDL_Rect _skin_image_position);
 void Reset_skin_image_position();
 void Set_velocityX(int _velocityX);
 void Set_velocityY(int _velocityY);
 void Set_movement_speed(int _movement_speed);
 void Block();
 void Block_attack();
 void Unblock();
 void Unblock_attack();
 double Get_hp();
 double Get_mana();
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
 char *Get_name();
 void Set_skin(char *skin_name);
 void Load_skin();
 void Update_skin_image_position();
 void Print_name(int x,int y,Texture *_screen);
 void Print_name(Texture *_screen);
 void Print_hp(int x,int y,Texture *_screen);
 void Print_hp(Texture *_screen);
 void Print_mana(int x,int y,Texture *_screen);
 void Print_mana(Texture *_screen);
 void Print_skin(int x,int y,int mapX,int mapY,Texture *_screen);
 void Print_skin_free(int x,int y,Texture *_screen);
 void Equip_items();
 bool Is_blocked();
 bool Can_attack();
 void Move_X();
 void Move_Y();
 void Use_hp_potion();
 void Use_mana_potion();
 bool Is_immortal();
 void Reset(int _map_positionX,int _map_positionY);
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
 void Print_buffs(int x,int y,Texture *_screen);
 ///Spells
 bool Pay_Spell(int spell_pos);
 Spell Get_Spell(int spell_pos);
 int Get_number_of_spells();
 void Block_Spell(int spell_pos);
 void Decrease_Spell_time_blocked(int spell_pos);
 void Decrease_all_Spells_time_blocked();
 bool Spell_Is_blocked(int spell_pos);
 void Print_spells(int x,int y,Texture *_screen);
 ///Keys
 void Add_key(int _key_id);
 void Add_keys(std::bitset<NUMBER_OF_MAX_KEYS> *_keys_ids);
 void Remove_key(int _key_id);
 void Remove_keys(std::bitset<NUMBER_OF_MAX_KEYS> *_keys_ids);
 std::bitset<NUMBER_OF_MAX_KEYS> *Get_keys();
 std::bitset<NUMBER_OF_MAX_KEYS> *Get_progress();
 ///Score
 void Increase_number_of_wins();
 int Get_number_of_wins();
 ///AI
 void Start_spell_timer(int _spell_id);
 int Get_spell_timer_ticks(int _spell_id);
 void Start_attack_timer();
 int Get_attack_timer_ticks();
 void Set_counter(int _counter);
 int Get_counter();
};

#endif //PLAYER_H
