#ifndef BIG_BACKGROUND_H
#define BIG_BACKGROUND_H

#include "library.h"

class Big_Background
{
 private:
 SDL_Surface *image=NULL;
 SDL_Rect image_position;
 int number_of_frames=1;
 int actual_frame=0;

 public:
 Big_Background();
 void Update_image_frame();
 void Update_image_position();
 void Print_image(int x,int y,SDL_Surface *_screen);
 void Set_actual_frame(int _actual_frame);
};

#endif //BIG_BACKGROUND_H
