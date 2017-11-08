#include "spell.h"

Spell::Spell()
{
 type=id=0;
 memset(name,0,sizeof name);
 memset(map_name,0,sizeof map_name);
 mana_cost=health_cost=mental_health_cost=0;
 range=0;
 //buffs.clear();
}

void Spell::Clear(bool _delete)
{
 type=id=0;
 memset(name,0,sizeof name);
 memset(map_name,0,sizeof map_name);
 mana_cost=health_cost=mental_health_cost=0;
 range=mana_cost=health_cost=mental_health_cost=cooldown=time_blocked=0;
 for(int i=0;i<buffs.size() && !buffs.empty();i++)
     buffs[i].Clear(_delete);
 std::vector<Buff>().swap(buffs);
 Destroy_Texture(image);
 image=NULL;
 Mix_FreeChunk(sound_effect);
 sound_effect=NULL;
 //buffs.clear();
}

void Spell::Load()
{
 char filename[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 itoa(id,aux);
 strcpy(filename,"spells/");
 strcat(filename,aux);
 strcat(filename,".pws");
 FILE *where=fopen(filename,"r");
 fscanf(where,"%d ",&type);
 fgets(name,sizeof name,where);
 if(name[strlen(name)-1]=='\n')
    name[strlen(name)-1]=NULL;
 strcpy(aux,"shop/items/inventory/images/");
 strcat(aux,name);
 strcat(aux,".png");
 image=Load_Transparent_Texture(aux);
 int number_of_buffs=0;
 fscanf(where,"%d ",&number_of_buffs);
 Buff aux_buff;
 for(int i=0;i<number_of_buffs;i++)
     {
      int buff_id;
      fscanf(where,"%d ",&buff_id);
      aux_buff.Set_id(buff_id);
      aux_buff.Load();
      buffs.push_back(aux_buff);
     }
 fscanf(where,"%d %d %d %d %d ",&range,&mana_cost,&health_cost,&mental_health_cost,&cooldown);
 fgets(map_name,sizeof map_name,where);
 if(map_name[strlen(map_name)-1]=='\n')
    map_name[strlen(map_name)-1]=NULL;
 if(map_name[strlen(map_name)-1]=='~')
    map_name[strlen(map_name)-1]=NULL;
 char sound_effect_name[TEXT_LENGTH_MAX]={NULL};
 fgets(sound_effect_name,sizeof sound_effect_name,where);
 if(sound_effect_name[strlen(sound_effect_name)-1]=='\n')
    sound_effect_name[strlen(sound_effect_name)-1]=NULL;
 strcpy(aux,"spells/sound effects/");
 strcat(aux,sound_effect_name);
 strcat(aux,".wav");
 sound_effect=Mix_LoadWAV(aux);
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
 return time_blocked!=0;
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

void Spell::Unblock()
{
 time_blocked=0;
}

bool Spell::Pay(double *_mana,double *_health,double *_mental_health)
{
 if((*_mana)<mana_cost || (*_health)<health_cost || (*_mental_health)<mental_health_cost)
    return false;
 (*_mana)-=1.0*mana_cost;
 (*_health)-=1.0*health_cost;
 (*_mental_health)-=1.0*mental_health_cost;
 return true;
}

bool Spell::Can_Pay(int _mana,int _health,int _mental_health)
{
 if((_mana)<mana_cost || (_health)<health_cost || (_mental_health)<mental_health_cost)
    return false;
 return true;
}

void Spell::Print_image(int x,int y,Texture *_screen)
{
 Apply_Texture(x,y,image,_screen);
}

void Spell::Play_sound_effect(int channel)
{
 Mix_PlayChannel(channel,sound_effect,0);
}
