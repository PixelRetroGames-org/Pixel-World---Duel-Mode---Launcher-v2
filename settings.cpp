#include "settings.h"

const int NUMBER_OF_AVAILABLE_RESOLUTIONS=4;
const int available_RESOLUTION_W[NUMBER_OF_AVAILABLE_RESOLUTIONS]={1366,1440,1600,1920};
const int available_RESOLUTION_H[NUMBER_OF_AVAILABLE_RESOLUTIONS]={ 768, 900, 900, 1080};
const int NUMBER_OF_AVAILABLE_VOLUMES=5,available_volumes[5]={0,25,50,75,100};
char *available_volumes_names[5]={"Mute","25%","50%","75%","100%"};

const SDL_Color settings_color={255,255,255};

int RESOLUTION_W=1366,RESOLUTION_H=768,RESOLUTION=2;
int DISPLAY_MODE=0;
int VOLUME,VOL=2;
bool POWER_SAVER=false,AUTO_ATTACK=false;

void Load_Settings()
{
 FILE *where=fopen("settings/settings.set","r");
 if(where==NULL)
    {
     Save_Settings();
     return;
    }
 int dm=0;
 fscanf(where,"%d %d %d %d %d ",&RESOLUTION,&dm,&VOL,&POWER_SAVER,&AUTO_ATTACK);
 RESOLUTION_W=available_RESOLUTION_W[RESOLUTION];
 RESOLUTION_H=available_RESOLUTION_H[RESOLUTION];
 DISPLAY_MODE=dm==0?SDL_WINDOW_FULLSCREEN:0;
 VOLUME=available_volumes[VOL];
 fclose(where);
}

void Save_Settings()
{
 FILE *where=fopen("settings/settings.set","w");
 int dm=0;
 fprintf(where,"%d %d %d %d %d",RESOLUTION,DISPLAY_MODE==SDL_WINDOW_FULLSCREEN?0:1,VOL,POWER_SAVER,AUTO_ATTACK);
 fclose(where);
}

void Set_RESOLUTION_W(int X)
{
 RESOLUTION_W=X;
}

void Set_RESOLUTION_H(int Y)
{
 RESOLUTION_H=Y;
}

void Set_DISPLAY_MODE(int MODE)
{
 DISPLAY_MODE=(Uint32)MODE;
}

void Print_Settings_Background(Texture *_screen)
{
 Apply_Texture(0,0,SETTINGS_background,_screen);
 Apply_Texture((RESOLUTION_W-SETTINGS_name->w)/2,PIXELS_PER_INGAME_UNIT,SETTINGS_name,_screen);
}

int selector_position,click_position;
const int number_of_options=6;
const int CONTROLLER_DELAY=100;

void Graphic_Settings(Texture *_screen)
{
 selector_position=click_position=-1;
 bool quit=false;
 SDL_Event event;
 SDL_Delay(100);
 while(SDL_PollEvent(&event));
 SDL_PumpEvents();
 Timer controller_timer;
 controller_timer.start();
 while(!quit)
       {
        Print_Settings_Background(_screen);
        Graphic_Change_Resolution((RESOLUTION_W-SETTINGS_option_background->w)/2,120,_screen,&event);
        Graphic_Change_Display_Mode((RESOLUTION_W-SETTINGS_option_background->w)/2,160,_screen,&event);
        Graphic_Change_Volume((RESOLUTION_W-SETTINGS_option_background->w)/2,200,_screen,&event);
        //Graphic_Power_Saver((RESOLUTION_W-SETTINGS_option_background->w)/2,240,_screen,&event);
        Graphic_Auto_Attack((RESOLUTION_W-SETTINGS_option_background->w)/2,240,_screen,&event);
        Graphic_Reset_Saves((RESOLUTION_W-SETTINGS_option_background->w)/2,280,_screen,&event);
        quit=Graphic_Back((RESOLUTION_W-SETTINGS_option_background->w)/2,400,_screen,&event);
        Flip_Buffers(_screen);
        SDL_PollEvent(&event);
        Update_Controllers_Events();
        SDL_Delay(20);
        if(event.type==SDL_MOUSEMOTION)
           selector_position=-1;
        if((controller[1].Pressed_Up() || controller[2].Pressed_Up()) && selector_position>0 && controller_timer.get_ticks()>CONTROLLER_DELAY)
           {
            selector_position--;
            controller_timer.start();
           }
        if((controller[1].Pressed_Down() || controller[2].Pressed_Down()) && selector_position<number_of_options-1 && controller_timer.get_ticks()>CONTROLLER_DELAY)
           {
            selector_position++;
            controller_timer.start();
           }
        if(controller_timer.get_ticks()>CONTROLLER_DELAY*2 && (controller[1].Pressed_A_button() || controller[2].Pressed_A_button() ||
                                                             controller[1].Pressed_Start_button() || controller[2].Pressed_Start_button()))
           {
            click_position=selector_position;
            controller_timer.start();
           }
        if(event.type==SDL_QUIT || ((event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE) || (controller[1].Pressed_Guide_button() || controller[2].Pressed_Guide_button())))
           quit=true;
       }
}

