#include "level.h"
#include "load_audio_effects.h"
#include "settings.h"
#include "menu.h"
#include "SDL/SDL_thread.h"

constexpr SDLKey player_keys[3][20]={{},{SDLK_UP,SDLK_DOWN,SDLK_LEFT,SDLK_RIGHT,SDLK_RCTRL,SDLK_j,SDLK_n,SDLK_u,SDLK_i,SDLK_o,SDLK_p},{SDLK_w,SDLK_s,SDLK_a,SDLK_d,SDLK_z,SDLK_TAB,SDLK_BACKQUOTE,SDLK_1,SDLK_2,SDLK_3,SDLK_4}};

Level::Level()
{
 player_name[0][0]=player_name[1][0]=player_name[2][0]=NULL;
 arena_size.x=(RESOLUTION_X-840)/2,arena_size.y=40;
 arena_size.w=860;
 arena_size.h=680;
}

void Level::Clear()
{
 player[1].Clear(true);
 if(type==2)
    player[2].Clear(true);
 arena.Clear();
 effects.Clear();
 darkness.Clear();
}

void Level::Set_arena_size()
{
 arena_size.x=(RESOLUTION_X-840)/2,arena_size.y=40;
 arena_size.w=840;
 arena_size.h=680;
}

void Level::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Level::Set_player_map_position(int x,int y,int _player)
{
 player[_player].Set_map_position(x,y);
}

void Level::Set_player_mana(int _mana,int _player)
{
 player[_player].Set_mana(_mana);
}

void Level::Set_player_hp(int _hp,int _player)
{
 player[_player].Set_hp(_hp);
}

void Level::Set_player_mental_health(int _mental_health,int _player)
{
 player[_player].Set_mental_health(_mental_health);
}

void Level::Set_player_POSX(int _player,int _x)
{
 player[_player].Set_PLAYER_INFO_POSX(_x);
}

void Level::Set_player_LAST_POSX(int _player,int _x)
{
 player[_player].Set_PLAYER_INFO_LAST_POSX(_x);
}

void Level::Set_player_velocityX(int _player,int _velocityX)
{
 player[_player].Set_velocityX(_velocityX);
}

void Level::Set_player_velocityY(int _player,int _velocityY)
{
 player[_player].Set_velocityY(_velocityY);
}

/*int static_number_of_background_music_tracks;
Mix_Music *static_background_music[NUMBER_OF_SONGS_MAX];*/

void Level::Load()
{
 done=false;
 char path[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"levels/");
 strcat(path,name);
 strcat(path,".pwl");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    return;
 int x,y,last_terrain_type=terrain_type;
 fscanf(where,"%d %d %d ",&type,&terrain_type,&duration);

 fgets(player_name[1],sizeof player_name[1],where);
 if(player_name[1][strlen(player_name[1])-1]=='\n')
    player_name[1][strlen(player_name[1])-1]=NULL;

 fscanf(where,"%d %d %d ",&x,&y,&player_type[1]);
 player[1].Set_name(player_name[1]);
 player[1].Set_map_position(x,y);
 player[1].Load();

 player[2].Set_map_position(-5,-5);

 if(type==2)
    {
     fgets(player_name[2],sizeof player_name[2],where);
     if(player_name[2][strlen(player_name[2])-1]=='\n')
        player_name[2][strlen(player_name[2])-1]=NULL;

     fscanf(where,"%d %d %d ",&x,&y,&player_type[2]);
     player[2].Set_name(player_name[2]);
     player[2].Set_map_position(x,y);
     player[2].Load();
    }

 fgets(arena_name,sizeof arena_name,where);
 if(arena_name[strlen(arena_name)-1]=='\n')
    arena_name[strlen(arena_name)-1]=NULL;

 fgets(darkness_image_name,sizeof darkness_image_name,where);
 if(darkness_image_name[strlen(darkness_image_name)-1]=='\n');
    darkness_image_name[strlen(darkness_image_name)-1]=NULL;
 int alpha,number_of_frames;
 fscanf(where,"%d %d",&alpha,&number_of_frames);
 darkness.Set_number_of_frames(number_of_frames);
 darkness.Set_aplha(alpha);
 darkness.Set_image_name(darkness_image_name);
 darkness.Load_image();
 darkness.Update_image();

 if(terrain_type!=last_terrain_type || type==2)
    {
     for(int i=0;i<number_of_background_music_tracks;i++)
         {
          Mix_FreeMusic(background_music[i]);
         }
    }
 fscanf(where,"%d ",&number_of_background_music_tracks);
 for(int i=0;i<number_of_background_music_tracks && (terrain_type!=last_terrain_type || type==2);i++)
     {
      fgets(background_music_name,sizeof background_music_name,where);
      if(background_music_name[strlen(background_music_name)-1]=='\n')
         background_music_name[strlen(background_music_name)-1]=NULL;
      strcpy(path,"audio/");
      strcat(path,background_music_name);
      strcat(path,".wav");
      background_music[i]=Mix_LoadMUS(path);
     }

 fclose(where);
 arena.Set_name(arena_name);
 arena.Load();
 effects.Set_name("Empty");
 effects.Load();
}

