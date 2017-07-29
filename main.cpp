#include "menu.h"
#include "settings.h"
#include "script_interpreter.h"
#include "shop.h"
#include "level.h"
#include "load_audio_effects.h"
#include "puzzle.h"
#include "controller.h"

#include <cstdio>
#include <ctime>
#include <cstdlib>

Texture *screen;

Script_interpreter script_interpreter;

Shop_Screen shop_screen;

Level level;

Mix_Music *launcher_background_music=NULL;

int main( int argc, char *args[] )
{
 ///Random
 srand((unsigned int)(time(NULL)));
 ///Random
 if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    {
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"SDL_Init() failed : %s ",SDL_GetError());
     fclose(log_file);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"SDL_Init() failed : ");
     strcat(message,SDL_GetError());
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL failure",message,NULL);
     exit(-1);
    }
 if(TTF_Init()==-1)
    {
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"TTF_Init() failed : %s ",TTF_GetError());
     fclose(log_file);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"TTF_Init() failed : ");
     strcat(message,SDL_GetError());
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL_TTF module failure",message,NULL);
     exit(-2);
    }
 MUSIC_MODULE_INIT=true;
 if(Mix_Init(MIX_INIT_MP3)&(MIX_INIT_MP3)!=MIX_INIT_MP3)
    {
     MUSIC_MODULE_INIT=false;
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"Mix_Init() failed : %s ",Mix_GetError());
     fclose(log_file);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"Mix_Init() failed : ");
     strcat(message,SDL_GetError());
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL_mixer module failure",message,NULL);
    }
 Init_Controllers();
 Load_Settings();
 Open_Window_and_Renderer(RESOLUTION_W,RESOLUTION_H,DISPLAY_MODE);
 last_frame=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
 screen=new Texture;
 screen->w=RESOLUTION_W;
 screen->h=RESOLUTION_H;
 screen->image=NULL;
 SCREEN=screen;
 if(screen==NULL)
    {
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"Open_Window_and_Renderer failed : %s ",SDL_GetError());
     fclose(log_file);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"Open_Window_and_Renderer failed : ");
     strcat(message,SDL_GetError());
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL video module failure",message,NULL);
     exit(-1);
    }
 MUSIC_MODULE_INIT=true;
 if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1)
    {
     MUSIC_MODULE_INIT=false;
     FILE *log_file=fopen("err/logs.txt","w");
     fprintf(log_file,"Mix_OpenAudio failed : %s ",Mix_GetError());
     fclose(log_file);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"Mix_OpenAudio() failed : ");
     strcat(message,Mix_GetError());
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL_mixer module failure",message,NULL);
    }
 Flip_Buffers(screen);
 LAUNCHER_BBACKGROUND.Load();
 LAUNCHER_BBACKGROUND.Load_Logo();
 static_screen=screen;
 #define SPLASH_SCREEN
 #ifdef SPLASH_SCREEN
 splash_screen_mutex=SDL_CreateMutex();
 SDL_Thread *splash_screen=NULL;
 splash_screen=SDL_CreateThread(Splash_Screen,"Splash Screen",NULL);
 #endif // SPLASH_SCREEN
 Menu main_menu,gamemode_menu,story_menu,duel_menu;
 Load_all_images();
 Load_Duel_Mode_effects();
 Load_Click_effects();
 main_menu.Load("menu/main_menu.pwm");
 gamemode_menu.Load("menu/gamemode.pwm");
 story_menu.Load("menu/story_menu.pwm");
 duel_menu.Load("menu/duel_menu.pwm");
 launcher_background_music=Mix_LoadMUS("audio/Hallowed Be Thy Name.mp3");
 if(MUSIC_MODULE_INIT)
    {
     Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
     Mix_Volume(2,MIX_MAX_VOLUME*VOLUME/100);
     Mix_Volume(3,MIX_MAX_VOLUME*(VOLUME/10)/100);
     Mix_Volume(4,MIX_MAX_VOLUME*(VOLUME/10)/100);
     Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
     //Mix_PlayMusic(launcher_background_music,-1);
     Mix_FadeInMusic(launcher_background_music,-1,8000);
    }
 Load_Journal();
 #ifdef SPLASH_SCREEN
 SDL_LockMutex(splash_screen_mutex);
 splash_screen_quit=true;
 SDL_UnlockMutex(splash_screen_mutex);
 int thread_return_value=0;
 SDL_WaitThread(splash_screen,&thread_return_value);
 Flip_Buffers(screen);
 SDL_DestroyMutex(splash_screen_mutex);
 #endif // SPLASH_SCREEN
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
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_HaltMusic();
                                                                      Launch_Story_Mode(&level,screen);
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_PlayMusic(launcher_background_music,-1);
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
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
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_HaltMusic();
                                                                      Launch_Duel_Mode(&level,screen);
                                                                      if(MUSIC_MODULE_INIT)
                                                                         Mix_PlayMusic(launcher_background_music,-1);
                                                                      _option=-2;
                                                                      break;
                                                                     };
                                                              case 1:{
                                                                      if(shop_screen.Start(screen,"Shop","Player1",1)==-1)
                                                                         return 0;
                                                                      if(shop_screen.Start(screen,"Shop","Player2",2)==-1)
                                                                         return 0;
                                                                      break;
                                                                     };
                                                              case 2:{
                                                                      Controls_screen(screen);
                                                                      break;
                                                                     };
                                                              case 3:{
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
                        LAUNCHER_BBACKGROUND.Load();
                        Clear_Journal();
                        Load_Journal();
                        main_menu.Load("menu/main_menu.pwm");
                        gamemode_menu.Load("menu/gamemode.pwm");
                        story_menu.Load("menu/story_menu.pwm");
                        duel_menu.Load("menu/duel_menu.pwm");
                        if(MUSIC_MODULE_INIT)
                           {
                            Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
                            Mix_Volume(2,MIX_MAX_VOLUME*VOLUME/100);
                            Mix_Volume(3,MIX_MAX_VOLUME*(VOLUME/10)/100);
                            Mix_Volume(4,MIX_MAX_VOLUME*(VOLUME/10)/100);
                            Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
                           }
                        break;
                       };
                case 2:{
                        script_interpreter.Start(screen,"Credits");
                        //script_interpreter.Start(screen,"Test");
                        break;
                       };
                case 3:{
                        option=-2;
                        break;
                       };
               }
       }
 Clear_all_images();
 Clear_Duel_Mode_effects();
 Clear_Journal();
 Destroy_Controllers();
 return 0;
}
