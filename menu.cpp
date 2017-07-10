#include "menu.h"

const int FRAMES_PER_SECOND_BACKGROUND=30,FRAMES_PER_SECOND_FIRE=25;

void Menu_Option::Clear()
{
 Destroy_Texture(text_image);
}

void Menu_Option::Load(FILE *where)
{
 int _r,_g,_b;
 fgets(text,sizeof text,where);
 if(text[strlen(text)-1]=='\n')
    text[strlen(text)-1]=NULL;
 fscanf(where,"%s %d",font_name,&font_size);
 fscanf(where,"%d %d %d ",&_r,&_g,&_b);
 color.r=(Uint8)_r;
 color.g=(Uint8)_g;
 color.b=(Uint8)_b;
 fscanf(where,"%d %d ",&screen_pos.x,&screen_pos.y);
 screen_pos.x+=(RESOLUTION_W-MENU_background->w)/2;
 screen_pos.h=MENU_background->h;
 screen_pos.w=MENU_background->w;
 //Create text image
 char font_path[TEXT_LENGTH_MAX]={NULL};
 strcat(font_path,"fonts/");
 strcat(font_path,font_name);
 TTF_Font *font=TTF_OpenFont(font_path,font_size);
 text_image=Create_TTF_Texture(font,text,color);
 TTF_CloseFont(font);
}

void Menu_Option::Set_text(char *_text)
{
 strcpy(text,_text);
}

void Menu_Option::Set_color(Uint8 r,Uint8 g,Uint8 b)
{
 color.r=r;
 color.g=g;
 color.b=b;
}

void Menu_Option::Set_font_name(char *_font_name)
{
 strcpy(font_name,font_name);
}

void Menu_Option::Set_font_size(int _size)
{
 font_size=_size;
}

SDL_Rect Menu_Option::Get_screen_pos()
{
 return screen_pos;
}

void Menu_Option::Print_text(Texture *_screen,bool selected=false,bool click=false)
{
 Texture *image=NULL;
 if(!selected && !click)
    image=MENU_background;
 else
    if(click)
       image=MENU_background_click;
    else
       image=MENU_background_selected;
 Apply_Texture(screen_pos.x,screen_pos.y,image,_screen);
 Apply_Texture(screen_pos.x+(screen_pos.w-text_image->w)/2,screen_pos.y+10,text_image,_screen);
}

void Menu::Clear()
{
 Destroy_Texture(background);
 Destroy_Texture(title);
 for(int i=0;i<number_of_options;i++)
     options[i].Clear();
}

void Menu::Load(const char *filename)
{
 FILE *where=fopen(filename,"r");
 fscanf(where,"%d %d ",&number_of_options,&is_main_menu);
 if(!is_main_menu)
    {
     char path[TEXT_LENGTH_MAX]={NULL},background_name[TEXT_LENGTH_MAX]={NULL};
     fgets(background_name,sizeof background_name,where);
     if(background_name[strlen(background_name)-1]=='\n')
        background_name[strlen(background_name)-1]=NULL;
     strcpy(path,"images/menu/");
     strcat(path,background_name);
     strcat(path,".png");
     background=Load_Texture(path);
     fgets(path,sizeof path,where);
     if(path[strlen(path)-1]=='\n')
        path[strlen(path)-1]=NULL;
     TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",40);
     title=Create_TTF_Texture(font,path,SDL_Color{255,255,255});
     TTF_CloseFont(font);
     fscanf(where,"%d %d ",&title_position.x,&title_position.y);
    }
 for(int i=0;i<number_of_options;i++)
     options[i].Load(where);
 fclose(where);
}

void Menu::Set_number_of_options(int _noptions)
{
 number_of_options=_noptions;
}

void Menu::Set_option(int _pos,Menu_Option _x)
{
 options[_pos]=_x;
}

void Menu::Print_options(Texture *_screen)
{
 if(is_main_menu)
    {
     LAUNCHER_BBACKGROUND.Print_image(0,0,_screen);
     LAUNCHER_BBACKGROUND.Update_image_frame();
    }
 else
    {
     Apply_Texture(0,0,background,_screen);
     Apply_Texture((RESOLUTION_W-title->w)/2+title_position.x,title_position.y,title,_screen);
    }
 for(int i=0;i<number_of_options;i++)
     options[i].Print_text(_screen,i==selector_position,i==click_position);
 Flip_Buffers(_screen);
}

const int CONTROLLER_DELAY=100;

