#ifndef SPELL_H
#define SPELL_H

#include "buff.h"

#include <vector>
#include <cstring>

class Spell
{
 private:
 int type,id;
 char name[TEXT_LENGTH_MAX],map_name[TEXT_LENGTH_MAX];
 int range,mana_cost,health_cost,mental_health_cost,cooldown;
 int time_blocked=0;
 std::vector<Buff> buffs;
 Texture *image=NULL;
 Mix_Chunk *sound_effect=NULL;

 public:
 Spell();
 void Clear(bool _delete=false);
 void Load();
 void Set_id(int _id);
 int Get_id();
 int Get_type();
 void Get_Buffs(std::vector<Buff> *_buffs);
 int Get_range();
 char *Get_map_name();
 bool Is_blocked();
 void Decrease_time_blocked();
 void Block();
 void Unblock();
 bool Pay(double *_mana,double *_health,double *_mental_health);
 bool Can_Pay(int _mana,int _health,int _mental_health);
 void Print_image(int x,int y,Texture *_screen);
 void Play_sound_effect(int channel);
};

#endif // SPELL_H
