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
 //itoa(RESOLUTION,aux);
 //strcat(path,aux);
 strcat(path,".bmp");
 background=SDL_LoadBMP(path);
}

void Big_Background::Load_Logo()
{
 char path[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};
 for(int i=0;i<=number_of_frames;i++)
     {
      strcpy(path,"images/launcher/logo animation/logo");
      itoa(i,aux);
      strcat(path,aux);
      strcat(path,".bmp");
      //image[i]=SDL_LoadBMP(path);
      image[i]=make_it_transparent(path);
     }
 adventure_image=make_it_transparent("images/launcher/logo animation/adventure name.bmp");
}

void Big_Background::Update_size()
{
 char path[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"images/launcher/launcher background");
 //itoa(RESOLUTION,aux);
 //strcat(path,aux);
 strcat(path,".bmp");
 background=SDL_LoadBMP(path);
}

void Big_Background::Update_image_frame()
{
 count++;
 if(count>=50)
    {
     actual_frame++;
     if(actual_frame==number_of_frames+1)
        count=0,actual_frame=0;
    }
}

void Big_Background::Print_image(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y,background,_screen);
 apply_surface(x+(RESOLUTION_X-adventure_image->w)/2,y+(RESOLUTION_Y-image[actual_frame]->h)/8+image[actual_frame]->h,adventure_image,_screen);
 if(image[actual_frame]==NULL)
    return;
 apply_surface(x+(RESOLUTION_X-image[actual_frame]->w)/2,y+(RESOLUTION_Y-image[actual_frame]->h)/8,image[actual_frame],_screen);
}

void Big_Background::Set_actual_frame(int _actual_frame)
{
 actual_frame=_actual_frame;
}