void Level::Change(char *_level_name)
{
 Clear();
 Setup(_level_name);
}

void Level::Update_players()
{
 player[1].Update();
 player[2].Update();
}

void Level::Update_all_arena_frames()
{
 arena.Update_all_frames();
 effects.Update_all_frames();
}

void Level::Start_music()
{
 if(Mix_PlayingMusic()==0)
    Mix_PlayMusic(background_music[0],0);
}

void Level::Pause_music()
{
 if(Mix_PlayingMusic()==1)
    Mix_PauseMusic();
}

void Level::Unpause_music()
{
 if(Mix_PausedMusic()==1)
    Mix_ResumeMusic();
}

int Level::Change_music(bool play)
{
 if(Mix_PlayingMusic()==0)
    {
     if(play)
        Mix_PlayMusic(background_music[rand()%number_of_background_music_tracks],0);
     return 1;
    }
 return 0;
}

void Level::Stop_music()
{
 Mix_HaltMusic();
}

bool Level::Move_player_X(int _player)
{
 if(player[_player].Get_velocityX()==0)
    return false;
 bool move_possible=true;
 int x=player[_player].Get_map_positionX(),y=player[_player].Get_map_positionY();
 x+=player[_player].Get_velocityX();
 if(x<0 || x+(player[_player].Get_skinW()/40)>arena.Get_number_of_columns() || y<0 || y+(player[_player].Get_skinH()/40)>arena.Get_number_of_lines())
    return false;
 for(int _x=x;_x<=x+(player[_player].Get_skinW()/40)-1 && move_possible;_x++)
     for(int _y=y;_y<=y+(player[_player].Get_skinH()/40)-1 && move_possible;_y++)
         if((/*(_x!=x-1 && _y!=y-1) && */(arena.Is_obstacle(_y,_x) || effects.Is_obstacle(_y,_x)))/* || (player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/40<=_y+player[_player].Get_skinH()/40 && player[Other_player(_player)].Get_map_positionY()>=_y && player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinW()/40<=_x+player[_player].Get_skinW()/40 && player[Other_player(_player)].Get_map_positionX()>=_x)*/)
            move_possible=false;

 int LX=x+player[_player].Get_skinW()/40-1,LY=y+player[_player].Get_skinH()/40-1;
 int X1=player[Other_player(_player)].Get_map_positionX(),Y1=player[Other_player(_player)].Get_map_positionY();
 int LX1=X1+player[Other_player(_player)].Get_skinW()/40-1,LY1=Y1+player[Other_player(_player)].Get_skinH()/40-1;

 bool collision_x=false,collision_y=false;

 if((X1<=x && x<=LX1) || (x<=X1 && X1<=LX))
    collision_x=true;

 if((Y1<=y && y<=LY1) || (y<=Y1 && Y1<=LY))
    collision_y=true;

 if(collision_x && collision_y)
    move_possible=false;

 if(move_possible && player[_player].Get_velocityX()!=0)
    {
     player[_player].Move_X();
     player_time_blocked[_player]=3-4*player[_player].Get_movement_speed()/100;
     player[_player].Block();
     return true;
    }
 return false;
}

