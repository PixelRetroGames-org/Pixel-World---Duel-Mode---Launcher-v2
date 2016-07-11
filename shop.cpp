#include "shop.h"
//CSTRING
#include<cstring>

///SHOP
Shop::Shop()
{
 name[0]=NULL;
 number_of_pages=0;
 page_selected=page_click=0;
}

void Shop::Set_POSX(int _x)
{
 POSX=_x;
}

void Shop::Set_LAST_POSX(int _x)
{
 LAST_POSX=_x;
}

void Shop::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Shop::Load()
{
 char path[TEXT_LENGHT_MAX]={NULL},page_name[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"shop/");
 strcat(path,name);
 strcat(path,".pwsh");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d ",&number_of_pages);
 Shop_Page aux;
 for(int i=0;i<number_of_pages;i++)
     {
      fgets(page_name,TEXT_LENGHT_MAX,where);
      if(page_name[strlen(page_name)-1]=='\n')
         page_name[strlen(page_name)-1]=NULL;
      aux.Set_name(page_name);
      aux.Load();
      aux.Set_POSX(POSX);
      pages.push_back(aux);
     }
 fclose(where);
}

void Shop::Print(SDL_Surface *_screen)
{
 int _x=0,_y=0;
 for(int i=0;i<number_of_pages;i++)
     {
      _x+=pages[i].Get_title_size();
     }
 apply_surface(0,0,_x,50,SHOP_title_clear,_screen);
 apply_surface(0,0,SHOP_shop_big_background,_screen);
 _x=0,_y=0;
 for(int i=0;i<number_of_pages;i++)
     {
      pages[i].Print_Title(_x,_y,_screen,(i==page_selected),(i==page_click));
      _x+=pages[i].Get_title_size();
     }
 apply_surface(_x,45,LAST_POSX-_x,45,SHOP_title_background_line,_screen);
}

int Shop::Start(SDL_Surface *_screen,SDL_Event *event)
{
 int _x=0,_y=0,mouse_x=event->button.x,mouse_y=event->button.y;
 page_selected=-1;
 for(int i=0;i<number_of_pages;i++)
     {
      if(mouse_x>=_x && mouse_x<=_x+pages[i].Get_title_size() && mouse_y>=_y && mouse_y<=_y+40)
         page_selected=i;
      _x+=pages[i].Get_title_size();
     }
 if(event->type==SDL_MOUSEBUTTONDOWN && page_selected!=-1)
    {
     page_click=page_selected;
    }
 Print(_screen);
 return pages[page_click].Start(_screen,event);
}

///SHOP_SCREEN
void Shop_Screen::Set_filename(char *_filename)
{
 strcpy(filename,_filename);
}

void Shop_Screen::Open(char *filename)
{
 char path[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"shop/");
 strcat(path,filename);
 strcat(path,".pwb");
 FILE *in=fopen(path,"r");
 char name[TEXT_LENGHT_MAX]={NULL};
 fgets(name,sizeof name,in);
 while(name[strlen(name)-1]=='\n')
       name[strlen(name)-1]=NULL;
 player.Set_name(name);
 name[0]=NULL;
 fgets(name,sizeof name,in);
 while(name[strlen(name)-1]=='\n')
       name[strlen(name)-1]=NULL;
 shop.Set_name(name);
 fclose(in);
}

SDL_Color MESSAGE_COLOR={255,255,255};

int Shop_Screen::Start(SDL_Surface *screen)
{
 player.Set_PLAYER_INFO_LAST_POSX(RESOLUTION_X);
 player.Set_PLAYER_INFO_POSX(RESOLUTION_X-480-70);
 player.Set_SKIN_POSX(RESOLUTION_X-190);
 shop.Set_LAST_POSX(player.Get_PLAYER_INFO_POSX()-10);
 if(RESOLUTION_X>=1366)
    {
     player.Set_PLAYER_INFO_LAST_POSX(RESOLUTION_X-(RESOLUTION_X-4*180-480-70)/2);
     player.Set_PLAYER_INFO_POSX(RESOLUTION_X-(RESOLUTION_X-4*180-480-70)/2-480-70);
     player.Set_SKIN_POSX(RESOLUTION_X-(RESOLUTION_X-4*180-480-70)/2-190);
     shop.Set_LAST_POSX(player.Get_PLAYER_INFO_POSX()-10);
     shop.Set_POSX(20);
     shop.Set_LAST_POSX(20+4*180+20);
    }
 Open(filename);
 shop.Load();
 shop.Print(screen);
 player.Load();
 player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
 player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen);
 bool quit=false;
 SDL_Event event;
 TTF_Font *font=NULL;
 font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Surface *not_enough_money=NULL,*not_enough_space=NULL,*not_enough_background=NULL;
 not_enough_money=TTF_RenderText_Solid(font,"You don't have enough money to buy this!",MESSAGE_COLOR);
 not_enough_space=TTF_RenderText_Solid(font,"Not enough space, sell some items!",MESSAGE_COLOR);
 not_enough_background=SDL_LoadBMP("images/shop/not_enough_background.bmp");
 int message=0,nr=0;
 while(!quit)
       {
        if(SDL_PollEvent(&event) && !quit)
              {
               int _item_id=shop.Start(screen,&event);
               if(_item_id!=-1)
                  {
                   if(player.Is_bought(_item_id) && !Is_potion(_item_id))
                      player.Equip(_item_id);
                   else
                      {
                       message=player.Buy(_item_id);
                      }
                   player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
                   player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen);
                  }
               _item_id=player.Start_inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,&event);
               if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
                  quit=true;
               switch(message)
                      {
                       case 0:{break;};
                       case 1:{apply_surface(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2-5,381,not_enough_background,screen);
                               apply_surface(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2,384,not_enough_money,screen);
                               nr++;break;};
                       case 2:{apply_surface(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2-5,381,not_enough_background,screen);
                               apply_surface(player.Get_PLAYER_INFO_POSX()/2-not_enough_space->w/2,384,not_enough_space,screen);
                               nr++;break;};
                      }
               if(nr>5)
                  message=0,nr=0;
               SDL_Flip(screen);
              }
        //SDL_Flip(screen);
        //SDL_Delay(16);
       }
 player.Update();
 if(event.type==SDL_QUIT)
    return -1;
 return 0;
}

int Shop_Screen::Start(SDL_Surface *screen,char *filename)
{
 Set_filename(filename);
 return Start(screen);
}
