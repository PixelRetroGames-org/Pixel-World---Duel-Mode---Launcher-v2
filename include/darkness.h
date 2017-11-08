#ifndef DARKNESS_H
#define DARKNESS_H

#include "library.h"
#include "texture.h"

#ifndef NUMBER_OF_DARKNESS_FRAMES_DEFINED
const int NUMBER_OF_DARKNESS_FRAMES=10;
const int DARKNESS_FRAME_X=860;
#define NUMBER_OF_DARKNESS_FRAMES_DEFINED
#endif // NUMBER_OF_DARKNESS_FRAMES_DEFINED

class Darkness
{
 private:
 int alpha=0;
 Texture *image=NULL;
 int number_of_frames=0,current_frame=0;
 char image_name[TEXT_LENGTH_MAX]={NULL};
 int frameW=DARKNESS_FRAME_X,frameH=0;

 public:
 Darkness();
 void Clear();
 void Set_image_name(char *_image_name);
 void Load_image();
 void Set_frameW(int _frameW);
 void Set_frameH(int _frameH);
 void Set_current_frame(int _current_frame);

 int Get_alpha();
 int Get_frame();

 void Update_frame();
 void Update_image();
 void Enshroud(SDL_Rect area,Texture *_screen=NULL);
 void Set_aplha(int _aplha);
 void Set_number_of_frames(int _number_of_frames);
 void Increase();
 void Decrease();
};

#endif // DARKNESS_H
