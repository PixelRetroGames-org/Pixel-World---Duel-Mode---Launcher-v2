#ifndef SPELL_H
#define SPELL_H

#include "buff.h"
#include<vector>

#ifndef TEXT_LENGHT_MAX_DEFINED
const int TEXT_LENGHT_MAX=101;
#define TEXT_LENGHT_MAX_DEFINED
#endif // TEXT_LENGHT_MAX_DEFINED

class Spell
{
 private:
 int type,id;
 char name[TEXT_LENGHT_MAX],map_name[TEXT_LENGHT_MAX];
 int range,mana_cost,health_cost,mental_health_cost,cooldown;
 int time_blocked=0;
 std::vector<Buff> buffs;

 public:
 Spell();
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
 bool Pay(int *_mana,int *_health,int *_mental_health);
};

#endif // SPELL_H
