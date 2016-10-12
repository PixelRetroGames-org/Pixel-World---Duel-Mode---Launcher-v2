//MENU
#include "menu.h"
#include "settings.h"
//CSTRING
#include<cstring>

const int FRAMES_PER_SECOND_BACKGROUND=30,FRAMES_PER_SECOND_FIRE=25;

void Menu_Option::Clear()
{
 SDL_FreeSurface(text_image);
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
 screen_pos.x+=(RESOLUTION_X-MENU_background->w)/2;
 screen_pos.h=MENU_background->h;
 screen_pos.w=MENU_background->w;
 //Create text image
 char font_path[TEXT_LENGHT_MAX]={NULL};
 strcat(font_path,"fonts/");
 strcat(font_path,font_name);
 TTF_Font *font=TTF_OpenFont(font_path,font_size);
 text_image=TTF_RenderText_Solid(font,text,color);
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

void Menu_Option::Print_text(SDL_Surface *_screen,bool selected=false,bool click=false)
{
 SDL_Surface *image=NULL;
 if(!selected && !click)
    image=MENU_background;
 else
    if(click)
       image=MENU_background_click;
    else
       image=MENU_background_selected;
 apply_surface(screen_pos.x,screen_pos.y,image,_screen);
 apply_surface(screen_pos.x+(screen_pos.w-text_image->w)/2,screen_pos.y+10,text_image,_screen);
}

void Menu::Clear()
{
 SDL_FreeSurface(background);
 SDL_FreeSurface(title);
 for(int i=0;i<number_of_options;i++)
     options[i].Clear();
}

void Menu::Load(const char *filename)
{
 FILE *where=fopen(filename,"r");
 fscanf(where,"%d %d ",&number_of_options,&is_main_menu);
 if(!is_main_menu)
    {
     char path[TEXT_LENGHT_MAX]={NULL},background_name[TEXT_LENGHT_MAX]={NULL};
     fgets(background_name,sizeof background_name,where);
     if(background_name[strlen(background_name)-1]=='\n')
        background_name[strlen(background_name)-1]=NULL;
     strcpy(path,"images/menu/");
     strcat(path,background_name);
     strcat(path,".bmp");
     SDL_FreeSurface(background);
     background=load_image(path);
     fgets(path,sizeof path,where);
     if(path[strlen(path)-1]=='\n')
        path[strlen(path)-1]=NULL;
     TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",40);
     title=TTF_RenderText_Solid(font,path,SDL_Color{255,255,255});
     TTF_CloseFont(font);
     fscanf(where,"%d %d ",&title_position.x,&title_position.y);
    }
 for(int i=0;i<number_of_options;i++)
     options[i].Load(where);
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
 if(is_main_menu)
    {
     LAUNCHER_BBACKGROUND.Print_image(0,0,_screen);
     LAUNCHER_BBACKGROUND.Update_image_frame();
    }
 else
    {
     apply_surface(0,0,background,_screen);
     apply_surface((RESOLUTION_X-title->w)/2+title_position.x,title_position.y,title,_screen);
    }
 for(int i=0;i<number_of_options;i++)
     options[i].Print_text(_screen,i==selector_position,i==click_position);
 SDL_Flip(_screen);
}

int Menu::Start(SDL_Surface *_screen)
{
 selector_position=-1;
 click_position=-1;
 SDL_Event event;
 bool quit=false,done=false;
 int x,y;
 Timer fps,fps1;
 fps1.start();
 while(SDL_PollEvent(&event));
 while(!quit && !done)
       {
        fps.start();
        if(SDL_PollEvent(&event))
           {
            bool sqq=false;
            if(event.type==SDL_MOUSEMOTION)
               {
                x=event.motion.x;
                y=event.motion.y;
                selector_position=-1;
                click_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect sq=options[i].Get_screen_pos();
                     if(x>=sq.x && x<=sq.x+sq.w && y>=sq.y && y<=sq.y+sq.h)
                        selector_position=i;
                    }
                sqq=true;
               }
            if(event.type==SDL_MOUSEBUTTONDOWN)
               {
                x=event.button.x;
                y=event.button.y;
                click_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect sq=options[i].Get_screen_pos();
                     if(x>=sq.x && x<=sq.x+sq.w && y>=sq.y && y<=sq.y+sq.h)
                        click_position=i;
                    }
                sqq=true;
               }
            if(event.type==SDL_MOUSEBUTTONUP)
               {
                x=event.button.x;
                y=event.button.y;
                selector_position=-1;
                for(int i=0;i<number_of_options;i++)
                    {
                     SDL_Rect sq=options[i].Get_screen_pos();
                     if(x>=sq.x && x<=sq.x+sq.w && y>=sq.y && y<=sq.y+sq.h)
                        selector_position=i;
                    }
                if(selector_position!=-1 && selector_position==click_position)
                   done=true;
                click_position=-1;
                sqq=true;
               }
            if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
               quit=true,sqq=true;
            if(!sqq)
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
