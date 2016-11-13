#include "big_background.h"
#include "settings.h"

#ifndef TEXT_LENGTH_MAX_DEFINED
const int TEXT_LENGTH_MAX=101;
#define TEXT_LENGTH_MAX_DEFINED
#endif // TEXT_LENGTH_MAX_DEFINED

Big_Background::Big_Background()
{
 char path[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"images/launcher/launcher background");
 //itoa(RESOLUTION,aux);
 //strcat(path,aux);
 strcat(path,".bmp");
 background=SDL_LoadBMP(path);
}

void Big_Background::Set_max_count(int _max_count)
{
 max_count=_max_count;
}

void Big_Background::Load_Logo()
{
 char path[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
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
 fire=make_it_transparent("images/launcher/torch_animation.bmp");
}

void Big_Background::Update_size()
{
 char path[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"images/launcher/launcher background");
 //itoa(RESOLUTION,aux);
 //strcat(path,aux);
 strcat(path,".bmp");
 SDL_FreeSurface(background);
 background=SDL_LoadBMP(path);
}

void Big_Background::Update_image_frame()
{
 count++;
 if(count>=max_count)
    {
     actual_frame++;
     if(actual_frame==number_of_frames+1)
        count=0,actual_frame=0;
    }
}

void Big_Background::Update_fire_frame()
{
 fire_actual_frame=(fire_actual_frame+1)%fire_number_of_frames;
}

void Big_Background::Print_image(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y,background,_screen);
 apply_surface(x+(RESOLUTION_X-adventure_image->w)/2,y+(RESOLUTION_Y-image[actual_frame]->h)/8+image[actual_frame]->h,adventure_image,_screen);
 apply_surface(fire->w/(fire_number_of_frames+1)*fire_actual_frame,0,x+(adventure_image->w-(fire->w/(fire_number_of_frames+1)))/4,y+(RESOLUTION_Y-fire->h)/2+image[actual_frame]->h,fire->w/(fire_number_of_frames+1),fire->h,fire,_screen);
 apply_surface(fire->w/(fire_number_of_frames+1)*fire_actual_frame,0,RESOLUTION_X-(fire->w/(fire_number_of_frames+1))-(x+(adventure_image->w-(fire->w/(fire_number_of_frames+1))))/4,y+(RESOLUTION_Y-fire->h)/2+image[actual_frame]->h,fire->w/(fire_number_of_frames+1),fire->h,fire,_screen);
 if(image[actual_frame]==NULL)
    return;
 apply_surface(x+(RESOLUTION_X-image[actual_frame]->w)/2,y+(RESOLUTION_Y-image[actual_frame]->h)/8,image[actual_frame],_screen);
}

void Big_Background::Set_actual_frame(int _actual_frame)
{
 actual_frame=_actual_frame;
}
