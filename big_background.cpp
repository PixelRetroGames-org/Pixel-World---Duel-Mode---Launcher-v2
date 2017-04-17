#include "big_background.h"
#include "settings.h"

Big_Background::Big_Background()
{
 char path[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"images/launcher/launcher background");
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
      image[i]=make_it_transparent(path);
     }
 adventure_image=make_it_transparent("images/launcher/logo animation/adventure name.bmp");
 fire=make_it_transparent("images/launcher/torch_animation.bmp");
}

void Big_Background::Update_size()
{
 char path[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"images/launcher/launcher background");
 strcat(path,".bmp");
 SDL_FreeSurface(background);
 background=SDL_LoadBMP(path);
}

void Big_Background::Update_image_frame()
{
 count++;
 if(count>=max_count)
    {
     current_frame++;
     if(current_frame==number_of_frames+1)
        count=0,current_frame=0;
    }
}

void Big_Background::Update_fire_frame()
{
 fire_current_frame=(fire_current_frame+1)%fire_number_of_frames;
}

void Big_Background::Print_image(int x,int y,SDL_Surface* _screen)
{
 apply_surface(x,y,background,_screen);
 apply_surface(x+(RESOLUTION_X-adventure_image->w)/2,y+(RESOLUTION_Y-image[current_frame]->h)/8+image[current_frame]->h,adventure_image,_screen);
 apply_surface(fire->w/(fire_number_of_frames+1)*fire_current_frame,0,x+(adventure_image->w-(fire->w/(fire_number_of_frames+1)))/4,y+(RESOLUTION_Y-fire->h)/2+image[current_frame]->h,fire->w/(fire_number_of_frames+1),fire->h,fire,_screen);
 apply_surface(fire->w/(fire_number_of_frames+1)*fire_current_frame,0,RESOLUTION_X-(fire->w/(fire_number_of_frames+1))-(x+(adventure_image->w-(fire->w/(fire_number_of_frames+1))))/4,y+(RESOLUTION_Y-fire->h)/2+image[current_frame]->h,fire->w/(fire_number_of_frames+1),fire->h,fire,_screen);
 if(image[current_frame]==NULL)
    return;
 apply_surface(x+(RESOLUTION_X-image[current_frame]->w)/2,y+(RESOLUTION_Y-image[current_frame]->h)/8,image[current_frame],_screen);
}

void Big_Background::Set_current_frame(int _current_frame)
{
 current_frame=_current_frame;
}
