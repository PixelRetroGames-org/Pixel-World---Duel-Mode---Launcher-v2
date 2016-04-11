#include "big_background.h"

Big_Background::Big_Background()
{
 image=SDL_LoadBMP("images/launcher/animation test.bmp");
 image_position.x=image_position.y=0;
 image_position.h=1366;
 image_position.w=768;
}

void Big_Background::Update_image_frame()
{
 actual_frame++;
 actual_frame%=number_of_frames;
 Update_image_position();
}

void Big_Background::Update_image_position()
{
 image_position.x=actual_frame*1366;
 image_position.y=0;
 image_position.h=1366;
 image_position.w=768;
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