int Menu::Start(Texture *_screen,bool *reload)
{
 selector_position=-1;
 click_position=-1;
 SDL_Event event;
 bool quit=false,done=false;
 int x,y;
 Timer fps,fps1,controller_timer;
 fps1.start();
 controller_timer.start();
 while(SDL_PollEvent(&event));
 bool focus=true,fullscreen,changed_window_status=false,_fullscreen;
 if(DISPLAY_MODE==SDL_WINDOW_FULLSCREEN)
    _fullscreen=true;
 while(!quit && !done)
       {
        fps.start();
        Update_Controllers_Events();
        if(SDL_PollEvent(&event))
           {
            bool event_action=false;
            if(event.type==SDL_MOUSEMOTION)
               {
                x=event.motion.x;
                y=event.motion.y;
                selector_position=-1;
                click_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect rect=options[i].Get_screen_pos();
                     if(x>=rect.x && x<=rect.x+rect.w && y>=rect.y && y<=rect.y+rect.h)
                        selector_position=i;
                    }
                event_action=true;
               }
            if(event.type==SDL_MOUSEBUTTONDOWN)
               {
                x=event.button.x;
                y=event.button.y;
                click_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect rect=options[i].Get_screen_pos();
                     if(x>=rect.x && x<=rect.x+rect.w && y>=rect.y && y<=rect.y+rect.h)
                        click_position=i;
                    }
                event_action=true;
               }
            if(event.type==SDL_MOUSEBUTTONUP)
               {
                //Mix_PlayChannel(0,CLICK,0);
                x=event.button.x;
                y=event.button.y;
                selector_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect rect=options[i].Get_screen_pos();
                     if(x>=rect.x && x<=rect.x+rect.w && y>=rect.y && y<=rect.y+rect.h)
                        selector_position=i;
                    }
                if(selector_position!=-1 && selector_position==click_position)
                   done=true;
                click_position=-1;
                event_action=true;
               }

            //Controller
            if((controller[1].Pressed_Up() || controller[2].Pressed_Up()) && selector_position>0 && controller_timer.get_ticks()>CONTROLLER_DELAY)
               {
                selector_position--;
                event_action=true;
                controller_timer.start();
               }
            if((controller[1].Pressed_Down() || controller[2].Pressed_Down()) && selector_position<number_of_options-1 && controller_timer.get_ticks()>CONTROLLER_DELAY)
               {
                selector_position++;
                event_action=true;
                controller_timer.start();
               }
            if(controller[1].Pressed_A_button() || controller[2].Pressed_A_button() ||
               controller[1].Pressed_Start_button() || controller[2].Pressed_Start_button() && controller_timer.get_ticks()>CONTROLLER_DELAY)
               {
                click_position=selector_position;
                done=true;
                event_action=true;
                controller_timer.start();
               }

            //Lost Focus
            if(event.type==SDL_WINDOWEVENT && _fullscreen && reload!=nullptr)
               {
                if(event.window.event==SDL_WINDOWEVENT_FOCUS_LOST || event.window.event==SDL_WINDOWEVENT_FOCUS_LOST || event.window.event==SDL_WINDOWEVENT_MINIMIZED)
                   focus=false,changed_window_status=true;
                if(event.window.event==SDL_WINDOWEVENT_FOCUS_GAINED || event.window.event==SDL_WINDOWEVENT_FOCUS_GAINED || event.window.event==SDL_WINDOWEVENT_MAXIMIZED)
                   focus=true,changed_window_status=true;
                if(!focus && changed_window_status)
                   {
                    if(DISPLAY_MODE==SDL_WINDOW_FULLSCREEN)
                       DISPLAY_MODE=0,fullscreen=true;
                    SDL_SetWindowFullscreen(WINDOW,DISPLAY_MODE);
                    SCREEN_SURFACE=SDL_GetWindowSurface(WINDOW);
                   }
                if(focus && changed_window_status)
                   {
                    if(fullscreen)
                       DISPLAY_MODE=SDL_WINDOW_FULLSCREEN,fullscreen=false;
                    SDL_SetWindowFullscreen(WINDOW,DISPLAY_MODE);
                    SCREEN_SURFACE=SDL_GetWindowSurface(WINDOW);
                    *reload=true;
                    SDL_PumpEvents();
                   }
               }

            if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE) || (controller[1].Pressed_Guide_button() || controller[2].Pressed_Guide_button()))
               quit=true,event_action=true;
            if(!event_action)
               continue;
           }
        Print_options(_screen);
        if(fps1.get_ticks()>=1000/FRAMES_PER_SECOND_FIRE)
           {
            LAUNCHER_BBACKGROUND.Update_fire_frame();
            fps1.start();
           }
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND_BACKGROUND)
           {
            SDL_Delay((1000/FRAMES_PER_SECOND_BACKGROUND)-fps.get_ticks());
           }
       }
 if(done)
    return selector_position;
 if(quit)
    return -2;
 return -1;
}