void Graphic_Settings(Texture *_screen,SDL_Event *event)
{
 selector_position=click_position=-1;
 bool quit=false;
 SDL_Delay(100);
 while(SDL_PollEvent(event));
 SDL_PumpEvents();
 Timer controller_timer;
 controller_timer.start();
 while(!quit)
       {
        Print_Settings_Background(_screen);
        Graphic_Change_Resolution((RESOLUTION_W-SETTINGS_option_background->w)/2,120,_screen,event);
        Graphic_Change_Display_Mode((RESOLUTION_W-SETTINGS_option_background->w)/2,160,_screen,event);
        Graphic_Change_Volume((RESOLUTION_W-SETTINGS_option_background->w)/2,200,_screen,event);
        //Graphic_Power_Saver((RESOLUTION_W-SETTINGS_option_background->w)/2,240,_screen,event);
        Graphic_Auto_Attack((RESOLUTION_W-SETTINGS_option_background->w)/2,240,_screen,event);
        Graphic_Reset_Saves((RESOLUTION_W-SETTINGS_option_background->w)/2,280,_screen,event);
        quit=Graphic_Back((RESOLUTION_W-SETTINGS_option_background->w)/2,400,_screen,event);
        Flip_Buffers(_screen);
        SDL_PollEvent(event);
        Update_Controllers_Events();
        SDL_Delay(20);
        if(event->type==SDL_MOUSEMOTION)
           selector_position=-1;
        if((controller[1].Pressed_Up() || controller[2].Pressed_Up()) && selector_position>0 && controller_timer.get_ticks()>CONTROLLER_DELAY)
           {
            selector_position--;
            controller_timer.start();
           }
        if((controller[1].Pressed_Down() || controller[2].Pressed_Down()) && selector_position<number_of_options-1 && controller_timer.get_ticks()>CONTROLLER_DELAY)
           {
            selector_position++;
            controller_timer.start();
           }
        if(controller_timer.get_ticks()>CONTROLLER_DELAY && (controller[1].Pressed_A_button() || controller[2].Pressed_A_button() ||
                                                             controller[1].Pressed_Start_button() || controller[2].Pressed_Start_button()))
           {
            click_position=selector_position;
            controller_timer.start();
           }
        if(event->type==SDL_QUIT || ((event->type==SDL_KEYDOWN && event->key.keysym.scancode==SDL_SCANCODE_ESCAPE) || (controller[1].Pressed_Guide_button() || controller[2].Pressed_Guide_button())))
           quit=true;
       }
}

