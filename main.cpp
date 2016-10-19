#include "menu.h"
#include "settings.h"
#include "script_interpreter.h"
#include "shop.h"
#include "level.h"
#include "load_audio_effects.h"
#include<cstdio>
SDL_Surface *screen;

Script_interpreter script_interpreter;

Shop_Screen shop_screen;

Level level;

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
 Menu main_menu,gamemode_menu,story_menu,duel_menu;
 Load_all_images();
 Load_Duel_Mode_effects();
 main_menu.Load("menu/main_menu.pwm");
 gamemode_menu.Load("menu/gamemode.pwm");
 story_menu.Load("menu/story_menu.pwm");
 duel_menu.Load("menu/duel_menu.pwm");
 LAUNCHER_BBACKGROUND.Update_size();
 LAUNCHER_BBACKGROUND.Load_Logo();
 background_music=Mix_LoadMUS("audio/Hallowed Be Thy Name.wav");
 Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
 Mix_Volume(2,MIX_MAX_VOLUME*VOLUME/100);
 Mix_Volume(3,MIX_MAX_VOLUME*(VOLUME/24)/100);
 Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
 Mix_PlayMusic(background_music,-1);
 int option=-1;
 while(option!=-2)
       {
        option=main_menu.Start(screen);
        switch(option)
               {
                case -2:{return 0;};
                case -1:{break;};
                case 0:{
                        int _option=-1;
                        while(_option!=-2)
                              {
                               _option=gamemode_menu.Start(screen);
                               option=-2;
                               switch(_option)
                                      {
                                       case -2:{return 0;};
                                       case -1:{break;};
                                       case 0:{
                                               //Story Mode
                                               int __option=-1;
                                               while(__option!=-2)
                                                     {
                                                      __option=story_menu.Start(screen);
                                                      switch(__option)
                                                             {
                                                              case -2:{return 0;};
                                                              case -1:{break;};
                                                              case 0:{//Launch Story Mode
                                                                      Mix_HaltMusic();
                                                                      level.Set_screen(screen);
                                                                      level.Setup("Test6");
                                                                      level.Start(screen);
                                                                      Mix_PlayMusic(background_music,-1);
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
                                                                      if(shop_screen.Start(screen,"Timy")==-1)
                                                                         return 0;
                                                                      shop_screen.Reset();
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
                                               int __option=-1;
                                               while(__option!=-2)
                                                     {
                                                      __option=duel_menu.Start(screen);
                                                      switch(__option)
                                                             {
                                                              case -2:{return 0;};
                                                              case -1:{break;};
                                                              case 0:{//Launch Duel Mode
                                                                      Mix_HaltMusic();
                                                                      level.Set_screen(screen);
                                                                      level.Setup("Duel Mode");
                                                                      level.Start(screen);
                                                                      Mix_PlayMusic(background_music,-1);
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
                                                                      if(shop_screen.Start(screen,"Player1")==-1)
                                                                         return 0;
                                                                      shop_screen.Reset();
                                                                      if(shop_screen.Start(screen,"Player2")==-1)
                                                                         return 0;
                                                                      shop_screen.Reset();
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
                        story_menu.Load("menu/story_menu.pwm");
                        duel_menu.Load("menu/duel_menu.pwm");
                        Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
                        Mix_Volume(2,MIX_MAX_VOLUME*VOLUME/100);
                        Mix_Volume(3,MIX_MAX_VOLUME*(VOLUME/24)/100);
                        Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
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
