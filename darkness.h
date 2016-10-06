#ifndef DARKNESS_H
#define DARKNESS_H

#include "library.h"

#ifndef TEXT_LENGHT_MAX_DEFINED
const int TEXT_LENGHT_MAX=101;
#define TEXT_LENGHT_MAX_DEFINED
#endif // TEXT_LENGHT_MAX_DEFINED

#ifndef NUMBER_OF_DARKNESS_FRAMES_DEFINED
const int NUMBER_OF_DARKNESS_FRAMES=10;
const int DARKNESS_FRAME_X=860;
#define NUMBER_OF_DARKNESS_FRAMES_DEFINED
#endif // NUMBER_OF_DARKNESS_FRAMES_DEFINED

class Darkness
{
 private:
 int alpha=0;
 SDL_Surface *image=NULL;
 int number_of_frames=0,current_frame=0;
 char image_name[TEXT_LENGHT_MAX]={NULL};

 public:
 Darkness();
 void Clear();
 void Set_image_name(char *_image_name);
 void Load_image();

 void Update_frame();
 void Update_image();
 void Enshroud(SDL_Rect area,SDL_Surface *_screen);
 void Set_aplha(int _aplha);
 void Set_number_of_frames(int _number_of_frames);
 void Increase();
 void Decrease();
};

#endif // DARKNESS_H
