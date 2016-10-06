#include "spell.h"

#include<cstring>

Spell::Spell()
{
 type=id=0;
 memset(name,0,sizeof name);
 mana_cost=health_cost=mental_health_cost=0;
 range=0;
 buffs.clear();
}

void Spell::Load()
{
 char filename[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};
 itoa(id,aux);
 strcpy(filename,"spells/");
 strcat(filename,aux);
 strcat(filename,".pws");
 FILE *where=fopen(filename,"r");
 fscanf(where,"%d ",&type);
 fgets(name,sizeof name,where);
 if(name[strlen(name)-1]=='\n')
    name[strlen(name)-1]=NULL;
 int number_of_buffs=0;
 fscanf(where,"%d ",&number_of_buffs);
 for(int i=0;i<number_of_buffs;i++)
     {
      int buff_id;
      Buff aux;
      fscanf(where,"%d ",&buff_id);
      aux.Set_id(buff_id);
      aux.Load();
      buffs.push_back(aux);
     }
 fscanf(where,"%d %d %d %d %d ",&range,&mana_cost,&health_cost,&mental_health_cost,&cooldown);
 fgets(map_name,sizeof map_name,where);
 if(map_name[strlen(map_name)-1]=='\n')
    map_name[strlen(map_name)-1]=NULL;
 fclose(where);
}

void Spell::Set_id(int _id)
{
 id=_id;
}

int Spell::Get_id()
{
 return id;
}

int Spell::Get_type()
{
 return type;
}

void Spell::Get_Buffs(std::vector<Buff> *_buffs)
{
 *_buffs=buffs;
}

int Spell::Get_range()
{
 return range;
}

char *Spell::Get_map_name()
{
 return map_name;
}

bool Spell::Is_blocked()
{
 return time_blocked;
}

void Spell::Decrease_time_blocked()
{
 if(time_blocked==0)
    return;
 time_blocked--;
}

void Spell::Block()
{
 time_blocked=cooldown;
}

bool Spell::Pay(int *_mana,int *_health,int *_mental_health)
{
 if((*_mana)<mana_cost || (*_health)<health_cost || (*_mental_health)<mental_health_cost)
    return false;
 (*_mana)-=mana_cost;
 (*_health)-=health_cost;
 (*_mental_health)-=mental_health_cost;
 return true;
}
