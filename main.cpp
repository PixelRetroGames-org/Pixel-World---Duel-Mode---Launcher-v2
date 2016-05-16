#include "menu.h"
#include "settings.h"
#include<cstdio>

SDL_Surface *screen;


int main( int argc, char* args[] )
{
 SDL_Init(SDL_INIT_EVERYTHING);
 TTF_Init();
 screen=SDL_SetVideoMode(RESOLUTION_X,RESOLUTION_Y,32,DISPLAY_MODE);
 Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
 SDL_Flip(screen);
 Menu menu;
 Load_menu_images();
 Load_settings_images();
 Load_Settings();
 menu.Load("menu/main_menu.pwm");
 int option=menu.Start(screen);
 while(option!=-2)
       {
        switch(option)
               {
                case -1:{break;};
                case 0:{/*Run the game*/break;};
                case 1:{
                        //Credits
                        break;
                       };
                case 2:{Graphic_Settings(screen);
                        Save_Settings();
                        menu.Load("menu/main_menu.pwm");
                        break;
                       };
               }
        option=menu.Start(screen);
       }
 return 0;
}
