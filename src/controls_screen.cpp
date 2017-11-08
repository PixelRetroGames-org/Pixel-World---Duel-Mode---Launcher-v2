#include "controls_screen.h"

const int CONTROLLER_DELAY=200;

void Controls_screen(Texture *_screen)
{
 const int number_of_pages=2;
 int page=0;
 SDL_Event event;
 SDL_PollEvent(&event);
 Timer controller_timer;
 controller_timer.start();
 bool quit=false,pressed_controller_button=false;
 if((controller[1].Pressed_Any_Button() || controller[2].Pressed_Any_Button()) && (controller_timer.get_ticks()>CONTROLLER_DELAY || !pressed_controller_button))
    {
     pressed_controller_button=true;
     page=(page+1)%number_of_pages;
     controller_timer.start();
    }
 while(!quit)
       {
        Apply_Texture(0,0,CONTROLS_background_image,_screen);
        switch(page)
               {
                case 0: Apply_Stretched_Texture(0,0,_screen->w,_screen->h,CONTROLS_keyboard_image,_screen); break;
                case 1: Apply_Stretched_Texture(0,0,_screen->w,_screen->h,CONTROLS_controller_image,_screen); break;
               }
        Flip_Buffers(_screen);
        if(event.type==SDL_QUIT || ((event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE) || (controller[1].Pressed_Guide_button() || controller[2].Pressed_Guide_button())))
           quit=true;
        if((controller[1].Pressed_Any_Button() || controller[2].Pressed_Any_Button()) && (controller_timer.get_ticks()>CONTROLLER_DELAY || !pressed_controller_button))
           {
            pressed_controller_button=true;
            page=(page+1)%number_of_pages;
            controller_timer.start();
           }
        SDL_Delay(40);
        SDL_PollEvent(&event);
        Update_Controllers_Events();
       }
}
