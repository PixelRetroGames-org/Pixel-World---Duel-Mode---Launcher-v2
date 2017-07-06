#include "inventory.h"

const int NUMBER_OF_ITEM_TYPES=2;

const int CONTROLLER_DELAY=200;

void player_inventory::Print_Inventory(Texture *screen,char *_player_name)
{
 SDL_Thread *_loading_image=NULL;
 static_screen=screen;
 _loading_image=SDL_CreateThread(Loading_image,"Inventory Loading",NULL);
 Player player;
 player.Set_name(_player_name);
 player.Load();
 player.Set_id(1);
 player.Set_PLAYER_INFO_LAST_POSX((RESOLUTION_W-550)/2+550);
 player.Set_PLAYER_INFO_POSX((RESOLUTION_W-550)/2);
 player.Set_SKIN_POSX((RESOLUTION_W-550)/2+550-190);
 int thread_return_value=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 SDL_WaitThread(_loading_image,&thread_return_value);
 Flip_Buffers(screen);
 Apply_Texture(0,0,SHOP_shop_big_background,screen);
 player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
 player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,1,false);
 int type=1;
 if(type!=1)
    Apply_Texture(player.Get_PLAYER_INFO_POSX()-INVENTORY_LARROW->w,player.Get_pos_last_y(),INVENTORY_LARROW,screen);
 if(type!=NUMBER_OF_ITEM_TYPES)
    Apply_Texture(player.Get_PLAYER_INFO_LAST_POSX(),player.Get_pos_last_y(),INVENTORY_RARROW,screen);
 Flip_Buffers(screen);
 Timer fps;
 bool quit=false;
 SDL_Event event;
 SDL_Delay(100);
 while(SDL_PollEvent(&event));
 Timer controller_timer;
 player.Set_Controller_Timer(&controller_timer);
 controller_timer.start();
 while(!quit)
       {
        fps.start();
        if((SDL_PollEvent(&event) || controller[1].Pressed_Any_Button() || controller[2].Pressed_Any_Button()) && !quit)
           {
            Update_Controllers_Events();
            if(event.type==SDL_MOUSEBUTTONDOWN || controller[1].Pressed_Any_Button() || controller[2].Pressed_Any_Button())
               {
                if((player.Get_PLAYER_INFO_POSX()-INVENTORY_LARROW->w<event.button.x && event.button.x<player.Get_PLAYER_INFO_POSX() &&
                   player.Get_pos_last_y()<event.button.y && event.button.y<player.Get_pos_last_y()+INVENTORY_LARROW->h) ||
                   (controller_timer.get_ticks()>CONTROLLER_DELAY && (controller[1].Pressed_LeftShoulder() || controller[2].Pressed_LeftShoulder())))
                   {
                    controller_timer.start();
                    if(type-1>0)
                       type--,player.Set_inventory_item_selected_position(-1,type);
                   }
                if(player.Get_PLAYER_INFO_LAST_POSX()<event.button.x && event.button.x<player.Get_PLAYER_INFO_LAST_POSX()+INVENTORY_RARROW->w &&
                   player.Get_pos_last_y()<event.button.y && event.button.y<player.Get_pos_last_y()+INVENTORY_RARROW->h ||
                   (controller_timer.get_ticks()>CONTROLLER_DELAY && (controller[1].Pressed_RightShoulder() || controller[2].Pressed_RightShoulder())))
                   {
                    controller_timer.start();
                    if(type+1<=NUMBER_OF_ITEM_TYPES)
                       type++,player.Set_inventory_item_selected_position(-1,type);
                   }
               }
            int _item_id=player.Start_inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,&event,type,false);
            Apply_Texture(0,0,SHOP_shop_big_background,screen);
            player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
            player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,type,false);
            if(type!=1)
               Apply_Texture(player.Get_PLAYER_INFO_POSX()-INVENTORY_LARROW->w,player.Get_pos_last_y(),INVENTORY_LARROW,screen);
            if(type!=NUMBER_OF_ITEM_TYPES)
               Apply_Texture(player.Get_PLAYER_INFO_LAST_POSX(),player.Get_pos_last_y(),INVENTORY_RARROW,screen);
            if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE) || (event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_I) ||
               (controller[1].Pressed_Back_button() || controller[1].Pressed_Guide_button() || controller[2].Pressed_Back_button() || controller[2].Pressed_Guide_button()))
               quit=true;
            Flip_Buffers(screen);
           }
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND)
           {
            SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
           }
       }
 player.Update();
 player.Clear(true);
 if(event.type==SDL_QUIT)
    exit(0);
 SDL_Delay(150);
 while(SDL_PollEvent(&event));
}