bool Level::Move_player_Y(int _player)
{
 if(player[_player].Get_velocityY()==0)
    return false;
 bool move_possible=true;
 int x=player[_player].Get_map_positionX(),y=player[_player].Get_map_positionY();
 y+=player[_player].Get_velocityY();
 if(x<0 || x+(player[_player].Get_skinW()/40)>arena.Get_number_of_columns() || y<0 || y+(player[_player].Get_skinH()/40)>arena.Get_number_of_lines())
    return false;
 for(int _x=x;_x<=x+(player[_player].Get_skinW()/40)-1 && move_possible;_x++)
     for(int _y=y;_y<=y+(player[_player].Get_skinH()/40)-1 && move_possible;_y++)
         if(arena.Is_obstacle(_y,_x) || effects.Is_obstacle(_y,_x))
            move_possible=false;

 int LX=x+player[_player].Get_skinW()/40-1,LY=y+player[_player].Get_skinH()/40-1;
 int X1=player[Other_player(_player)].Get_map_positionX(),Y1=player[Other_player(_player)].Get_map_positionY();
 int LX1=X1+player[Other_player(_player)].Get_skinW()/40-1,LY1=Y1+player[Other_player(_player)].Get_skinH()/40-1;

 bool collision_x=false,collision_y=false;

 if((X1<=x && x<=LX1) || (x<=X1 && X1<=LX))
    collision_x=true;

 if((Y1<=y && y<=LY1) || (y<=Y1 && Y1<=LY))
    collision_y=true;

 if(collision_x && collision_y)
    move_possible=false;

 /*if((player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/40-1<=y+player[_player].Get_skinH()/40-1 && player[Other_player(_player)].Get_map_positionY()>=y && player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinW()/40-1<=x+player[_player].Get_skinW()/40-1 && player[Other_player(_player)].Get_map_positionX()>=x))
    move_possible=false;*/
 if(move_possible && player[_player].Get_velocityY()!=0)
    {
     player[_player].Move_Y();
     player_time_blocked[_player]=3-4*player[_player].Get_movement_speed()/100;
     player[_player].Block();
     return true;
    }
 return false;
}

bool Level::Move_player(int _player)
{
 player[_player].Add_buff(arena.Get_map_texture_Buff(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()));
 player[_player].Add_buff(effects.Get_map_texture_Buff(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()));

 int x,y,x1,y1,velocityX,velocityY;
 char _map_name[TEXT_LENGHT_MAX]={NULL};
 switch(arena.Get_map_texture_type(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()))
        {
         case 2:x=player[_player].Get_map_positionX(),y=player[_player].Get_map_positionY();
                if((arena.Get_map_texture_map_name(y,x))[0]==NULL)
                   break;
                player[_player].Set_map_position(arena.Get_map_texture_player_pos_x(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()),arena.Get_map_texture_player_pos_y(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()));
                arena.Set_name(arena.Get_map_texture_map_name(y,x));
                arena.Clear();
                arena.Load();
                effects.Clear();
                effects.Set_name("Empty");
                effects.Load();
                break;
         case 3:x=player[_player].Get_map_positionX(),y=player[_player].Get_map_positionY();
                x1=arena.Get_map_texture_player_pos_x(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()),y1=arena.Get_map_texture_player_pos_y(player[_player].Get_map_positionY(),player[_player].Get_map_positionX());
                velocityX=player[_player].Get_velocityX(),velocityY=player[_player].Get_velocityY();
                strcpy(_map_name,name);
                Change(arena.Get_map_texture_map_name(y,x));
                player[_player].Set_map_position(y1,x1);
                if(type==2)
                   {
                    Start(_screen);
                    Change(_map_name);
                    player[_player].Set_map_position(x-velocityX,y-velocityY);
                    Change_music(1);
                   }
                break;
        }

 if(player[_player].Is_blocked())
    return false;
 bool player_moved_x=Move_player_X(_player),player_moved_y=Move_player_Y(_player);
 player_moved_x=player_moved_x || player_moved_y;
 player[_player].Update_skin_image_position();
 return player_moved_x;
}

void Level::Move_all_players()
{
 bool first=Move_player(1),second=false;
 if(type==2)
    second=Move_player(2);
 if(first)
    Trigger_player_map(1);
 if(second)
    Trigger_player_map(2);
}

void Level::Block_all_players()
{
 Block_player(1);
 Block_player(2);
}

void Level::Block_player(int _player)
{
 player[_player].Block();
}

void Level::Unblock_all_players()
{
 Unblock_player(1);
 Unblock_player(2);
}

void Level::Unblock_player(int _player)
{
 player[_player].Unblock();
}

bool Level::Players_can_attack(int _player)
{
 if(((player[_player].Get_map_positionX()>=player[Other_player(_player)].Get_map_positionX() &&
    player[_player].Get_map_positionX()<=player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinH()/40) ||
    (player[_player].Get_map_positionX()+player[_player].Get_skinH()/40>=player[Other_player(_player)].Get_map_positionX() &&
    player[_player].Get_map_positionX()+player[_player].Get_skinH()/40<=player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinH()/40)) &&
    ((player[_player].Get_map_positionY()>=player[Other_player(_player)].Get_map_positionY() &&
    player[_player].Get_map_positionY()<=player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/40) ||
    (player[_player].Get_map_positionY()+player[_player].Get_skinH()/40>=player[Other_player(_player)].Get_map_positionY() &&
    player[_player].Get_map_positionY()+player[_player].Get_skinH()/40<=player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/40)))
    return true;
 return false;
}

