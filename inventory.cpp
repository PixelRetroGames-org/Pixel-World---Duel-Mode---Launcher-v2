#include "inventory.h"

void player_inventory::Print_Inventory(SDL_Surface *screen,char *_player_name)
{
 SDL_Thread *_loading_image=NULL;
 static_screen=screen;
 _loading_image=SDL_CreateThread(Loading_image,NULL);
 Player player;
 player.Set_name(_player_name);
 player.Load();
 player.Set_PLAYER_INFO_LAST_POSX(RESOLUTION_X);
 player.Set_PLAYER_INFO_POSX(RESOLUTION_X-480-70);
 player.Set_SKIN_POSX(RESOLUTION_X-190);
 if(RESOLUTION_X>=1366)
    {
     player.Set_PLAYER_INFO_LAST_POSX(RESOLUTION_X-(RESOLUTION_X-4*180-480-70)/2);
     player.Set_PLAYER_INFO_POSX(RESOLUTION_X-(RESOLUTION_X-4*180-480-70)/2-480-70);
     player.Set_SKIN_POSX(RESOLUTION_X-(RESOLUTION_X-4*180-480-70)/2-190);
    }
 int thread_return_value=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 SDL_WaitThread(_loading_image,&thread_return_value);
 SDL_Flip(static_screen);
 player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
 player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,1);
 SDL_Flip(screen);
 Timer fps;
 bool quit=false;
 SDL_Event event;
 while(!quit)
       {
        fps.start();
        if(SDL_PollEvent(&event) && !quit)
           {
            int _item_id=player.Start_inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,&event,1);
            if(_item_id!=-1)
               {
                player.Print_Character(player.Get_PLAYER_INFO_POSX(),0,screen);
                player.Print_Inventory(player.Get_PLAYER_INFO_POSX(),player.Get_pos_last_y(),screen,true,1);
               }
            if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
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
 while(SDL_PollEvent(&event))
       SDL_PumpEvents();
}
