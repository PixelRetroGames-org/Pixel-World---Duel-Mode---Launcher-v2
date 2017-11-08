#include "shop.h"

///SHOP
Shop::Shop()
{
 name[0]=NULL;
 number_of_pages=0;
 page_selected=page_click=0;
}

void Shop::Clear()
{
 for(int i=0;i<number_of_pages;i++)
     pages[i].Clear(true);
 std::vector<Shop_Page>().swap(pages);
 name[0]=NULL;
 number_of_pages=0;
 page_selected=page_click=0;
 changed_page=false;
 POSX=LAST_POSX=0;
}

int Shop::Get_shop_page_type()
{
 return pages[page_click].Get_type();
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
 char path[TEXT_LENGTH_MAX]={NULL},page_name[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"shop/");
 strcat(path,name);
 strcat(path,".pwsh");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d ",&number_of_pages);
 Shop_Page aux;
 for(int i=0;i<number_of_pages;i++)
     {
      fgets(page_name,TEXT_LENGTH_MAX,where);
      if(page_name[strlen(page_name)-1]=='\n')
         page_name[strlen(page_name)-1]=NULL;
      aux.Clear();
      aux.Set_name(page_name);
      aux.Load();
      aux.Set_POSX(POSX);
      aux.Set_LAST_POSX(LAST_POSX);
      pages.push_back(aux);
     }
 fclose(where);
}

void Shop::Print(Texture *_screen)
{
 int _x=0,_y=0;
 for(int i=0;i<number_of_pages;i++)
     {
      _x+=pages[i].Get_title_size();
     }
 Apply_Texture(0,0,_x,50,SHOP_title_clear,_screen);
 Apply_Texture(0,0,SHOP_shop_big_background,_screen);
 _x=0,_y=0;
 for(int i=0;i<number_of_pages;i++)
     {
      pages[i].Print_Title(_x,_y,_screen,(i==page_selected),(i==page_click));
      _x+=pages[i].Get_title_size();
     }
 Apply_Texture(_x,45,LAST_POSX-_x,SHOP_title_background_line->h,SHOP_title_background_line,_screen);
}

void Shop::Reset()
{
 pages[page_click].Reset();
 page_click=page_selected=0;
}

void Shop::Set_Controller_Timer(Timer *_controller_timer)
{
 controller_timer=_controller_timer;
 for(int i=0;i<number_of_pages;i++)
     pages[i].Set_Controller_Timer(_controller_timer);
}

const int CONTROLLER_DELAY=200;

inline int Shop::Start(Texture *_screen,SDL_Event *event,int _player_id)
{
 player_id=_player_id;
 if(event->type==SDL_MOUSEMOTION || event->type==SDL_MOUSEBUTTONDOWN)
    {
     int _x=0,_y=0,mouse_x=0,mouse_y=0;
     mouse_x=event->button.x,mouse_y=event->button.y;
     page_selected=-1;
     for(int i=0;i<number_of_pages;i++)
         {
          if(mouse_x>=_x && mouse_x<_x+pages[i].Get_title_size() && mouse_y>=_y && mouse_y<_y+PIXELS_PER_INGAME_UNIT)
             page_selected=i;
          _x+=pages[i].Get_title_size();
         }
    }
 if(event->type==SDL_MOUSEBUTTONDOWN && page_selected!=-1)
    {
     page_click=page_selected;
    }
 if(page_selected>0 && controller_timer->get_ticks()>CONTROLLER_DELAY && (controller[player_id].Pressed_LeftShoulder()))
    {
     page_click=--page_selected;
     controller_timer->start();
    }
 if(page_selected<number_of_pages-1 && controller_timer->get_ticks()>CONTROLLER_DELAY && (controller[player_id].Pressed_RightShoulder()))
    {
     page_click=++page_selected;
     controller_timer->start();
    }
 Print(_screen);
 return pages[page_click].Start(_screen,event);
}

///SHOP_SCREEN
SDL_Color MESSAGE_COLOR={255,255,255};
const int FRAMES_PER_SECOND=60;