void Level::Player_basic_attack(int _player)
{
 if(Players_can_attack(_player))
    {
     player[_player].Set_velocityX(player[_player].Get_map_positionX()>player[Other_player(_player)].Get_map_positionX()?-1:1);
     player[Other_player(_player)].Set_velocityX(player[Other_player(_player)].Get_map_positionX()>player[_player].Get_map_positionX()?-1:1);

     player[_player].Set_hp(std::min(1000,player[_player].Get_hp()+player[_player].Get_life_steal()*((std::max(player[_player].Get_attack()-player[Other_player(_player)].Get_defense()*3/8,50))/10)/100));
     player[Other_player(_player)].Set_hp(player[Other_player(_player)].Get_hp()-(std::max(player[_player].Get_attack()-player[Other_player(_player)].Get_defense()*3/8,50))/10);
     player[_player].Block_attack(),player_time_blocked_attack[_player]=7-50*player[_player].Get_movement_speed()/100;
     if(player_time_blocked_attack[_player]<=0)
        player_time_blocked_attack[_player]=4;
     std::vector<Buff> _buffs[3];
     for(int i=0;i<3;i++)
         _buffs[i].clear();
     player[_player].Get_transmitted_buffs(&_buffs[_player]);
     for(std::vector<Buff>::iterator i=_buffs[_player].begin();i!=_buffs[_player].end();i++)
         {
          if((*i).Get_id()!=0)
             player[Other_player(_player)].Add_buff(*i);
         }
     #ifdef AUDIO
     Mix_PlayChannel(3,DUEL_MODE_hit[rand()%3],0);
     #endif // AUDIO
    }
}

void Level::Player_time_blocked_decrease(int _player)
{
 if(player[_player].Is_blocked())
    player_time_blocked[_player]--;
 if(player_time_blocked[_player]<=0)
    player[_player].Unblock();

 if(!player[_player].Can_attack())
    player_time_blocked_attack[_player]--;
 if(player_time_blocked_attack[_player]<=0)
    player[_player].Unblock_attack();
}

void Level::Players_time_pass()
{
 Player_time_blocked_decrease(1);
 Player_time_blocked_decrease(2);
 Decrease_all_Spells_time_blocked(1);
 Decrease_all_Spells_time_blocked(2);
}

bool Level::Player_is_on_light(int _player)
{
 return arena.Is_light(player[_player].Get_map_positionY(),player[_player].Get_map_positionX());
}

void Level::Print_Map(int x,int y,SDL_Surface *_screen)
{
 arena.Print_background(x,y,_screen,true,false);
 arena.Print_background_Animations(x,y,_screen,true,false);
 arena.Print(x,y,_screen,true,false);
 arena.Print_Animations(x,y,_screen,true,false);
 player[1].Print_skin(x,y,_screen);
 player[2].Print_skin(x,y,_screen);
 arena.Print_background(x,y,_screen,false);
 arena.Print_background_Animations(x,y,_screen,false);
 arena.Print(x,y,_screen,false);
 arena.Print_Animations(x,y,_screen,false);
 darkness.Enshroud(arena_size,_screen);
 arena.Print_background(x,y,_screen,true,true);
 arena.Print_background_Animations(x,y,_screen,true,true);
 arena.Print(x,y,_screen,true,true);
 arena.Print_Animations(x,y,_screen,true,true);
 if(Player_is_on_light(1))
    player[1].Print_skin(x,y,_screen);
 if(type==2 && Player_is_on_light(2))
    player[2].Print_skin(x,y,_screen);
 arena.Print_background(x,y,_screen,false,true);
 arena.Print_background_Animations(x,y,_screen,false,true);
 arena.Print(x,y,_screen,false,true);
 arena.Print_Animations(x,y,_screen,false,true);
 effects.Print_Animations(x,y,_screen,false,true);
 //effects.Print(x,y,_screen,false);
}

void Level::Print_players_informations(SDL_Surface *_screen)
{
 Print_player_information(1,_screen);
 Print_player_information(2,_screen);
}

void Level::Print_player_information(int _player,SDL_Surface *_screen)
{
 player[_player].Print_name(_screen);
 player[_player].Print_hp(_screen);
 if(player_type[_player]<1)
    {
     player[_player].Print_mana(_screen);
     player[_player].Print_buffs((player[_player].Get_PLAYER_INFO_POSX()+player[_player].Get_PLAYER_INFO_LAST_POSX())/2-85,120,_screen);
     player[_player].Print_items((player[_player].Get_PLAYER_INFO_POSX()+player[_player].Get_PLAYER_INFO_LAST_POSX())/2-85,160,_screen);
     player[_player].Print_Inventory_equipped_items(player[_player].Get_PLAYER_INFO_POSX(),120+100*5,_screen,false);
    }
}

