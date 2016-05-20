#ifndef BIG_BACKGROUND_H
#define BIG_BACKGROUND_H

#include "library.h"

class Big_Background
{
 private:
 SDL_Surface *image[101],*background=NULL,*adventure_image=NULL;
 int number_of_frames=42,count=0;
 int actual_frame=0;

 public:
 Big_Background();
 void Load_Logo();
 void Update_size();
 void Update_image_frame();
 void Update_image_position();
 void Print_image(int x,int y,SDL_Surface *_screen);
 void Set_actual_frame(int _actual_frame);
};

#endif //BIG_BACKGROUND_H
