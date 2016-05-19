#include "big_background.h"
#include "settings.h"

#ifndef TEXT_LENGHT_MAX_DEFINED
const int TEXT_LENGHT_MAX=101;
#define TEXT_LENGHT_MAX_DEFINED
#endif // TEXT_LENGHT_MAX_DEFINED

Big_Background::Big_Background()
{
 char path[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"images/launcher/launcher background");
 itoa(RESOLUTION,aux);
 strcat(path,aux);
 strcat(path,".bmp");
 image=SDL_LoadBMP(path);
 image_position.x=image_position.y=0;
 image_position.h=RESOLUTION_X;
 image_position.w=RESOLUTION_Y;
}

void Big_Background::Update_size()
{
 char path[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"images/launcher/launcher background");
 itoa(RESOLUTION,aux);
 strcat(path,aux);
 strcat(path,".bmp");
 image=SDL_LoadBMP(path);
 image_position.x=image_position.y=0;
 image_position.h=RESOLUTION_X;
 image_position.w=RESOLUTION_Y;
}

void Big_Background::Update_image_frame()
{
 actual_frame++;
 actual_frame%=number_of_frames;
 Update_image_position();
}

void Big_Background::Update_image_position()
{
 image_position.x=actual_frame*RESOLUTION_X;
 image_position.y=0;
 image_position.h=RESOLUTION_X;
 image_position.w=RESOLUTION_Y;
}

void Big_Background::Print_image(int x,int y,SDL_Surface *_screen)
{
 if(image==NULL)
    return;
 apply_surface(image_position.x,image_position.y,x,y,image_position.h,image_position.w,image,_screen);
}

void Big_Background::Set_actual_frame(int _actual_frame)
{
 actual_frame=_actual_frame;
}