void Level::Handle_Event(int _player)
{
 if(player_type[_player]>1)
    return;
 int keys=_player;
 if(player_type[1]==0 && player_type[2]==0)
    keys=Other_player(_player);

 //Spells
 for(int i=1;i<=4 && type==2;i++)
     {
      if(keystates[player_keys[keys][i+6]] && !player[_player].Spell_Is_blocked(i-1)/*&& !player[_player].Is_blocked()*/)
         {
          if(Cast_Spell(_player,i-1))
             player[_player].Block(),player_time_blocked[_player]=10;
         }
     }

 if(keystates[player_keys[keys][0]])
    Set_player_velocityY(_player,-1);
 if(keystates[player_keys[keys][1]])
    Set_player_velocityY(_player,1);
 if(keystates[player_keys[keys][2]])
    Set_player_velocityX(_player,-1);
 if(keystates[player_keys[keys][3]])
    Set_player_velocityX(_player,1);
 if(type==1)
    {
     if(keystates[player_keys[keys][4]] && !player[_player].Is_blocked())
        {
         Trigger_around_player_map(_player);
         player_time_blocked[_player]=10;
        }
    }
 else
    {
     if(keystates[player_keys[keys][4]] && player[_player].Can_attack())
        Player_basic_attack(_player);
     if(keystates[player_keys[keys][5]] && !player[_player].Is_blocked())
        player[_player].Use_hp_potion(),player_time_blocked[_player]=10;
     if(keystates[player_keys[keys][6]] && !player[_player].Is_blocked())
        player[_player].Use_mana_potion(),player_time_blocked[_player]=10;
    }

 if(player_time_blocked[_player]!=0)
    player[_player].Block();
}

void Level::Handle_Events(SDL_Surface *_screen)
{
 Set_player_velocityY(1,0);
 Set_player_velocityX(1,0);
 Set_player_velocityY(2,0);
 Set_player_velocityX(2,0);
 Handle_Event(1);
 Handle_Event(2);

 if(keystates[SDLK_EQUALS])
    Darkness_increase();
 if(keystates[SDLK_MINUS])
    Darkness_decrease();
 if((keystates[SDLK_RALT] || keystates[SDLK_LALT]) && keystates[SDLK_RETURN])
    {
     DISPLAY_MODE=(DISPLAY_MODE==SDL_FULLSCREEN)?(SDL_HWSURFACE):(SDL_FULLSCREEN);
     _screen=SDL_SetVideoMode(RESOLUTION_X,RESOLUTION_Y,32,DISPLAY_MODE);
    }
 if(keystates[SDLK_ESCAPE])
    {
     Pause_Menu();
    }
}

void Level::Darkness_increase()
{
 darkness.Increase();
}

void Level::Darkness_decrease()
{
 darkness.Decrease();
}

void Level::Apply_player_buffs(int _player)
{
 player[_player].Apply_all_buffs();
}

void Level::Apply_all_players_buffs()
{
 Apply_player_buffs(1);
 Apply_player_buffs(2);
}

