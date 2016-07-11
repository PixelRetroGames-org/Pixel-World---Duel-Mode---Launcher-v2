#ifndef BUFF_H
#define BUFF_H

#include "library.h"

#define BUFF_PERMANENT -1

#ifndef TEXT_LENGHT_MAX_DEFINED
const int TEXT_LENGHT_MAX=101;
#define TEXT_LENGHT_MAX_DEFINED
#endif // TEXT_LENGHT_MAX_DEFINED

class Buff
{
 private:
 int id,duration,type;
 char name[TEXT_LENGHT_MAX]={NULL};
 SDL_Surface *image=NULL;

 public:
 //Buff();
 void Load();
 void Set_id(int _id);
 void Set_duration(int _duration);
 void Set_type(int _type);
 void Set_name(char *_name);

 int Get_id();
 int Get_duration();
 int Get_type();
 char* Get_name();

 void Decrease_duration();
 void Print(int x,int y,SDL_Surface *_screen);
};

const int NUMBER_OF_ALL_BUFFS=10;

extern int NUMBER_OF_BUFFS;

void Load_all_buffs();

#endif // BUFF_H

