#include "darkness.h"

const int DARKNESS_UNIT=5;

Darkness::Darkness()
{
 strcpy(image_name,"all_black");
}

void Darkness::Clear()
{
 if(image!=NULL)
    {
     SDL_FreeSurface(image);
     image=NULL;
    }
}

void Darkness::Set_image_name(char *_image_name)
{
 strcpy(image_name,_image_name);
}

void Darkness::Load_image()
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"images/darkness/");
 strcat(path,image_name);
 strcat(path,".bmp");
 image=load_image(path);
}

void Darkness::Update_frame()
{
 current_frame++;
 current_frame%=number_of_frames;
}

void Darkness::Update_image()
{
 if(image==NULL)
    return;
 SDL_SetAlpha(image,SDL_SRCALPHA,alpha);
}

void Darkness::Enshroud(SDL_Rect area,SDL_Surface *_screen)
{
// apply_surface((int)area.x,(int)area.y,(int)area.w,(int)area.h,image,_screen);
 apply_surface(current_frame*DARKNESS_FRAME_X,0,area.x,area.y,area.w,area.h,image,_screen);
}

void Darkness::Set_aplha(int _alpha)
{
 alpha=_alpha;
 Update_image();
}

void Darkness::Set_number_of_frames(int _number_of_frames)
{
 number_of_frames=_number_of_frames;
 current_frame=0;
}

void Darkness::Increase()
{
 alpha+=DARKNESS_UNIT;
 if(alpha>SDL_ALPHA_OPAQUE)
    alpha=SDL_ALPHA_OPAQUE;
 Update_image();
}

void Darkness::Decrease()
{
 alpha-=DARKNESS_UNIT;
 if(alpha<SDL_ALPHA_TRANSPARENT)
    alpha=SDL_ALPHA_TRANSPARENT;
 Update_image();
}