bool Level::Cast_Spell(int _player,int spell_pos)
{
 if(!player[_player].Pay_Spell(spell_pos))
    return false;
 player[_player].Block_Spell(spell_pos);
 Spell _spell=player[_player].Get_Spell(spell_pos);
 std::vector<Buff> _buffs;
 _spell.Get_Buffs(&_buffs);
 ///Spell effect
 if((_spell.Get_map_name())[0]!=NULL)
    {
     spell_effect.Set_name(_spell.Get_map_name());
     spell_effect.Load();
     effects.Copy(player[_player].Get_map_positionY()-_spell.Get_range(),player[_player].Get_map_positionX()-_spell.Get_range(),&spell_effect);
     spell_effect.Clear();
    }
 switch(_spell.Get_type())
        {
         ///Self-buff
         case 1:
                for(std::vector<Buff>::iterator i=_buffs.begin();i!=_buffs.end();i++)
                    {
                     player[_player].Add_buff(*i);
                     player[_player].Apply_new_active_buff();
                    }
                break;
         ///Curse
         case 2:if(std::max(std::abs(player[_player].Get_map_positionX()-player[Other_player(_player)].Get_map_positionX()),std::abs(player[_player].Get_map_positionY()-player[Other_player(_player)].Get_map_positionY()))>_spell.Get_range())
                   break;
                for(std::vector<Buff>::iterator i=_buffs.begin();i!=_buffs.end();i++)
                    {
                     player[Other_player(_player)].Add_buff(*i);
                     player[Other_player(_player)].Apply_new_active_buff();
                    }
                break;
         ///Map modifier
         case 3://effects.Copy(player[_player].Get_map_positionX(),player[_player].Get_map_positionY(),spell_effect);
                break;
         ///Flash
         case 4:player[_player].Set_map_position(player[_player].Get_map_positionX()+player[_player].Get_velocityX()*_spell.Get_range(),player[_player].Get_map_positionY()+player[_player].Get_velocityY()*_spell.Get_range());
                if(player[_player].Get_map_positionX()<0)
                   player[_player].Set_map_position(0,player[_player].Get_map_positionY());
                if(player[_player].Get_map_positionX()>=arena.Get_number_of_columns())
                   player[_player].Set_map_position(arena.Get_number_of_columns()-1,player[_player].Get_map_positionY());
                if(player[_player].Get_map_positionY()<0)
                   player[_player].Set_map_position(player[_player].Get_map_positionX(),0);
                if(player[_player].Get_map_positionY()>=arena.Get_number_of_lines())
                   player[_player].Set_map_position(player[_player].Get_map_positionX(),arena.Get_number_of_lines()-1);
                while(arena.Is_obstacle(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()) || effects.Is_obstacle(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()) || (player[Other_player(_player)].Get_map_positionX()==player[_player].Get_map_positionX() && player[Other_player(_player)].Get_map_positionY()==player[_player].Get_map_positionY()))
                      player[_player].Set_map_position(player[_player].Get_map_positionX()-player[_player].Get_velocityX(),player[_player].Get_map_positionY()-player[_player].Get_velocityY());
                break;
         default:break;
        }
 _buffs.clear();
 return true;
}

void Level::Decrease_all_Spells_time_blocked(int _player)
{
 player[_player].Decrease_all_Spells_time_blocked();
}

//Interaction with map
void Level::Trigger_player_map(int _player)
{
 if(arena.Get_type(player[_player].Get_map_positionY(),player[_player].Get_map_positionX())!=1)
    return;
 arena.Trigger(player[_player].Get_map_positionY(),player[_player].Get_map_positionX());
}

void Level::Trigger_all_players_map()
{
 Trigger_player_map(1);
 Trigger_player_map(2);
}

void Level::Trigger_around_player_map(int _player)
{
 int dirx[]={1,0,-1,0,1,1,-1,-1};
 int diry[]={0,1,0,-1,1,-1,1,-1};
 int x,y;
 for(int i=0;i<8;i++)
     {
      x=player[_player].Get_map_positionX()+dirx[i];
      y=player[_player].Get_map_positionY()+diry[i];
      if(x<0 || x>arena.Get_number_of_columns() || y<0 || y>arena.Get_number_of_lines())
         continue;
      if(arena.Get_type(y,x)!=1)
         continue;
      arena.Trigger(y,x);
     }
}

//Start
void Level::Pause_Menu()
{
 Menu menu;
 if(type==1)
    {
     menu.Load("menu/pause_menu.pwm");
     int option=0;
     while(option!=-2)
           {
            option=menu.Start(_screen);
            switch(option)
                   {
                    case 0:option=-2;
                           break;
                    case 1:option=-2;
                           done=true;
                           break;
                    case 2:exit(0);
                   }
           }
     SDL_Delay(100);
    }
 if(type==2)
    {
     Pause_music();
     menu.Load("menu/pause_menu.pwm");
     int option=0;
     while(option!=-2)
           {
            option=menu.Start(_screen);
            switch(option)
                   {
                    case -2:Unpause_music();
                            break;
                    case 0:Unpause_music();
                           option=-2;
                           break;
                    case 1:option=-2;
                           done=true;
                           break;
                    case 2:exit(0);
                   }
           }
     SDL_Delay(100);
    }
 menu.Clear();
}

void Level::Duel_Mode_Finish_Screen(int _player_winner)
{
 bool quit=false;
 Print_Duel_Mode_Finish_Screen(_player_winner);
 SDL_Event event;
 SDL_Delay(500);
 while(SDL_PollEvent(&event));
 while(!quit)
       {
        SDL_PollEvent(&event);
        if(event.type==SDL_KEYDOWN)
           quit=true;
        SDL_Delay(100);
       }
 quit=false;
}

