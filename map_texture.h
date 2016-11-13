#ifndef MAP_TEXTURE_H
#define MAP_TEXTURE_H

#include "library.h"
#include "load_images.h"
#include "buff.h"

#ifndef TEXT_LENGHT_MAX_DEFINED
const int TEXT_LENGTH_MAX=101;
#define TEXT_LENGHT_MAX_DEFINED
#endif // TEXT_LENGHT_MAX_DEFINED

class Texture
{
 private:
 int id=0;
 char name[TEXT_LENGTH_MAX]={NULL};
 SDL_Surface *image=NULL;
 SDL_Rect image_position;
 int type,damage;
 Buff buff;
 int number_of_frames=1;
 int power=0;
 bool print_before_player=false,obstacle=false,light=false,animation=false;

 public:
 Texture();
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
 Buff Get_Buff();
 int Get_power();
 int Get_number_of_frames();
 void Load();
 void Print_image(int x,int y,SDL_Surface *_screen,int frame);
};

#endif //MAP_TEXTURE_H
