#ifndef BUFF_H
#define BUFF_H

#include "library.h"
#include "texture.h"

#define BUFF_PERMANENT -1

class Buff
{
 private:
 bool printable=false,self=true;
 int transmissible_buff_id=0,transmitted_buff_id=0;
 int damage=0,chance=0,mana=0;
 ///SELF BUFF / CURSE
 int attack=0,defense=0,spell_damage=0,spell_resistance=0,movement_speed=0,life_steal=0;
 ///SHAPESHIFT
 char skin_name[TEXT_LENGTH_MAX]={NULL};
 SDL_Rect skin_image_position;
 int id,duration,type;
 int remaining_duration;
 char name[TEXT_LENGTH_MAX]={NULL};
 Texture *image=NULL;

 public:
 Buff();
 void Clear(bool _delete=false);
 void Load();
 void Set_id(int _id);
 void Set_duration(int _duration);
 void Set_type(int _type);
 void Set_name(char *_name);
 void Set_damage(int _damage);
 void Set_transmitted_buff_id(int _id);

 int Get_id();
 int Get_duration();
 int Get_remaining_duration();
 int Get_type();
 char *Get_name();
 int Get_transmissible_buff_id();
 int Get_transmitted_buff_id();
 int Get_damage();
 int Get_chance();
 int Get_mana();

 int Get_attack();
 int Get_defense();
 int Get_spell_damage();
 int Get_spell_resistance();
 int Get_movement_speed();
 int Get_life_steal();

 char *Get_skin_name();
 SDL_Rect Get_skin_image_position();

 bool Is_done();
 bool Is_printable();
 bool Is_self();
 bool Is_transmissible();

 void Decrease_remaining_duration();
 void Reset();
 void Print_image(int x,int y,Texture *_screen=NULL);
};

const int NUMBER_OF_ALL_BUFFS=10;

extern int NUMBER_OF_BUFFS;

extern Buff All_BUFFS[NUMBER_OF_ALL_BUFFS];

void Load_all_buffs();

#endif // BUFF_H
