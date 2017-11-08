#include "buff.h"

#include <cstring>

//LOAD

Buff::Buff()
{
 id=type=duration=0;
 printable=self=false;
 damage=0,chance=0,mana=0;
 memset(name,0,sizeof name);
}

void Buff::Clear(bool _delete)
{
 if(_delete && image!=NULL)
    {
     Destroy_Texture(image);
     image=NULL;
    }
 id=type=duration=0;
 printable=self=false;
 damage=0,chance=0,mana=0;
 memset(name,0,sizeof name);
}

void Buff::Load()
{
 char filename[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 itoa(id,aux);
 strcpy(filename,"buffs/ids/");
 strcat(filename,aux);
 strcat(filename,".pwb");
 FILE *where=fopen(filename,"r");
 if(where==NULL)
    return;
 fscanf(where,"%d %d %d %d ",&type,&duration,&printable,&self);
 remaining_duration=duration;
 fgets(name,sizeof name,where);
 if(name[strlen(name)-1]=='\n')
    name[strlen(name)-1]=NULL;
 switch(type)
        {
         ///Transmissible
         case 1:fscanf(where,"%d %d ",&transmissible_buff_id,&chance);
                break;
         ///Damage
         case 2:fscanf(where,"%d ",&damage);
                break;
         ///Mana
         case 3:fscanf(where,"%d ",&mana);
                break;
         ///Self Buff
         case 4:fscanf(where,"%d %d %d %d %d %d",&attack,&defense,&spell_damage,&spell_resistance,&movement_speed,&life_steal);
                break;
         ///ShapeShift
         case 5:fscanf(where,"%d %d %d %d %d %d",&attack,&defense,&spell_damage,&spell_resistance,&movement_speed,&life_steal);
                int x,y;
                fscanf(where,"%d %d ",&x,&y);
                skin_image_position.w=x,skin_image_position.h=y;
                skin_image_position.x=skin_image_position.y=0;
                fgets(skin_name,sizeof skin_name,where);
                if(skin_name[strlen(skin_name)-1]=='\n')
                   skin_name[strlen(skin_name)-1]=NULL;
                break;
         ///Last Breath
         case 6:break;
         default:break;
        }
 strcpy(filename,"buffs/images/");
 strcat(filename,name);
 strcat(filename,".png");
 image=Load_Transparent_Texture(filename);
 fclose(where);
}

//SET
void Buff::Set_id(int _id)
{
 id=_id;
}

void Buff::Set_duration(int _duration)
{
 duration=_duration;
}

void Buff::Set_type(int _type)
{
 type=_type;
}

void Buff::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Buff::Set_damage(int _damage)
{
 damage=_damage;
}

void Buff::Set_transmitted_buff_id(int _id)
{
 transmitted_buff_id=_id;
}

//GET
int Buff::Get_id()
{
 return id;
}

int Buff::Get_duration()
{
 return duration;
}

int Buff::Get_remaining_duration()
{
 return remaining_duration;
}

int Buff::Get_type()
{
 return type;
}

char *Buff::Get_name()
{
 return name;
}

int Buff::Get_transmissible_buff_id()
{
 return transmissible_buff_id;
}

int Buff::Get_transmitted_buff_id()
{
 return transmitted_buff_id;
}

int Buff::Get_damage()
{
 return damage;
}

int Buff::Get_chance()
{
 return chance;
}

int Buff::Get_mana()
{
 return mana;
}

int Buff::Get_attack()
{
 return attack;
}

int Buff::Get_defense()
{
 return defense;
}

int Buff::Get_spell_damage()
{
 return spell_damage;
}

int Buff::Get_spell_resistance()
{
 return spell_resistance;
}

int Buff::Get_movement_speed()
{
 return movement_speed;
}

int Buff::Get_life_steal()
{
 return life_steal;
}

char *Buff::Get_skin_name()
{
 return skin_name;
}

SDL_Rect Buff::Get_skin_image_position()
{
 return skin_image_position;
}

bool Buff::Is_done()
{
 return (remaining_duration<=0);
}

bool Buff::Is_printable()
{
 return printable;
}

bool Buff::Is_self()
{
 return self;
}

bool Buff::Is_transmissible()
{
 return !self;
}

//GAME
void Buff::Decrease_remaining_duration()
{
 if(remaining_duration!=BUFF_PERMANENT)
    remaining_duration--;
}

void Buff::Reset()
{
 remaining_duration=duration;
 if(type==4 || type==5)
    remaining_duration--;
}

void Buff::Print_image(int x,int y,Texture *_screen)
{
 Apply_Texture(x,y,image,_screen);
}

//LOAD_ALL_BUFFS
int NUMBER_OF_BUFFS;

Buff All_BUFFS[NUMBER_OF_ALL_BUFFS];

void Load_all_buffs()
{
 FILE *where=fopen("buffs/all_buffs.pwab","r");
 fscanf(where,"%d ",&NUMBER_OF_BUFFS);
 for(int i=0;i<NUMBER_OF_BUFFS;i++)
     {
      int _id;
      fscanf(where,"%d ",&_id);
      All_BUFFS[i].Set_id(_id);
      All_BUFFS[i].Load();
     }
 fclose(where);
}
