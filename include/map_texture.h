#ifndef MAP_TEXTURE_H
#define MAP_TEXTURE_H

#include "library.h"
#include "texture.h"
#include "load_images.h"
#include "buff.h"

class Map_Texture
{
 private:
 int id=0;
 char name[TEXT_LENGTH_MAX]={NULL};
 Texture *image=NULL;
 SDL_Rect image_position;
 int type,damage;
 Buff buff;
 int number_of_frames=1;
 int frame_change_delay=0;
 int power=0;
 bool print_before_player=false,obstacle=false,light=false,animation=false,is_trigger=false,is_passer=false;

 public:
 Map_Texture();
 void Clear(bool _delete=true);
 void Set_name(char *_name);
 void Set_type(int _type);
 void Set_id(int _id);
 int Get_id();
 int Get_type();
 bool Get_print_before_player();
 bool Is_obstacle();
 bool Is_light();
 bool Is_animation();
 bool Is_done();
 bool Is_trigger();
 bool Is_passer();
 Buff Get_Buff();
 int Get_power();
 int Get_number_of_frames();
 int Get_frame_change_delay();
 void Load();
 void Print_image(int x,int y,Texture *_screen,int frame);
};

#endif //MAP_TEXTURE_H
