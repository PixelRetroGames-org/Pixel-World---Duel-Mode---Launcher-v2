#include "menu.h"
#include "settings.h"
#include "script_interpreter.h"
#include "shop.h"
#include<cstdio>
SDL_Surface *screen;

Script_interpreter script_interpreter;

Shop_Screen shop_screen;

Mix_Music *background_music=NULL;

int main( int argc, char* args[] )
{
 SDL_Init(SDL_INIT_EVERYTHING);
 TTF_Init();
 Mix_Init(MIX_INIT_MP3);
 SDL_WM_SetCaption("Pixel World",NULL);
 Set_icon("images/icon.bmp");
 Load_Settings();
 screen=SDL_SetVideoMode(RESOLUTION_X,RESOLUTION_Y,32,DISPLAY_MODE);
 Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
 SDL_Flip(screen);
 Menu main_menu,gamemode_menu,shop_menu;
 Load_all_images();
 main_menu.Load("menu/main_menu.pwm");
 gamemode_menu.Load("menu/gamemode.pwm");
 shop_menu.Load("menu/shop_menu.pwm");
 LAUNCHER_BBACKGROUND.Update_size();
 LAUNCHER_BBACKGROUND.Load_Logo();
 background_music=Mix_LoadMUS("audio/Hallowed Be Thy Name.wav");
 Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
 Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
 Mix_PlayMusic(background_music,-1);
 int option=-1;
 while(option!=-2)
       {
        option=main_menu.Start(screen);
        switch(option)
               {
                case -1:{break;};
                case 0:{
                        int _option=-1;
                        while(_option!=-2)
                              {
                               _option=gamemode_menu.Start(screen);
                               option=-2;
                               switch(_option)
                                      {
                                       case -1:{break;};
                                       case 0:{
                                               //Story Mode
                                               int __option;
                                               while(__option!=-2)
                                                     {
                                                      __option=shop_menu.Start(screen);
                                                      switch(__option)
                                                             {
                                                              case -1:{break;};
                                                              case 0:{//Launch Story Mode
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
                                                                      shop_screen.Start(screen,"Timy");
                                                                      break;
                                                                     };
                                                              case 2:{
                                                                      _option=-1;
                                                                      __option=-2;
                                                                      break;
                                                                     };
                                                             }
                                                     }
                                               break;
                                              };
                                       case 1:{//Launch Duel Mode
                                               int __option;
                                               while(__option!=-2)
                                                     {
                                                      __option=shop_menu.Start(screen);
                                                      switch(__option)
                                                             {
                                                              case -1:{break;};
                                                              case 0:{//Launch Duel Mode
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
                                                                      shop_screen.Start(screen,"Player1");
                                                                      shop_screen.Start(screen,"Player2");
                                                                      break;
                                                                     };
                                                              case 2:{
                                                                      _option=-1;
                                                                      __option=-2;
                                                                      break;
                                                                     };
                                                             }
                                                     }
                                               break;
                                              };
                                       case 2://Back
                                              option=-1;
                                              _option=-2;
                                              break;
                                      }
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
