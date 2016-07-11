#include "buff.h"

#include<cstring>

//LOAD

void Buff::Load()
{
 char filename[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};
 itoa(id,aux);
 strcpy(filename,"buffs/");
 strcat(filename,aux);
 strcat(filename,".pwb");
 FILE *where=fopen(filename,"r");
 if(where==NULL)
    return;
 fscanf(where,"%d %d ",&type,&duration);
 fgets(name,sizeof name,where);
 if(name[strlen(name)-1]=='\n')
    name[strlen(name)-1]=NULL;
 strcpy(filename,"buffs/images/");
 strcat(filename,name);
 strcat(filename,".bmp");
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

//GET
int Buff::Get_id()
{
 return id;
}

int Buff::Get_duration()
{
 return duration;
}

int Buff::Get_type()
{
 return type;
}

char* Buff::Get_name()
{
 return name;
}

//GAME
void Buff::Decrease_duration()
{
 if(duration!=BUFF_PERMANENT)
    duration--;
}

void Buff::Print(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y,image,_screen);
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
}