void Level::Print_Duel_Mode_Finish_Screen(int _player_winner)
{
 apply_surface(0,0,LEVEL_background_image,_screen);
 if(_player_winner==1)
    {
     apply_surface((_screen->w/2-LEVEL_WINNER->w)/2,5,LEVEL_WINNER,_screen);
     player[1].Print_name((_screen->w/2-LEVEL_WINNER->w)/2,5+LEVEL_WINNER->h,_screen);
     apply_surface(_screen->w/2+(_screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,_screen);
     player[2].Print_name(_screen->w/2+(_screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,_screen);
    }
 if(_player_winner==2)
    {
     apply_surface((_screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,_screen);
     player[1].Print_name((_screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,_screen);
     apply_surface(_screen->w/2+(_screen->w/2-LEVEL_WINNER->w)/2,5,LEVEL_WINNER,_screen);
      player[2].Print_name(_screen->w/2+(_screen->w/2-LEVEL_WINNER->w)/2,5+LEVEL_WINNER->h,_screen);
    }
 if(_player_winner==0)
    {
     apply_surface((_screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,_screen);
     player[1].Print_name((_screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,_screen);
     apply_surface(_screen->w/2+(_screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,_screen);
     player[2].Print_name(_screen->w/2+(_screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,_screen);
    }
 SDL_Surface *player_xp,*player_money,*player_xp_gain,*player_money_gain;
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Color xp_color={75,0,130},MONEY_COLOR={236,242,4};
 char aux[TEXT_LENGHT_MAX]={NULL};

 itoa(player[1].Get_experience(),aux);
 strcat(aux,"  +  ");
 player_xp=TTF_RenderText_Solid(font,aux,xp_color);

 itoa(20*player[2].Get_experience()/100+200,aux);
 player_xp_gain=TTF_RenderText_Solid(font,aux,xp_color);

 apply_surface((_screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2,_screen->h/2-LEVEL_XP->h,LEVEL_XP,_screen);
 apply_surface((_screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w,_screen->h/2-LEVEL_XP->h,player_xp,_screen);
 apply_surface((_screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w+player_xp->w,_screen->h/2-LEVEL_XP->h,player_xp_gain,_screen);
 SDL_FreeSurface(player_xp);
 SDL_FreeSurface(player_xp_gain);

 itoa(player[1].Get_money(),aux);
 strcat(aux,"  +  ");
 player_money=TTF_RenderText_Solid(font,aux,MONEY_COLOR);

 itoa((10+player[1].Get_extra_money())*player[2].Get_experience()/100+200,aux);
 player_money_gain=TTF_RenderText_Solid(font,aux,MONEY_COLOR);

 apply_surface((_screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2,_screen->h/2+LEVEL_MONEY->h,LEVEL_MONEY,_screen);
 apply_surface((_screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w,_screen->h/2+LEVEL_MONEY->h,player_money,_screen);
 apply_surface((_screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w+player_money->w,_screen->h/2+LEVEL_MONEY->h,player_money_gain,_screen);
 SDL_FreeSurface(player_money);
 SDL_FreeSurface(player_money_gain);

 itoa(player[2].Get_experience(),aux);
 strcat(aux,"  +  ");
 player_xp=TTF_RenderText_Solid(font,aux,xp_color);

 itoa(20*player[1].Get_experience()/100+200,aux);
 player_xp_gain=TTF_RenderText_Solid(font,aux,xp_color);

 apply_surface(_screen->w/2+(_screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2,_screen->h/2-LEVEL_XP->h,LEVEL_XP,_screen);
 apply_surface(_screen->w/2+(_screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w,_screen->h/2-LEVEL_XP->h,player_xp,_screen);
 apply_surface(_screen->w/2+(_screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w+player_xp->w,_screen->h/2-LEVEL_XP->h,player_xp_gain,_screen);
 SDL_FreeSurface(player_xp);
 SDL_FreeSurface(player_xp_gain);

 itoa(player[2].Get_money(),aux);
 strcat(aux,"  +  ");
 player_money=TTF_RenderText_Solid(font,aux,MONEY_COLOR);

 itoa((10+player[2].Get_extra_money())*player[1].Get_experience()/100+200,aux);
 player_money_gain=TTF_RenderText_Solid(font,aux,MONEY_COLOR);

 apply_surface(_screen->w/2+(_screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2,_screen->h/2+LEVEL_MONEY->h,LEVEL_MONEY,_screen);
 apply_surface(_screen->w/2+(_screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w,_screen->h/2+LEVEL_MONEY->h,player_money,_screen);
 apply_surface(_screen->w/2+(_screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w+player_money->w,_screen->h/2+LEVEL_MONEY->h,player_money_gain,_screen);
 SDL_FreeSurface(player_money);
 SDL_FreeSurface(player_money_gain);


 player[1].Set_money(player[1].Get_money()+(10+player[1].Get_extra_money())*player[2].Get_experience()/100+200);
 player[2].Set_money(player[2].Get_money()+(10+player[2].Get_extra_money())*player[1].Get_experience()/100+200);
 int a=player[1].Get_experience(),b=player[2].Get_experience();
 player[1].Set_experience(player[1].Get_experience()+20*b/100+200);
 player[2].Set_experience(player[2].Get_experience()+20*a/100+200);

 player_money=TTF_RenderText_Solid(font,"Press any key to continue!",{255,255,255});
 apply_surface((_screen->w-player_money->w)/2,_screen->h/2+(_screen->h/2+LEVEL_MONEY->h+LEVEL_XP->h)/2,player_money,_screen);

 TTF_CloseFont(font);
 SDL_Flip(_screen);
}

void Level::Set_screen(SDL_Surface *screen)
{
 _screen=screen;
 static_screen=screen;
}

void Level::Setup(char *_level_name)
{
 Set_arena_size();
 Set_name(_level_name);
 SDL_Thread *_loading_image=NULL;
 _loading_image=SDL_CreateThread(Loading_image,NULL);
 Load();
 //SDL_Delay(1000);
 SDL_KillThread(_loading_image);
 player_time_blocked[1]=player_time_blocked[2]=0;
 player[1].Unblock();
 player[2].Unblock();
 if(type==1)
    return;
 level_duration.start();
 Set_player_POSX(1,((RESOLUTION_X-840)/2)/2-100);
 Set_player_LAST_POSX(1,((RESOLUTION_X-840)/2)/2+100);
 Set_player_POSX(2,(RESOLUTION_X+(RESOLUTION_X-840)/2+840)/2-100);
 Set_player_LAST_POSX(2,(RESOLUTION_X+(RESOLUTION_X-840)/2+840)/2+100);
}

void Level::Start(SDL_Surface *screen)
{
 done=false;
 _screen=screen;
 static_screen=screen;
 bool quit=false;
 int frame=0;
 Timer fps,arena,buffs,current_time,music_time;
 arena.start();
 buffs.start();
 level_duration.start();
 current_time.start();
 music_time.start();
 srand(time(NULL));
 #ifdef AUDIO
 Change_music(1);
 //Start_music();
 #endif // AUDIO
 int previous_time=current_time.get_ticks(),lag=0;
 while(!quit && !done)
       {
        fps.start();
        if(!Mix_PlayingMusic() && music_time.get_ticks()>MUSIC_PAUSE)
           {
            Change_music(1);
            music_time.start();
           }
        int elapsed=current_time.get_ticks()-previous_time;
        previous_time=current_time.get_ticks();
        lag+=elapsed;
        SDL_PumpEvents();
        quit=keystates[SDL_QUIT] || ((keystates[SDLK_RALT] || keystates[SDLK_LALT]) && keystates[SDLK_F4]);
        Handle_Events(screen);
        while(lag>MS_PER_UPDATE && !done)
              {
               Players_time_pass();
               Move_all_players();
               if(arena.get_ticks()>1000/ARENA_FRAMES_PER_SECOND)
                  {
                   //SDL_Flip(screen);
                   Update_all_arena_frames();
                   darkness.Update_frame();
                   arena.start();
                  }
               if(buffs.get_ticks()>=1000 && type==2)
                  {
                   Apply_all_players_buffs();
                   buffs.start();
                  }
               lag-=MS_PER_UPDATE;
              }
        //Update_all_arena_frames();
        apply_surface(0,0,LEVEL_background_image,screen);
        if(type==2)
           Print_players_informations(screen);
        Print_Map((RESOLUTION_X-840)/2,40,screen);
        if(!done)
           SDL_Flip(screen);
        /*if(arena.get_ticks()>1000/ARENA_FRAMES_PER_SECOND)
           {
            //SDL_Flip(screen);
            Update_all_arena_frames();
            arena.start();
           }
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND)
           {
            SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
           }*/
        if(type==2 && level_duration.get_ticks()>duration)
           quit=true;
        if(type==2 && (player[1].Get_hp()<=0 || player[2].Get_hp()<=0))
           quit=true;
       }
 if(quit)
    {
     Pause_music();
     if(player[1].Get_hp()<=0 && player[2].Get_hp()<=0)
        Duel_Mode_Finish_Screen(0);
     else
        Duel_Mode_Finish_Screen(player[1].Get_hp()<=0?2:1);
    }
 for(int _player_pos=1;_player_pos<=2;_player_pos++)
     {
      player[_player_pos].Update();
     }
 Clear();
}

int Other_player(int _player)
{
 return _player==1?2:1;
}