int Shop_Screen::Start(Texture *screen)
{
 SDL_Thread *_loading_image=NULL;
 static_screen=screen;
 _loading_image=SDL_CreateThread(Loading_image,"Shop Loading",NULL);
 player.Set_PLAYER_INFO_LAST_POSX(RESOLUTION_W);
 player.Set_PLAYER_INFO_POSX(RESOLUTION_W-480-70);
 player.Set_SKIN_POSX(RESOLUTION_W-190);
 shop.Set_LAST_POSX(player.Get_PLAYER_INFO_POSX()-10);
 if(RESOLUTION_W>=1366)
    {
     player.Set_PLAYER_INFO_LAST_POSX(RESOLUTION_W-(RESOLUTION_W-4*180-480-70)/2);
     player.Set_PLAYER_INFO_POSX(RESOLUTION_W-(RESOLUTION_W-4*180-480-70)/2-480-70);
     player.Set_SKIN_POSX(RESOLUTION_W-(RESOLUTION_W-4*180-480-70)/2-190);
     shop.Set_LAST_POSX(player.Get_PLAYER_INFO_POSX()-10);
     shop.Set_POSX(5);
     shop.Set_LAST_POSX(5+4*180+20);
    }
 shop.Load();
 shop.Set_Controller_Timer(&controller_timer);
 player.Load();
 int thread_return_value=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 SDL_WaitThread(_loading_image,&thread_return_value);
 Flip_Buffers(screen);
 shop.Print(screen);
 player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
 player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,shop.Get_shop_page_type());
 Flip_Buffers(screen);
 bool quit=false;
 SDL_Event event;
 TTF_Font *font=NULL;
 font=TTF_OpenFont("fonts/pixel.ttf",30);
 Texture *not_enough_money=NULL,*not_enough_space_items=NULL,*not_enough_space_spells=NULL,*not_enough_space_potions=NULL,*not_enough_background=NULL;
 not_enough_money=Create_TTF_Texture(font,"You don't have enough money to buy this!",MESSAGE_COLOR);
 not_enough_space_items=Create_TTF_Texture(font,"Not enough space, sell some items!",MESSAGE_COLOR);
 not_enough_space_spells=Create_TTF_Texture(font,"Not enough space, sell some spells!",MESSAGE_COLOR);
 not_enough_space_potions=Create_TTF_Texture(font,"Not enough space, sell some potions!",MESSAGE_COLOR);
 TTF_CloseFont(font);
 not_enough_background=Load_Transparent_Texture("images/shop/not_enough_background.png");
 int message=0,nr=0;
 Timer fps;
 controller_timer.start();
 player.Set_Controller_Timer(&controller_timer);
 int shop_page_type=shop.Get_shop_page_type();
 bool inventory_focus=false;
 while(!quit)
       {
        fps.start();
        if((SDL_PollEvent(&event) || controller[player_id].Pressed_Any_Button()) && !quit)
              {
               Update_Controllers_Events();
               int _item_id=-1;
               if(!inventory_focus || event.type==SDL_MOUSEMOTION || event.type==SDL_MOUSEBUTTONDOWN)
                  {
                   _item_id=shop.Start(screen,&event,player_id);
                  }
               else
                  {
                   shop.Print(screen);
                  }
               if(_item_id!=-1)
                  {
                   if(player.Is_bought(_item_id) && !Is_potion(_item_id))
                      {
                       if(shop.Get_shop_page_type()==1)
                          player.Equip(_item_id);
                      }
                   else
                      {
                       message=player.Buy(_item_id);
                      }
                  }
               if(shop_page_type!=shop.Get_shop_page_type())
                  {
                   shop_page_type=shop.Get_shop_page_type();
                   player.Set_inventory_item_selected_position(-1,shop.Get_shop_page_type());
                  }
               if(inventory_focus || event.type==SDL_MOUSEMOTION || event.type==SDL_MOUSEBUTTONDOWN)
                  {
                   _item_id=player.Start_inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,&event,shop.Get_shop_page_type());
                  }
               player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
               player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,shop.Get_shop_page_type());
               if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE) ||
                  (controller[player_id].Pressed_Guide_button()))
                  quit=true;

               int number_of_inventory_items=0;
               switch(shop.Get_shop_page_type())
                      {
                       case 1: number_of_inventory_items=player.Get_inventory_number_of_items(); break;
                       case 2: number_of_inventory_items=player.Get_inventory_number_of_spells(); break;
                      }
               if(inventory_focus && number_of_inventory_items==0)
                  {
                   inventory_focus=!inventory_focus;
                  }

               if(controller[player_id].Pressed_Start_button() && controller_timer.get_ticks()>CONTROLLER_DELAY)
                  {
                   int number_of_inventory_items=0;
                   switch(shop.Get_shop_page_type())
                          {
                           case 1: number_of_inventory_items=player.Get_inventory_number_of_items(); break;
                           case 2: number_of_inventory_items=player.Get_inventory_number_of_spells(); break;
                          }

                   inventory_focus=!inventory_focus;
                   if(inventory_focus && number_of_inventory_items==0)
                      inventory_focus=!inventory_focus;
                   if(inventory_focus)
                      player.Set_inventory_item_selected_position(0,shop.Get_shop_page_type());
                   else
                      player.Set_inventory_item_selected_position(-1,shop.Get_shop_page_type());
                   SDL_Delay(100);
                   Update_Controllers_Events();
                   while(SDL_PollEvent(&event));
                   controller_timer.start();
                  }
               switch(message)
                      {
                       case 0:{break;};
                       case 1:{Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2-5,381,not_enough_background,screen);
                               Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2,384,not_enough_money,screen);
                               nr++;break;};
                       case 2:{Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2-5,381,not_enough_background,screen);
                               Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_space_items->w/2,384,not_enough_space_items,screen);
                               nr++;break;};
                       case 3:{Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2-5,381,not_enough_background,screen);
                               Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_space_spells->w/2,384,not_enough_space_spells,screen);
                               nr++;break;};
                       case 4:{Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_money->w/2-5,381,not_enough_background,screen);
                               Apply_Texture(player.Get_PLAYER_INFO_POSX()/2-not_enough_space_potions->w/2,384,not_enough_space_potions,screen);
                               nr++;break;};
                      }
               if(nr>5)
                  message=0,nr=0;
               Flip_Buffers(screen);
              }
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND)
           {
            SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
           }
        //SDL_Flip(screen);
        //SDL_Delay(16);
       }
 player.Update();
 Destroy_Texture(not_enough_money);
 Destroy_Texture(not_enough_space_items);
 Destroy_Texture(not_enough_space_spells);
 Destroy_Texture(not_enough_space_potions);
 Destroy_Texture(not_enough_background);
 player.Clear(true);
 shop.Reset();
 shop.Clear();
 if(event.type==SDL_QUIT)
    return -1;
 while(SDL_PollEvent(&event))
       SDL_PumpEvents();
 return 0;
}

int Shop_Screen::Start(Texture *screen,char *shop_name,char *player_name,int _player_id)
{
 player.Set_name(player_name);
 player.Set_id(_player_id);
 shop.Set_name(shop_name);
 player_id=_player_id;
 return Start(screen);
}
