#include "menu.h"
#include<cstdio>

SDL_Surface *screen;


int main( int argc, char* args[] )
{
 SDL_Init(SDL_INIT_EVERYTHING);
 TTF_Init();
 #ifndef DEBUG
 screen=SDL_SetVideoMode(0,0,32,SDL_FULLSCREEN);
 #endif // DEBUG
 #ifdef DEBUG
 screen=SDL_SetVideoMode(0,0,32,SDL_SWSURFACE);
 #endif // DEBUG
 SDL_Flip(screen);
 Menu X;
 Load_Menu_images();
 X.Load("menu/main_menu.pwm");
 int _option=X.Start(screen);
 switch(_option)
        {
         case -1:{break;};
         case 1:{/*Run the game*/break;};
         case 2:{
                 //Credits
                 break;
                };
         case 3:{break;};
        }
 return 0;
}
