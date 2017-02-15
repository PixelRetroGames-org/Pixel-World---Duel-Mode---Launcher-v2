#include "inventory.h"

void player_inventory::Print_Inventory(SDL_Surface *screen,char *_player_name)
{
 SDL_Thread *_loading_image=NULL;
 static_screen=screen;
 _loading_image=SDL_CreateThread(Loading_image,NULL);
 Player player;
 player.Set_name(_player_name);
 player.Load();
 player.Set_PLAYER_INFO_LAST_POSX((RESOLUTION_X-550)/2+550);
 player.Set_PLAYER_INFO_POSX((RESOLUTION_X-550)/2);
 player.Set_SKIN_POSX((RESOLUTION_X-550)/2+550-190);
 int thread_return_value=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 SDL_WaitThread(_loading_image,&thread_return_value);
 SDL_Flip(static_screen);
 apply_surface(0,0,SHOP_shop_big_background,screen);
 player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
 player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,1,false);
 apply_surface(player.Get_PLAYER_INFO_POSX()-INVENTORY_LARROW->w,player.Get_pos_last_y(),INVENTORY_LARROW,screen);
 apply_surface(player.Get_PLAYER_INFO_LAST_POSX(),player.Get_pos_last_y(),INVENTORY_RARROW,screen);
 SDL_Flip(screen);
 Timer fps;
 int type=1;
 bool quit=false;
 SDL_Event event;
 bool redraw=false;
 SDL_Delay(100);
 while(SDL_PollEvent(&event));
 while(!quit)
       {
        fps.start();
        if(SDL_PollEvent(&event) && !quit)
           {
            if(event.type==SDL_MOUSEBUTTONDOWN)
               {
                if(player.Get_PLAYER_INFO_POSX()-INVENTORY_LARROW->w<event.button.x && event.button.x<player.Get_PLAYER_INFO_POSX() &&
                   player.Get_pos_last_y()<event.button.y && event.button.y<player.Get_pos_last_y()+INVENTORY_LARROW->h)
                   {
                    if(type-1>=0)
                       type--,redraw=true;
                   }
                if(player.Get_PLAYER_INFO_LAST_POSX()<event.button.x && event.button.x<player.Get_PLAYER_INFO_LAST_POSX()+INVENTORY_RARROW->w &&
                   player.Get_pos_last_y()<event.button.y && event.button.y<player.Get_pos_last_y()+INVENTORY_RARROW->h)
                   {
                    if(type+1<=2)
                       type++,redraw=true;
                   }
               }
            int _item_id=player.Start_inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,&event,type,false);
            if(_item_id<0 || redraw || _item_id==666013)
               {
                redraw=false;
                apply_surface(0,0,SHOP_shop_big_background,screen);
                player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
                player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,type,false);
                apply_surface(player.Get_PLAYER_INFO_POSX()-INVENTORY_LARROW->w,player.Get_pos_last_y(),INVENTORY_LARROW,screen);
                apply_surface(player.Get_PLAYER_INFO_LAST_POSX(),player.Get_pos_last_y(),INVENTORY_RARROW,screen);
               }
            if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE) || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_i))
               quit=true;
            SDL_Flip(screen);
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
 SDL_Delay(100);
 while(SDL_PollEvent(&event));
}
