//MENU
#include "menu.h"
//CSTRING
#include<cstring>

void Option::Load(FILE *where)
{
 fscanf(where,"%s %s %d",text,font_name,&font_size);
 fscanf(where,"%d %d %d ",&color.r,&color.g,&color.b);
 fscanf(where,"%d %d %d %d ",&screen_pos.x,&screen_pos.y,&background_pos.x,&background_pos.y);
}

void Option::Set_text(char *_text)
{
 strcpy(text,_text);
}

void Option::Set_screen_pos(int x,int y)
{
 screen_pos.x=x;
 screen_pos.y=y;
}

void Option::Set_background_pos(int x,int y)
{
 background_pos.x=x;
 background_pos.y=y;
}

void Option::Set_color(Uint8 r,Uint8 g,Uint8 b)
{
 color.r=r;
 color.g=g;
 color.b=b;
}

void Option::Set_font_name(char *_font_name)
{
 strcpy(font_name,font_name);
}

void Option::Set_font_size(int _size)
{
 font_size=_size;
}

void Option::Set_size()
{
 char font_path[TEXT_LENGHT_MAX];
 strcat(font_path,"fonts/");
 strcat(font_path,font_name);
 TTF_Font *font=TTF_OpenFont(font_path,font_size);
 SDL_Surface *image=NULL;
 image=SDL_LoadBMP("images/menu/menu_background.bmp");
 background_pos.w=image->w;
 background_pos.h=image->h;
 image=TTF_RenderText_Solid(font,text,color);
 screen_pos.w=image->w;
 screen_pos.h=image->h;
}

SDL_Rect Option::Get_screen_pos()
{
 return screen_pos;
}

SDL_Rect Option::Get_background_pos()
{
 return background_pos;
}

void Option::Print_text(SDL_Surface *_screen,bool selected=false,bool click=false)
{
 char font_path[TEXT_LENGHT_MAX];
 strcat(font_path,"fonts/");
 strcat(font_path,font_name);
 TTF_Font *font=TTF_OpenFont(font_path,font_size);
 SDL_Surface *image=NULL;
 if(!selected && !click)
    image=SDL_LoadBMP("images/menu/menu_background.bmp");
 else
    if(selected)
       image=SDL_LoadBMP("images/menu/menu_background_selected.bmp");
    else
       image=SDL_LoadBMP("images/menu/menu_background_click.bmp");
 apply_surface(background_pos.x,background_pos.y,image,_screen);
 image=TTF_RenderText_Solid(font,text,color);
 apply_surface(screen_pos.x,screen_pos.y,image,_screen);
}

void Menu::Load(const char *filename)
{
 FILE *where=fopen(filename,"r");
 fscanf(where,"%d ",&number_of_options);
 for(int i=0;i<=number_of_options;i++)
     options[i].Load(where);
}

void Menu::Set_number_of_options(int _noptions)
{
 number_of_options=_noptions;
}

void Menu::Set_option(int _pos,Option _x)
{
 options[_pos]=_x;
}

void Menu::Print_options(SDL_Surface *_screen)
{
 for(int i=0;i<number_of_options;i++)
     options[i].Print_text(_screen,i==selector_position,i==click_position);
 SDL_Flip(_screen);
}

int Menu::Start(SDL_Surface *_screen)
{
 SDL_Event event;
 bool quit=false,done=false;
 int x,y;
 while(!quit && !done)
       {
        if(SDL_PollEvent(&event))
           {
            if(event.type==SDL_MOUSEMOTION)
               {
                x=event.motion.x;
                y=event.motion.y;
                selector_position=-1;
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
                if(selector_position!=-1)
                   done=true;
               }
            if(event.type==SDL_QUIT)
               quit=true;
           }
        Print_options(_screen);
       }
 if(done)
    return selector_position;
 return -1;
}
