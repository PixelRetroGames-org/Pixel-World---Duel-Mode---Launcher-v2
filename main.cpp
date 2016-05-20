#include "menu.h"
#include "settings.h"
#include "script_interpreter.h"
#include<cstdio>
SDL_Surface *screen;

Script_interpreter script_interpreter;

int main( int argc, char* args[] )
{
 SDL_Init(SDL_INIT_EVERYTHING);
 TTF_Init();
 SDL_WM_SetCaption("Pixel World",NULL);
 Set_icon("images/icon.bmp");
 Load_Settings();
 screen=SDL_SetVideoMode(RESOLUTION_X,RESOLUTION_Y,32,DISPLAY_MODE);
 Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
 SDL_Flip(screen);
 Menu main_menu,gamemode_menu;
 Load_menu_images();
 Load_settings_images();
 Load_script_images();
 main_menu.Load("menu/main_menu.pwm");
 gamemode_menu.Load("menu/gamemode.pwm");
 LAUNCHER_BBACKGROUND.Update_size();
 LAUNCHER_BBACKGROUND.Load_Logo();
 int option=-1;
 while(option!=-2)
       {
        option=main_menu.Start(screen);
        switch(option)
               {
                case -1:{break;};
                case 0:{
                        int _option=gamemode_menu.Start(screen);
                        option=-2;
                        switch(_option)
                               {
                                case 0://Launch Story Mode
                                       break;
                                case 1://Launch Duel Mode
                                       break;
                                case 2://Back
                                       option=-1;
                                       break;
                               }
                        /*Run the game*/break;};
                case 1:{Graphic_Settings(screen);
                        Save_Settings();
                        LAUNCHER_BBACKGROUND.Update_size();
                        main_menu.Load("menu/main_menu.pwm");
                        gamemode_menu.Load("menu/gamemode.pwm");
                        break;
                       };
                case 2:{
                        script_interpreter.Start(screen,"Credits");
                        break;
                       };
                case 3:{
                        option=-2;
                        break;
                       };
               }
       }
 return 0;
}
