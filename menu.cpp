//MENU
#include "menu.h"
//CSTRING
#include<cstring>

const int FRAMES_PER_SECOND=5;

void Menu_Option::Load(FILE *where)
{
 int _r,_g,_b;
 fscanf(where,"%s %s %d",text,font_name,&font_size);
 fscanf(where,"%d %d %d ",&_r,&_g,&_b);
 color.r=(Uint8)_r;
 color.g=(Uint8)_g;
 color.b=(Uint8)_b;
 fscanf(where,"%d %d %d %d ",&screen_pos.x,&screen_pos.y,&background_pos.x,&background_pos.y);
}

void Menu_Option::Set_text(char *_text)
{
 strcpy(text,_text);
}

void Menu_Option::Set_screen_pos(int x,int y)
{
 screen_pos.x=x;
 screen_pos.y=y;
}

void Menu_Option::Set_background_pos(int x,int y)
{
 background_pos.x=x;
 background_pos.y=y;
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

void Menu_Option::Set_size()
{
 char font_path[TEXT_LENGHT_MAX]={NULL};
 strcat(font_path,"fonts/");
 strcat(font_path,font_name);
 TTF_Font *font=TTF_OpenFont(font_path,font_size);
 SDL_Surface *image=NULL;
 image=MENU_background;
 background_pos.w=image->w;
 background_pos.h=image->h;
 image=TTF_RenderText_Solid(font,text,color);
 screen_pos.w=image->w;
 screen_pos.h=image->h;
}

SDL_Rect Menu_Option::Get_screen_pos()
{
 return screen_pos;
}

SDL_Rect Menu_Option::Get_background_pos()
{
 return background_pos;
}

void Menu_Option::Print_text(SDL_Surface *_screen,bool selected=false,bool click=false)
{
 char font_path[TEXT_LENGHT_MAX]={NULL};
 strcat(font_path,"fonts/");
 strcat(font_path,font_name);
 TTF_Font *font=TTF_OpenFont(font_path,font_size);
 SDL_Surface *image=NULL;
 if(!selected && !click)
    image=MENU_background;
 else
    if(click)
       image=MENU_background_click;
    else
       image=MENU_background_selected;
 apply_surface(background_pos.x,background_pos.y,image,_screen);
 image=TTF_RenderText_Solid(font,text,color);
 apply_surface(screen_pos.x,screen_pos.y,image,_screen);
}

void Menu::Load(const char *filename)
{
 FILE *where=fopen(filename,"r");
 fscanf(where,"%d ",&number_of_options);
 for(int i=0;i<number_of_options;i++)
     options[i].Load(where),options[i].Set_size();
}

void Menu::Set_number_of_options(int _noptions)
{
 number_of_options=_noptions;
}

void Menu::Set_option(int _pos,Menu_Option _x)
{
 options[_pos]=_x;
}

void Menu::Print_options(SDL_Surface *_screen)
{
 LAUNCHER_BBACKGROUND.Print_image(0,0,_screen);
 LAUNCHER_BBACKGROUND.Update_image_frame();
 for(int i=0;i<number_of_options;i++)
     options[i].Print_text(_screen,i==selector_position,i==click_position);
 SDL_Flip(_screen);
}

int Menu::Start(SDL_Surface *_screen)
{
 SDL_Event event;
 bool quit=false,done=false;
 int x,y;
 Timer fps;
 while(!quit && !done)
       {
        fps.start();
        if(SDL_PollEvent(&event))
           {
            if(event.type==SDL_MOUSEMOTION)
               {
                x=event.motion.x;
                y=event.motion.y;
                selector_position=-1;
                click_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect sq=options[i].Get_background_pos();
                     if(x>=sq.x && x<=sq.x+sq.w && y>=sq.y && y<=sq.y+sq.h)
                        selector_position=i;
                    }
               }
            if(event.type==SDL_MOUSEBUTTONDOWN)
               {
                x=event.button.x;
                y=event.button.y;
                click_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect sq=options[i].Get_background_pos();
                     if(x>=sq.x && x<=sq.x+sq.w && y>=sq.y && y<=sq.y+sq.h)
                        click_position=i;
                    }
               }
            if(event.type==SDL_MOUSEBUTTONUP)
               {
                x=event.button.x;
                y=event.button.y;
                selector_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect sq=options[i].Get_background_pos();
                     if(x>=sq.x && x<=sq.x+sq.w && y>=sq.y && y<=sq.y+sq.h)
                        selector_position=i;
                    }
                if(selector_position!=-1 && selector_position==click_position)
                   done=true;
                click_position=-1;
               }
            if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
               quit=true;
           }
        Print_options(_screen);
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND)
           {
            SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
           }
       }
 if(done)
    return selector_position;
 return -1;
}