void Graphic_Change_Resolution(int x,int y,Texture *_screen,SDL_Event *event)
{
 if(event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h)
    selector_position=0;
 if(selector_position!=0)
    Apply_Texture(x,y,SETTINGS_option_background,_screen);
 else
    Apply_Texture(x,y,SETTINGS_option_background_selected,_screen);
 if(event->type==SDL_MOUSEBUTTONDOWN && event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h ||
    (click_position==0))
    {
     click_position=-1;
     RESOLUTION++;
     RESOLUTION%=NUMBER_OF_AVAILABLE_RESOLUTIONS;
     RESOLUTION_W=available_RESOLUTION_W[RESOLUTION];
     RESOLUTION_H=available_RESOLUTION_H[RESOLUTION];
     SDL_SetWindowSize(WINDOW,RESOLUTION_W,RESOLUTION_H);
     SCREEN_SURFACE=SDL_GetWindowSurface(WINDOW);
     SCREEN->w=RESOLUTION_W;
     SCREEN->h=RESOLUTION_H;
     static_screen=SCREEN;
     Destroy_Texture(last_frame);
     last_frame=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
     return;
    }
 TTF_Font *font;
 font=TTF_OpenFont("fonts/pixel.ttf",20);
 Texture *image=NULL;
 char text[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 itoa(RESOLUTION_W,text);
 itoa(RESOLUTION_H,aux);
 strcat(text," x ");
 strcat(text,aux);
 image=Create_TTF_Texture(font,text,settings_color);
 TTF_CloseFont(font);
 Apply_Texture(x+20,y+10,image,_screen);
 Destroy_Texture(image);
}

void Graphic_Change_Display_Mode(int x,int y,Texture *_screen,SDL_Event *event)
{
 if(event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h)
    selector_position=1;
 if(selector_position!=1)
    Apply_Texture(x,y,SETTINGS_option_background,_screen);
 else
    Apply_Texture(x,y,SETTINGS_option_background_selected,_screen);
 if((event->type==SDL_MOUSEBUTTONDOWN && event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h) ||
    (click_position==1))
    {
     click_position=-1;
     if(DISPLAY_MODE==SDL_WINDOW_FULLSCREEN)
        DISPLAY_MODE=0;
     else
        DISPLAY_MODE=SDL_WINDOW_FULLSCREEN;
     SDL_SetWindowFullscreen(WINDOW,DISPLAY_MODE);
     SCREEN_SURFACE=SDL_GetWindowSurface(WINDOW);
     return;
    }
 TTF_Font *font;
 font=TTF_OpenFont("fonts/pixel.ttf",20);
 Texture *image=NULL,*image1=NULL;
 image=Create_TTF_Texture(font,"Fullscreen:",settings_color);
 Apply_Texture(x+20,y+10,image,_screen);
 if(DISPLAY_MODE==SDL_WINDOW_FULLSCREEN)
    image1=Create_TTF_Texture(font,"ON",settings_color);
 else
    image1=Create_TTF_Texture(font,"OFF",settings_color);
 Apply_Texture(x+image->w+20,y+10,image1,_screen);
 TTF_CloseFont(font);
 Destroy_Texture(image1);
 Destroy_Texture(image);
}

void Graphic_Change_Volume(int x,int y,Texture *_screen,SDL_Event *event)
{
 if(event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h)
    selector_position=2;
 if(selector_position!=2)
    Apply_Texture(x,y,SETTINGS_option_background,_screen);
 else
    Apply_Texture(x,y,SETTINGS_option_background_selected,_screen);
 if(event->type==SDL_MOUSEBUTTONDOWN && event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h ||
    (click_position==2))
    {
     click_position=-1;
     VOL++;
     VOL%=NUMBER_OF_AVAILABLE_VOLUMES;
     VOLUME=available_volumes[VOL];
     Mix_Volume(-1,MIX_MAX_VOLUME*VOLUME/100);
     Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
     SDL_Delay(75);
    }
 TTF_Font *font;
 font=TTF_OpenFont("fonts/pixel.ttf",20);
 Texture *image=NULL,*image1=NULL;
 image=Create_TTF_Texture(font,"Volume:",settings_color);
 Apply_Texture(x+20,y+10,image,_screen);
 image1=Create_TTF_Texture(font,available_volumes_names[VOL],settings_color);
 Apply_Texture(x+image->w+20,y+10,image1,_screen);
 TTF_CloseFont(font);
 Destroy_Texture(image1);
 Destroy_Texture(image);
}

void Graphic_Power_Saver(int x,int y,Texture *_screen,SDL_Event *event)
{
 Apply_Texture(x,y,SETTINGS_option_background,_screen);
 if(event->type==SDL_MOUSEBUTTONDOWN && event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h ||
    (click_position==3))
    {
     POWER_SAVER=!POWER_SAVER;
     SDL_Delay(75);
    }
 TTF_Font *font;
 font=TTF_OpenFont("fonts/pixel.ttf",20);
 Texture *image=NULL,*image1=NULL;
 image=Create_TTF_Texture(font,"Power Saver:",settings_color);
 Apply_Texture(x+20,y+10,image,_screen);
 if(POWER_SAVER)
    image1=Create_TTF_Texture(font,"ON",settings_color);
 else
    image1=Create_TTF_Texture(font,"OFF",settings_color);
 Apply_Texture(x+image->w+20,y+10,image1,_screen);
 TTF_CloseFont(font);
 Destroy_Texture(image1);
 Destroy_Texture(image);
}

void Graphic_Auto_Attack(int x,int y,Texture *_screen,SDL_Event *event)
{
 if(event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h)
    selector_position=3;
 if(selector_position!=3)
    Apply_Texture(x,y,SETTINGS_option_background,_screen);
 else
    Apply_Texture(x,y,SETTINGS_option_background_selected,_screen);
 if(event->type==SDL_MOUSEBUTTONDOWN && event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h ||
    (click_position==3))
    {
     click_position=-1;
     AUTO_ATTACK=!AUTO_ATTACK;
     SDL_Delay(75);
    }
 TTF_Font *font;
 font=TTF_OpenFont("fonts/pixel.ttf",20);
 Texture *image=NULL,*image1=NULL;
 image=Create_TTF_Texture(font,"Auto Attack:",settings_color);
 Apply_Texture(x+20,y+10,image,_screen);
 if(AUTO_ATTACK)
    image1=Create_TTF_Texture(font,"ON",settings_color);
 else
    image1=Create_TTF_Texture(font,"OFF",settings_color);
 Apply_Texture(x+image->w+20,y+10,image1,_screen);
 TTF_CloseFont(font);
 Destroy_Texture(image1);
 Destroy_Texture(image);
}

void Graphic_Reset_Saves(int x,int y,Texture *_screen,SDL_Event *event)
{
 if(event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h)
    selector_position=4;
 if(selector_position!=4)
    Apply_Texture(x,y,SETTINGS_option_background,_screen);
 else
    Apply_Texture(x,y,SETTINGS_option_background_selected,_screen);
 if(event->type==SDL_MOUSEBUTTONDOWN && event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h ||
    (click_position==4))
    {
     click_position=-1;
     system("reset-saves.bat");
     SDL_Delay(75);
    }
 TTF_Font *font;
 font=TTF_OpenFont("fonts/pixel.ttf",20);
 Texture *image=NULL;
 image=Create_TTF_Texture(font,"Reset Saves",settings_color);
 Apply_Texture(x+20,y+10,image,_screen);
 TTF_CloseFont(font);
 Destroy_Texture(image);
}

bool Graphic_Back(int x,int y,Texture *_screen,SDL_Event *event)
{
 if(event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h)
    selector_position=5;
 if(selector_position!=5)
    Apply_Texture(x,y,SETTINGS_option_background,_screen);
 else
    Apply_Texture(x,y,SETTINGS_option_background_selected,_screen);
 if(event->type==SDL_MOUSEBUTTONDOWN && event->button.x>=x && event->button.x<=x+SETTINGS_option_background->w && event->button.y>=y && event->button.y<=y+SETTINGS_option_background->h ||
    (click_position==5))
    {
     click_position=-1;
     return true;
    }
 TTF_Font *font;
 font=TTF_OpenFont("fonts/pixel.ttf",20);
 Texture *image=NULL;
 image=Create_TTF_Texture(font,"Back",settings_color);
 Apply_Texture(x+20,y+10,image,_screen);
 TTF_CloseFont(font);
 Destroy_Texture(image);
 return false;
}
