#ifndef BIG_BACKGROUND_H
#define BIG_BACKGROUND_H

#include "library.h"

class Big_Background
{
 private:
 SDL_Surface* image[101],*background=NULL,*adventure_image=NULL,*fire=NULL;
 int number_of_frames=42,count=0,max_count=50;
 int fire_number_of_frames=7,fire_current_frame=0;
 int current_frame=0;

 public:
 Big_Background();
 void Set_max_count(int _max_count);
 void Load_Logo();
 void Update_size();
 void Update_image_frame();
 void Update_fire_frame();
 void Update_image_position();
 void Print_image(int x,int y,SDL_Surface* _screen);
 void Set_current_frame(int _current_frame);
};

#endif //BIG_BACKGROUND_H
