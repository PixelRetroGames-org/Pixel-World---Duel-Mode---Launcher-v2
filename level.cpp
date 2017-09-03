#include "level.h"

const SDL_Keycode player_keys[3][20]={{},{SDL_SCANCODE_UP,SDL_SCANCODE_DOWN,SDL_SCANCODE_LEFT,SDL_SCANCODE_RIGHT,SDL_SCANCODE_RCTRL,SDL_SCANCODE_J,SDL_SCANCODE_N,SDL_SCANCODE_U,SDL_SCANCODE_I,SDL_SCANCODE_O,SDL_SCANCODE_P,SDL_SCANCODE_RSHIFT,SDL_SCANCODE_H},{SDL_SCANCODE_W,SDL_SCANCODE_S,SDL_SCANCODE_A,SDL_SCANCODE_D,SDL_SCANCODE_Z,SDL_SCANCODE_GRAVE,SDL_SCANCODE_TAB,SDL_SCANCODE_1,SDL_SCANCODE_2,SDL_SCANCODE_3,SDL_SCANCODE_4,SDL_SCANCODE_X,SDL_SCANCODE_H}};
const int SKEPTIC_VISION_MAX_ALPHA=100,SKEPTIC_VISION_TIME_PER_FRAME=40;
const int LEVEL_NAME_IMAGE_TIME_PER_FRAME=30;

#define PRESENTATION_MODE
#define GOD_POWERS

#ifdef PRESENTATION_MODE
bool save_slot_used=false;
#endif // PRESENTATION_MODE

#ifdef GOD_POWERS
bool OBSTACLES=true;
#endif // GOD_POWERS

int level_number_of_background_music_tracks;
Mix_Music *level_background_music[NUMBER_OF_SONGS_MAX];
Timer level_music_time;
bool level_paused_music;
int level_last_track_played;
SDL_Thread *level_music_overseer=NULL;
SDL_mutex *music_overseer_mutex;

Timer _controller_timer;
const int CONTROLLER_DELAY=200;

Level::Level()
{
 player_name[0][0]=player_name[1][0]=player_name[2][0]=NULL;
 arena_size.x=(RESOLUTION_W-840)/2,arena_size.y=40;
 arena_size.w=860;
 arena_size.h=680;
 spell_effects.reserve(15);
 music_overseer_mutex=SDL_CreateMutex();
}

void Level::Clear(bool terminal)
{
 player[1].Clear(true);
 player[1].Set_id(0);
 if(type==2)
    {
     player[2].Clear(true);
     player[1].Set_id(1);
    }
 arena.Clear(true,true);
 effects.Clear(true,false);
 darkness.Clear();
 //aux.Clear();
 for(int i=0;i<spell_effects.size();i++)
     spell_effects[i].Clear(true,true);
 std::vector<Map>().swap(spell_effects);
 spell_effects_ids.clear();
 for(int i=0;i<number_of_non_playable_characters;i++)
     non_playable_characters[i].Clear();
 number_of_non_playable_characters=0;
 skeptic_vision_alpha=0;
 skeptic_vision_on=false;
 Set_Texture_Alpha(SKEPTIC_VISION_image,skeptic_vision_alpha);
 if(terminal)
    {
     for(int i=0;i<level_number_of_background_music_tracks;i++)
         {
          Mix_FreeMusic(level_background_music[i]);
         }
     SDL_DestroyMutex(music_overseer_mutex);
     level_last_track_played=-1;
     terrain_type=-1;
    }
}

void Level::Set_arena_size()
{
 arena_size.w=std::min(PIXELS_PER_INGAME_UNIT*arena.Get_number_of_columns(),840);
 arena_size.h=std::min(PIXELS_PER_INGAME_UNIT*arena.Get_number_of_lines(),680);
 arena_size.x=(RESOLUTION_W-arena_size.w+(arena_size.w%(2*PIXELS_PER_INGAME_UNIT)==0?-PIXELS_PER_INGAME_UNIT:0))/2;
 if(arena_size.h==680)
    arena_size.y=PIXELS_PER_INGAME_UNIT;
 else
    arena_size.y=(RESOLUTION_H-arena_size.h+(arena_size.h%(2*PIXELS_PER_INGAME_UNIT)==0?-PIXELS_PER_INGAME_UNIT:0))/2-4;
}

void Level::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Level::Set_player_map_position(int x,int y,int _player)
{
 player_map_position[_player].first=x;
 player_map_position[_player].second=y;
 player[_player].Set_map_position(x,y);
}

void Level::Set_player_mana(double _mana,int _player)
{
 player[_player].Set_mana(_mana);
}

void Level::Set_player_hp(double _hp,int _player)
{
 player[_player].Set_hp(_hp);
}

void Level::Set_player_mental_health(double _mental_health,int _player)
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

int Level::Get_player_map_position_x(int _player)
{
 return player[_player].Get_map_positionX();
}

int Level::Get_player_map_position_y(int _player)
{
 return player[_player].Get_map_positionY();
}

char *Level::Get_name()
{
 return name;
}

int Level::Get_terrain_type()
{
 return terrain_type;
}

void Level::Load()
{
 _controller_timer.start();
 done=false;
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"levels/");
 strcat(path,name);
 strcat(path,".pwl");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    return;
 int x,y;
 last_terrain_type=terrain_type;
 fscanf(where,"%d %d %d ",&type,&terrain_type,&duration);

 fgets(player_name[1],sizeof player_name[1],where);
 if(player_name[1][strlen(player_name[1])-1]=='\n')
    player_name[1][strlen(player_name[1])-1]=NULL;
 player[1].Set_name(player_name[1]);
 player[1].Load();
 player[1].Set_id(1);
 if(type!=2)
    player[1].Set_movement_speed(2);

 fscanf(where,"%d %d %d ",&player_map_position[1].first,&player_map_position[1].second,&player_type[1]);
 player[1].Set_map_position(player_map_position[1].first,player_map_position[1].second);

 player[2].Set_map_position(-5,-5);

 if(type==2)
    {
     for(int i=0;i<player[1].Get_number_of_spells();i++)
         {
          if(!spell_effects_ids.count((player[1].Get_Spell(i)).Get_id()) && ((player[1].Get_Spell(i)).Get_map_name())[0]!=NULL)
             {
              aux.Set_name((player[1].Get_Spell(i)).Get_map_name());
              aux.Load(NULL);
              spell_effects.push_back(aux);
              spell_effects_ids[(player[1].Get_Spell(i)).Get_id()]=spell_effects.size()-1;
              aux.Clear(false);
              #ifdef DEBUG
              fprintf(stderr,"1:%s\n",spell_effects[spell_effects_ids[(player[1].Get_Spell(i)).Get_id()]].Get_name());
              #endif // DEBUG
             }
         }

     fgets(player_name[2],sizeof player_name[2],where);
     if(player_name[2][strlen(player_name[2])-1]=='\n')
        player_name[2][strlen(player_name[2])-1]=NULL;

     fscanf(where,"%d %d %d ",&player_map_position[2].first,&player_map_position[2].second,&player_type[2]);
     player[2].Set_name(player_name[2]);
     player[2].Set_map_position(player_map_position[2].first,player_map_position[2].second);
     player[2].Load();
     player[2].Set_id(2);

     for(int i=0;i<player[2].Get_number_of_spells();i++)
         {
          if(!spell_effects_ids.count((player[2].Get_Spell(i)).Get_id()) && ((player[2].Get_Spell(i)).Get_map_name())[0]!=NULL)
             {
              aux.Set_name((player[2].Get_Spell(i)).Get_map_name());
              aux.Load(NULL);
              spell_effects.push_back(aux);
              spell_effects_ids[(player[2].Get_Spell(i)).Get_id()]=spell_effects.size()-1;
              aux.Clear(false);
              #ifdef DEBUG
              fprintf(stderr,"2:%s\n",spell_effects[spell_effects_ids[(player[2].Get_Spell(i)).Get_id()]].Get_name());
              #endif // DEBUG
             }
         }

    }

 if(type!=2)
    {
     fscanf(where,"%d ",&number_of_non_playable_characters);
     for(int i=0;i<number_of_non_playable_characters;i++)
         {
          fgets(non_playable_characters_names[i],sizeof non_playable_characters_names[i],where);
          if(non_playable_characters_names[i][strlen(non_playable_characters_names[i])-1]=='\n')
             non_playable_characters_names[i][strlen(non_playable_characters_names[i])-1]=NULL;
          non_playable_characters[i].Load(non_playable_characters_names[i],player[1].Get_keys(),{player[1].Get_map_positionX(),player[1].Get_map_positionY(),player[1].Get_skinW()/PIXELS_PER_INGAME_UNIT,player[1].Get_skinH()/PIXELS_PER_INGAME_UNIT});
         }
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
     level_last_track_played=-1;
     for(int i=0;i<level_number_of_background_music_tracks;i++)
         {
          Mix_FreeMusic(level_background_music[i]);
         }
     fscanf(where,"%d ",&level_number_of_background_music_tracks);
     for(int i=0;i<level_number_of_background_music_tracks && (terrain_type!=last_terrain_type || type==2);i++)
         {
          fgets(background_music_name,sizeof background_music_name,where);
          if(background_music_name[strlen(background_music_name)-1]=='\n')
             background_music_name[strlen(background_music_name)-1]=NULL;
          strcpy(path,"audio/");
          strcat(path,background_music_name);
          strcat(path,".mp3");
          level_background_music[i]=Mix_LoadMUS(path);
         }
    }

 fclose(where);
 arena.Set_name(arena_name);
 arena.Load(player[1].Get_keys());
 effects.Set_name("Empty");
 effects.Load(player[1].Get_keys());
}

void Level::Fast_Reload()
{
 arena.Clear(true,true);
 arena.Set_name(arena_name);
 arena.Load(player[1].Get_keys());
 for(int i=0;i<number_of_non_playable_characters;i++)
     {
      non_playable_characters[i].Clear();
     }
 for(int i=0;i<number_of_non_playable_characters;i++)
     {
      non_playable_characters[i].Load(non_playable_characters_names[i],player[1].Get_keys(),{player[1].Get_map_positionX(),player[1].Get_map_positionY(),player[1].Get_skinW()/PIXELS_PER_INGAME_UNIT,player[1].Get_skinH()/PIXELS_PER_INGAME_UNIT});
     }
}

void Level::Change(char *_level_name)
{
 level_changed=true;
 SDL_Thread *_loading_image=NULL;
 _loading_image=SDL_CreateThread(Loading_image,"Level Change Loading",NULL);
 Clear();
 int thread_return_value=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 SDL_WaitThread(_loading_image,&thread_return_value);
 Flip_Buffers(_screen);
 Setup(_level_name);
 if(DISPLAY_MODE!=SDL_WINDOW_FULLSCREEN)
    Fast_Reload();
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
 if(!MUSIC_MODULE_INIT)
    return;
 if(Mix_PlayingMusic()==0)
    Mix_PlayMusic(level_background_music[0],0);
}

void Level::Pause_music()
{
 if(!MUSIC_MODULE_INIT)
    return;
 if(Mix_PlayingMusic()==1)
    Mix_PauseMusic();
}

void Level::Unpause_music()
{
 if(!MUSIC_MODULE_INIT)
    return;
 if(Mix_PausedMusic()==1)
    Mix_ResumeMusic();
}

int Level::Change_music(bool play)
{
 srand((unsigned int)(time(NULL)));
 if(!MUSIC_MODULE_INIT)
    return 0;
 if(Mix_PlayingMusic()==0)
    {
     if(play)
        {
         int x=rand()%level_number_of_background_music_tracks;
         if(x==level_last_track_played && level_number_of_background_music_tracks!=1)
            {
             x+=rand()%(level_number_of_background_music_tracks-1);
             x%=level_number_of_background_music_tracks;
            }
         level_last_track_played=x;
         if(Mix_PlayMusic(level_background_music[x],0)==-1)
            {
             FILE *log_file=fopen("err/logs.txt","w");
             fprintf(log_file,"Mix_PlayMusic failed : %s ",SDL_GetError());
             fclose(log_file);
             char message[TEXT_LENGTH_MAX];
             strcpy(message,"Mix_PlayMusic failed : ");
             strcat(message,SDL_GetError());
             SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Music module failure",message,NULL);
             exit(-3);
            }
        }
     return 1;
    }
 return 0;
}

void Level::Stop_music()
{
 Mix_HaltMusic();
}

bool Oversee_music_quit;

int Level::Oversee_music(void *data)
{
 if(!MUSIC_MODULE_INIT)
    return -1;
 SDL_LockMutex(music_overseer_mutex);
 Oversee_music_quit=false;
 while(!Oversee_music_quit)
       {
        SDL_UnlockMutex(music_overseer_mutex);
        if(!Mix_PlayingMusic() && !level_paused_music)
           {
            level_music_time.start();
            level_paused_music=true;
           }
        if(!Mix_PlayingMusic() && level_music_time.get_ticks()>MUSIC_PAUSE)
           {
            level_paused_music=false;
            Change_music(1);
            level_music_time.start();
           }
        SDL_LockMutex(music_overseer_mutex);
       }
 SDL_UnlockMutex(music_overseer_mutex);
}

bool Level::Move_player_X(int _player)
{
 if(player[_player].Get_velocityX()==0)
    return false;
 bool move_possible=true;
 int x=player[_player].Get_map_positionX(),y=player[_player].Get_map_positionY();
 x+=player[_player].Get_velocityX();
 if(x<0 || x+(player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT)>arena.Get_number_of_columns() || y<0 || y+(player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT)>arena.Get_number_of_lines())
    return false;
 for(int _x=x;_x<=x+(player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT)-1 && move_possible;_x++)
     for(int _y=y;_y<=y+(player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT)-1 && move_possible;_y++)
         if((/*(_x!=x-1 && _y!=y-1) && */(arena.Is_obstacle(_y,_x) || effects.Is_obstacle(_y,_x)))/* || (player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT<=_y+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT && player[Other_player(_player)].Get_map_positionY()>=_y && player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinW()/PIXELS_PER_INGAME_UNIT<=_x+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT && player[Other_player(_player)].Get_map_positionX()>=_x)*/)
            {
             #ifdef GOD_POWERS
             if(!OBSTACLES)
                continue;
             #endif // GOD_POWERS
             move_possible=false;
            }
 int LX=x+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY=y+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;
 int X1=player[Other_player(_player)].Get_map_positionX(),Y1=player[Other_player(_player)].Get_map_positionY();
 int LX1=X1+player[Other_player(_player)].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY1=Y1+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;

 bool collision_x=false,collision_y=false;

 if((X1<=x && x<=LX1) || (x<=X1 && X1<=LX))
    collision_x=true;

 if((Y1<=y && y<=LY1) || (y<=Y1 && Y1<=LY))
    collision_y=true;

 if(collision_x && collision_y)
    move_possible=false;

 for(int i=0;i<number_of_non_playable_characters && move_possible;i++)
     {
      LX=x+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY=y+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;
      X1=non_playable_characters[i].Get_map_positionX(),Y1=non_playable_characters[i].Get_map_positionY();
      LX1=X1+non_playable_characters[i].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY1=Y1+non_playable_characters[i].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;

      collision_x=collision_y=false;

      if((X1<=x && x<=LX1) || (x<=X1 && X1<=LX))
         collision_x=true;

      if((Y1<=y && y<=LY1) || (y<=Y1 && Y1<=LY))
         collision_y=true;

      if(collision_x && collision_y)
         move_possible=false;
     }

 if(move_possible && player[_player].Get_velocityX()!=0)
    {
     player[_player].Move_X();
     player_time_blocked[_player]=std::max(5-player[_player].Get_movement_speed(),2);
     if(player_type[_player]>=1)
        AI_Block_player(_player);
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
 if(x<0 || x+(player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT)>arena.Get_number_of_columns() || y<0 || y+(player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT)>arena.Get_number_of_lines())
    return false;
 for(int _x=x;_x<=x+(player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT)-1 && move_possible;_x++)
     for(int _y=y;_y<=y+(player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT)-1 && move_possible;_y++)
         if(arena.Is_obstacle(_y,_x) || effects.Is_obstacle(_y,_x))
            {
             #ifdef GOD_POWERS
             if(!OBSTACLES)
                continue;
             #endif // GOD_POWERS
             move_possible=false;
            }
 int LX=x+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY=y+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;
 int X1=player[Other_player(_player)].Get_map_positionX(),Y1=player[Other_player(_player)].Get_map_positionY();
 int LX1=X1+player[Other_player(_player)].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY1=Y1+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;

 bool collision_x=false,collision_y=false;

 if((X1<=x && x<=LX1) || (x<=X1 && X1<=LX))
    collision_x=true;

 if((Y1<=y && y<=LY1) || (y<=Y1 && Y1<=LY))
    collision_y=true;

 if(collision_x && collision_y)
    move_possible=false;

 for(int i=0;i<number_of_non_playable_characters && move_possible;i++)
     {
      LX=x+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY=y+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;
      X1=non_playable_characters[i].Get_map_positionX(),Y1=non_playable_characters[i].Get_map_positionY();
      LX1=X1+non_playable_characters[i].Get_skinW()/PIXELS_PER_INGAME_UNIT-1,LY1=Y1+non_playable_characters[i].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;

      collision_x=collision_y=false;

      if((X1<=x && x<=LX1) || (x<=X1 && X1<=LX))
         collision_x=true;

      if((Y1<=y && y<=LY1) || (y<=Y1 && Y1<=LY))
         collision_y=true;

      if(collision_x && collision_y)
         move_possible=false;
     }

 /*if((player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT-1<=y+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT-1 && player[Other_player(_player)].Get_map_positionY()>=y && player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinW()/PIXELS_PER_INGAME_UNIT-1<=x+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT-1 && player[Other_player(_player)].Get_map_positionX()>=x))
    move_possible=false;*/
 if(move_possible && player[_player].Get_velocityY()!=0)
    {
     player[_player].Move_Y();
     player_time_blocked[_player]=std::max(5-player[_player].Get_movement_speed(),2);
     if(player_type[_player]>=1)
        AI_Block_player(_player);
     player[_player].Block();
     return true;
    }
 return false;
}

bool Level::Move_player(int _player)
{
 if(level_changed)
    {
     return false;
    }

 player[_player].Add_key(arena.Get_map_texture_key_id(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()));

 if(type==2)
    {
     Buff aux;
     aux=effects.Get_map_texture_Buff(player[_player].Get_map_positionY(),player[_player].Get_map_positionX());
     for(int x=player[_player].Get_map_positionX();aux.Get_id()==0 && x<player[_player].Get_map_positionX()+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT;x++)
         {
          for(int y=player[_player].Get_map_positionY();aux.Get_id()==0 && y<player[_player].Get_map_positionY()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT;y++)
              aux=effects.Get_map_texture_Buff(y,x);
         }
     if(aux.Get_id()!=0)
        {
         aux.Set_damage(aux.Get_damage()+((aux.Get_damage()/100)*player[Other_player(_player)].Get_spell_damage()));
         player[_player].Add_buff(aux);
         aux.Clear(false);
        }
    }

 int x,y,x1,y1,velocityX,velocityY;
 char _map_name[TEXT_LENGTH_MAX]={NULL},_aux[TEXT_LENGTH_MAX]={NULL};
 x=player[_player].Get_map_positionX(),y=player[_player].Get_map_positionY();
 switch(arena.Get_map_texture_type(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()))
        {
         case 2:if((arena.Get_map_texture_map_name(y,x))[0]==NULL)
                   break;
                player[_player].Set_map_position(arena.Get_map_texture_player_pos_x(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()),arena.Get_map_texture_player_pos_y(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()));
                strcpy(_aux,arena.Get_map_texture_map_name(y,x));
                arena.Set_name(_aux);
                arena.Clear();
                arena.Load(player[1].Get_keys());
                effects.Clear();
                effects.Set_name("Empty");
                effects.Load(player[1].Get_keys());
                Save_gamemode();
                return false;
                break;
         case 3:if((arena.Get_map_texture_map_name(y,x))[0]==NULL)
                   break;
                x1=arena.Get_map_texture_player_pos_x(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()),y1=arena.Get_map_texture_player_pos_y(player[_player].Get_map_positionY(),player[_player].Get_map_positionX());
                velocityX=player[_player].Get_velocityX(),velocityY=player[_player].Get_velocityY();
                strcpy(_map_name,name);
                strcpy(_aux,arena.Get_map_texture_map_name(y,x));
                SDL_LockMutex(music_overseer_mutex);
                Oversee_music_quit=true;
                SDL_UnlockMutex(music_overseer_mutex);
                Change(_aux);
                player[_player].Set_map_position(y1,x1);
                Save_gamemode();
                if(type==2)
                   {
                    Start(_screen);
                    Change(_map_name);
                    player[_player].Set_map_position(x-velocityX,y-velocityY);
                    Save_gamemode();
                    Change_music(1);
                   }
                level_music_overseer=SDL_CreateThread(Oversee_music,"Music Overseer",NULL);
                return false;
                break;
         case 6:if(arena.Get_map_texture_player_pos_x(player[_player].Get_map_positionY(),player[_player].Get_map_positionX())==-1 ||
                   arena.Get_map_texture_player_pos_y(player[_player].Get_map_positionY(),player[_player].Get_map_positionX())==-1)
                   break;
                player[_player].Set_map_position(arena.Get_map_texture_player_pos_x(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()),arena.Get_map_texture_player_pos_y(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()));
                Block_player(_player);
                player_time_blocked[_player]=30;
                return false;
                break;
        }

 if(player[_player].Is_blocked())
    return false;
 bool player_moved_x=Move_player_X(_player),player_moved_y=Move_player_Y(_player);
 player_moved_x=player_moved_x || player_moved_y;
 player[_player].Update_skin_image_position();
 while(player[_player].Get_map_positionX()+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT>arena.Get_number_of_columns())
       player[_player].Set_map_position(player[_player].Get_map_positionX()-1,player[_player].Get_map_positionY());

 while(player[_player].Get_map_positionY()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT>arena.Get_number_of_lines())
       player[_player].Set_map_position(player[_player].Get_map_positionX(),player[_player].Get_map_positionY()-1);
 return player_moved_x;
}

void Level::Move_all_players()
{
 if(type!=2)
    {
     Move_NPC();
     Interaction_NPC_player(1);
    }
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

void Level::Move_NPC()
{
 int dirx[]={1,0,-1,0};
 int diry[]={0,1,0,-1};
 for(int i=0;i<number_of_non_playable_characters;i++)
     {
      if(non_playable_characters[i].Is_static() || non_playable_characters[i].Is_blocked())
         continue;
      int x,y;
      x=non_playable_characters[i].Get_map_positionX();
      y=non_playable_characters[i].Get_map_positionY();
      int z=rand()%100;
      int keep_direction=false;
      if(z<=non_playable_characters[i].Get_chance_to_move())
         {
          std::vector<int> possible_directions;
          for(int j=0;j<4;j++)
              {
               if(non_playable_characters[i].Is_accessible(x+dirx[j],y+diry[j]) && (
                  (player[1].Get_map_positionX()+player[1].Get_skinW()/PIXELS_PER_INGAME_UNIT-1<x+dirx[j] ||
                   player[1].Get_map_positionX()>x+dirx[j]+non_playable_characters[i].Get_skinW()/PIXELS_PER_INGAME_UNIT-1) ||
                  (player[1].Get_map_positionY()+player[1].Get_skinH()/PIXELS_PER_INGAME_UNIT-1<y+diry[j] ||
                   player[1].Get_map_positionY()>y+diry[j]+non_playable_characters[i].Get_skinH()/PIXELS_PER_INGAME_UNIT-1)))
                  {
                   possible_directions.push_back(j);
                   if(j==non_playable_characters[i].Get_last_dir())
                      keep_direction=true;
                  }
              }
          if(possible_directions.size()==0)
             continue;
          int poz=rand()%(possible_directions.size());
          if(keep_direction)
             {
              poz=non_playable_characters[i].Get_last_dir();
              x+=dirx[poz];
              y+=diry[poz];
             }
          else
             {
              x+=dirx[possible_directions[poz]];
              y+=diry[possible_directions[poz]];
             }
          non_playable_characters[i].Update_skin(possible_directions[poz]);
          non_playable_characters[i].Set_map_positionX(x);
          non_playable_characters[i].Set_map_positionY(y);
         }
      non_playable_character_time_blocked[i]=PIXELS_PER_INGAME_UNIT;
      non_playable_characters[i].Block();
     }
}

bool Level::Players_can_attack(int _player)
{
 if(((player[_player].Get_map_positionX()>=player[Other_player(_player)].Get_map_positionX() &&
    player[_player].Get_map_positionX()<=player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT) ||
    (player[_player].Get_map_positionX()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT>=player[Other_player(_player)].Get_map_positionX() &&
    player[_player].Get_map_positionX()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT<=player[Other_player(_player)].Get_map_positionX()+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT)) &&
    ((player[_player].Get_map_positionY()>=player[Other_player(_player)].Get_map_positionY() &&
    player[_player].Get_map_positionY()<=player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT) ||
    (player[_player].Get_map_positionY()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT>=player[Other_player(_player)].Get_map_positionY() &&
    player[_player].Get_map_positionY()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT<=player[Other_player(_player)].Get_map_positionY()+player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT)))
    return true;
 return false;
}

void Level::Player_basic_attack(int _player)
{
 if(Players_can_attack(_player))
    {
     player[_player].Set_velocityX(player[_player].Get_map_positionX()>player[Other_player(_player)].Get_map_positionX()?-1:1);
     if(player[_player].Get_map_positionX()==player[Other_player(_player)].Get_map_positionX())
        player[_player].Set_velocityX(0);
     player[Other_player(_player)].Set_velocityX(player[Other_player(_player)].Get_map_positionX()>player[_player].Get_map_positionX()?-1:1);
     if(player[_player].Get_map_positionX()==player[Other_player(_player)].Get_map_positionX())
        player[Other_player(_player)].Set_velocityX(0);
     player[_player].Update_skin_image_position();
     player[Other_player(_player)].Update_skin_image_position();
     player[_player].Set_velocityX(0);
     player[Other_player(_player)].Set_velocityX(0);

     player[_player].Set_hp(player[_player].Get_hp()+1.0*player[_player].Get_life_steal()*(1.0*(std::max(1.0*player[_player].Get_attack()-1.0*player[Other_player(_player)].Get_defense()*3.0/8.0,10.0))/10)/100);
     player[Other_player(_player)].Set_hp(player[Other_player(_player)].Get_hp()-1.0*(std::max(1.0*player[_player].Get_attack()-1.0*player[Other_player(_player)].Get_defense()*3.0/8.0,10.0))/10.0);
     player[_player].Block_attack(),player_time_blocked_attack[_player]=7-player[_player].Get_movement_speed();
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
 if(level_changed)
    return;
 Player_time_blocked_decrease(1);
 Player_time_blocked_decrease(2);
 Decrease_all_Spells_time_blocked(1);
 Decrease_all_Spells_time_blocked(2);
}

void Level::NPC_time_blocked_decrease()
{
 for(int i=0;i<number_of_non_playable_characters;i++)
     {
      if(non_playable_characters[i].Is_blocked())
         non_playable_character_time_blocked[i]--;
      if(non_playable_character_time_blocked[i]<=0)
         non_playable_characters[i].Unblock();
     }
}

void Level::Time_Pass()
{
 Players_time_pass();
 NPC_time_blocked_decrease();
}

bool Level::Player_is_on_light(int _player)
{
 const int dirx[]={0,0,1,0,-1,1,1,-1,-1};
 const int diry[]={0,1,0,-1,0,1,-1,1,-1};
 bool rtn=arena.Is_light(player[_player].Get_map_positionY(),player[_player].Get_map_positionX());
 for(int x=player[_player].Get_map_positionX();x<arena.Get_number_of_columns() && x<player[_player].Get_map_positionX()+(player[_player].Get_skinW())/PIXELS_PER_INGAME_UNIT;x++)
     for(int y=player[_player].Get_map_positionY();y<arena.Get_number_of_lines() && y<player[_player].Get_map_positionY()+(player[_player].Get_skinH())/PIXELS_PER_INGAME_UNIT;y++)
         for(int dir=0;dir<9;dir++)
             rtn=(rtn || arena.Is_light(y+diry[dir],x+dirx[dir]));
 return rtn;
}

bool Level::Non_Playable_Character_is_on_light(int _npc_pos)
{
 const int dirx[]={0,0,1,0,-1,1,1,-1,-1};
 const int diry[]={0,1,0,-1,0,1,-1,1,-1};
 bool rtn=arena.Is_light(non_playable_characters[_npc_pos].Get_map_positionY(),non_playable_characters[_npc_pos].Get_map_positionX());
 for(int x=non_playable_characters[_npc_pos].Get_map_positionX();x<arena.Get_number_of_columns() && x<non_playable_characters[_npc_pos].Get_map_positionX()+(non_playable_characters[_npc_pos].Get_skinW())/PIXELS_PER_INGAME_UNIT;x++)
     for(int y=non_playable_characters[_npc_pos].Get_map_positionY();y<arena.Get_number_of_lines() && y<non_playable_characters[_npc_pos].Get_map_positionY()+(non_playable_characters[_npc_pos].Get_skinH())/PIXELS_PER_INGAME_UNIT;y++)
         for(int dir=0;dir<9;dir++)
             rtn=(rtn || arena.Is_light(y+diry[dir],x+dirx[dir]));
 return rtn;
}

const int MAP_IMAGE_WEIGHT=21,MAP_IMAGE_HEIGHT=17;

void Level::Print_Map(int x,int y,Texture *_screen)
{
 int mapX=0,mapY=0;
 if(type!=2)
    {
     mapX=std::min(std::max(0,player[1].Get_map_positionX()-(MAP_IMAGE_WEIGHT-1)/2),arena.Get_number_of_columns()-(MAP_IMAGE_WEIGHT));
     mapY=std::min(std::max(0,player[1].Get_map_positionY()-(MAP_IMAGE_HEIGHT-1)/2),arena.Get_number_of_lines()-(MAP_IMAGE_HEIGHT));
     if(arena.Get_number_of_columns()<MAP_IMAGE_WEIGHT)
        mapX=-(MAP_IMAGE_WEIGHT-arena.Get_number_of_columns())/2;
     if(arena.Get_number_of_lines()<MAP_IMAGE_HEIGHT)
        mapY=-(MAP_IMAGE_HEIGHT-arena.Get_number_of_lines())/2;
    }
 arena.Print_background(x,y,mapX,mapY,_screen,true,false);
 arena.Print_background_Animations(x,y,mapX,mapY,_screen,true,false);
 arena.Print(x,y,mapX,mapY,_screen,true,false);
 arena.Print_Animations(x,y,mapX,mapY,_screen,true,false);
 if(type!=2)
    arena.Print_Clues(x,y,mapX,mapY,_screen,true,false);
 player[1].Print_skin(x,y,mapX,mapY,_screen);
 player[2].Print_skin(x,y,mapX,mapY,_screen);

 for(int i=0;i<number_of_non_playable_characters;i++)
     {
      non_playable_characters[i].Print_skin(x,y,mapX,mapY,arena_size.w,arena_size.h,_screen);
     }

 arena.Print_background(x,y,mapX,mapY,_screen,false);
 arena.Print_background_Animations(x,y,mapX,mapY,_screen,false);
 arena.Print(x,y,mapX,mapY,_screen,false);
 arena.Print_Animations(x,y,mapX,mapY,_screen,false);
 if(type!=2)
    arena.Print_Clues(x,y,mapX,mapY,_screen,false);

 SDL_Rect _area=arena_size;
 //_area.x=x;
 ///TREBUIE SCHIMBAT
 if(_area.h<MAP_IMAGE_HEIGHT*PIXELS_PER_INGAME_UNIT)
    _area.y=y+(MAP_IMAGE_HEIGHT-_area.h/PIXELS_PER_INGAME_UNIT)/2*PIXELS_PER_INGAME_UNIT;
 /*_area.h=MAP_IMAGE_WEIGHT*PIXELS_PER_INGAME_UNIT-mapY*PIXELS_PER_INGAME_UNIT;
 _area.w=MAP_IMAGE_HEIGHT*PIXELS_PER_INGAME_UNIT-mapX*PIXELS_PER_INGAME_UNIT;
 _area.x=x+mapX*PIXELS_PER_INGAME_UNIT;
 _area.y=y+mapY*PIXELS_PER_INGAME_UNIT;*/
 darkness.Enshroud(_area,_screen);

 arena.Print_background(x,y,mapX,mapY,_screen,true,true);
 arena.Print_background_Animations(x,y,mapX,mapY,_screen,true,true);
 arena.Print(x,y,mapX,mapY,_screen,true,true);
 arena.Print_Animations(x,y,mapX,mapY,_screen,true,true);
 if(type!=2)
    arena.Print_Clues(x,y,mapX,mapY,_screen,true,true);
 if(Player_is_on_light(1))
    player[1].Print_skin(x,y,mapX,mapY,_screen);
 if(type==2 && Player_is_on_light(2))
    player[2].Print_skin(x,y,mapX,mapY,_screen);
 for(int i=0;i<number_of_non_playable_characters;i++)
     {
      if(Non_Playable_Character_is_on_light(i))
         non_playable_characters[i].Print_skin(x,y,mapX,mapY,arena_size.w,arena_size.h,_screen);
     }
 arena.Print_background(x,y,mapX,mapY,_screen,false,true);
 arena.Print_background_Animations(x,y,mapX,mapY,_screen,false,true);
 arena.Print(x,y,mapX,mapY,_screen,false,true);
 arena.Print_Animations(x,y,mapX,mapY,_screen,false,true);
 if(type!=2)
    {
     arena.Print_Clues(x,y,mapX,mapY,_screen,false,true);
     if(skeptic_vision_on)
        arena.Print_Special_Clues(x,y,mapX,mapY,_screen);
     if(skeptic_vision_timer.get_ticks()>=SKEPTIC_VISION_TIME_PER_FRAME)
        {
         if(skeptic_vision_on)
            {
             skeptic_vision_alpha+=10;
             if(skeptic_vision_alpha>SKEPTIC_VISION_MAX_ALPHA)
                skeptic_vision_alpha=SKEPTIC_VISION_MAX_ALPHA;
             Set_Texture_Alpha(SKEPTIC_VISION_image,skeptic_vision_alpha);
            }
         else
            {
             if(skeptic_vision_alpha!=0)
                {
                 skeptic_vision_alpha-=10;
                 if(skeptic_vision_alpha<0)
                    skeptic_vision_alpha=0;
                 Set_Texture_Alpha(SKEPTIC_VISION_image,skeptic_vision_alpha);
                }
            }
         skeptic_vision_timer.start();
        }
     Apply_Texture(0,0,_area.x,_area.y,_area.w,_area.h,SKEPTIC_VISION_image,_screen);
    }
 if(type==2)
    effects.Print_Animations(x,y,mapX,mapY,_screen,false,true);
 //effects.Print(x,y,_screen,false);
 if(level_name_image_timer.get_ticks()>=LEVEL_NAME_IMAGE_TIME_PER_FRAME)
    {
     arena.Update_name_image();
     level_name_image_timer.start();
    }
 arena.Print_name_image(_screen);
}

void Level::Print_players_informations(Texture *_screen)
{
 Print_player_information(1,_screen);
 Print_player_information(2,_screen);
}

void Level::Print_player_information(int _player,Texture *_screen)
{
 player[_player].Print_name(_screen);
 player[_player].Print_hp(_screen);
 player[_player].Print_buffs((player[_player].Get_PLAYER_INFO_POSX()+player[_player].Get_PLAYER_INFO_LAST_POSX())/2-95,120,_screen);
 player[_player].Print_mana(_screen);
 if(player_type[_player]<1)
    {
     player[_player].Print_spells((player[_player].Get_PLAYER_INFO_POSX()+player[_player].Get_PLAYER_INFO_LAST_POSX())/2-95,160,_screen);
     player[_player].Print_items((player[_player].Get_PLAYER_INFO_POSX()+player[_player].Get_PLAYER_INFO_LAST_POSX())/2-85,200,_screen);
     player[_player].Print_Inventory_equipped_items(player[_player].Get_PLAYER_INFO_POSX(),120+100*5,_screen,false);
    }
}

void Level::Handle_Event(int _player)
{
 if(player_type[_player]>=1)
    {
     AI_Make_Move_player(_player);
     return;
    }
 int keys=_player;
 if(player_type[1]==0 && player_type[2]==0)
    keys=Other_player(_player);

 //Spells
 for(int i=1;i<=4 && type==2;i++)
     {
      if(((keystates[player_keys[keys][i+6]] || controller[_player].Get_Button_State(i)) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][i+6]] || controller[Other_player(_player)].Get_Button_State(i)))) && !player[_player].Spell_Is_blocked(i-1)/*&& !player[_player].Is_blocked()*/)
         {
          if(Cast_Spell(_player,i-1))
             player[_player].Block(),player_time_blocked[_player]=10;
         }
     }

 player[_player].Set_velocityX(0);
 player[_player].Set_velocityY(0);

 if((keystates[player_keys[keys][0]] || controller[_player].Pressed_Up()) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][0]] || controller[Other_player(_player)].Pressed_Up())))
    Set_player_velocityY(_player,-1);
 if((keystates[player_keys[keys][1]] || controller[_player].Pressed_Down()) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][1]] ||controller[Other_player(_player)].Pressed_Down())))
    Set_player_velocityY(_player,1);
 if((keystates[player_keys[keys][2]] || controller[_player].Pressed_Left()) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][2]] || controller[Other_player(_player)].Pressed_Left())))
    Set_player_velocityX(_player,-1);
 if((keystates[player_keys[keys][3]] || controller[_player].Pressed_Right()) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][3]] || controller[Other_player(_player)].Pressed_Right())))
    Set_player_velocityX(_player,1);
 if(type!=2)
    {
     if(((keystates[player_keys[keys][4]] || controller[_player].Pressed_A_button()) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][4]] || controller[Other_player(_player)].Pressed_A_button()))) && !player[_player].Is_blocked())
        {
         Trigger_around_player_map(_player);
         Interact_with_map_textures_around_player(_player);
         Interact_with_NPC_around_player(_player);
         Interact_with_clues_around_player(_player);
         player_time_blocked[_player]=10;
        }
     if((keystates[player_keys[keys][11]] || controller[_player].Pressed_B_button() || keystates[player_keys[Other_player(keys)][11]] || controller[Other_player(_player)].Pressed_B_button()) && !player[_player].Is_blocked())
        {
         skeptic_vision_on=!skeptic_vision_on,player_time_blocked[_player]=10,player[_player].Block();
         if(skeptic_vision_on)
            player[_player].Set_movement_speed(0);
         else
            player[_player].Set_movement_speed(2);
        }
     if((keystates[player_keys[keys][12]] || controller[_player].Pressed_Y_button() || keystates[player_keys[Other_player(keys)][11]] || controller[Other_player(_player)].Pressed_Y_button()) && !player[_player].Is_blocked())
        {
         SDL_Thread *Meditation_Screen_Thread=NULL;
         Meditation_Screen_Thread=SDL_CreateThread(Meditation_Screen,"Meditation Screen",NULL);
         Fast_Reload();
         int thread_return_value=0;
         SDL_WaitThread(Meditation_Screen_Thread,&thread_return_value);
         SDL_PumpEvents();
        }
    }
 else
    {
     if((AUTO_ATTACK || ((keystates[player_keys[keys][4]]) || ((player_type[Other_player(_player)]>=1 || type!=2) && keystates[player_keys[Other_player(keys)][4]]))) && player[_player].Can_attack())
        Player_basic_attack(_player);
     if(((keystates[player_keys[keys][5]] || controller[_player].Pressed_LeftShoulder()) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][5]] || controller[Other_player(_player)].Pressed_LeftShoulder()))) && !player[_player].Is_blocked())
        player[_player].Use_hp_potion(),player_time_blocked[_player]=10;
     if(((keystates[player_keys[keys][6]] || controller[_player].Pressed_RightShoulder()) || ((player_type[Other_player(_player)]>=1 || type!=2) && (keystates[player_keys[Other_player(keys)][6]] || controller[Other_player(_player)].Pressed_RightShoulder()))) && !player[_player].Is_blocked())
        player[_player].Use_mana_potion(),player_time_blocked[_player]=10;
    }

 if(player_time_blocked[_player]!=0)
    player[_player].Block();
}

void Level::Handle_Events(Texture *_screen)
{
 Handle_Event(1);
 if(type==2)
    Handle_Event(2);

 if(keystates[SDL_SCANCODE_EQUALS])
    Darkness_increase();
 if(keystates[SDL_SCANCODE_MINUS])
    Darkness_decrease();
 if(keystates[SDL_SCANCODE_ESCAPE] || ((controller[1].Pressed_Guide_button() || controller[2].Pressed_Guide_button()) && _controller_timer.get_ticks()>CONTROLLER_DELAY) || (focus && changed_window_status))
    {
     _controller_timer.start();
     Pause_Menu();
    }
 if((keystates[SDL_SCANCODE_J] || ((controller[1].Pressed_Start_button() || controller[2].Pressed_Guide_button()) && _controller_timer.get_ticks()>CONTROLLER_DELAY)) && type!=2)
    {
     _controller_timer.start();
     Open_Journal(player[1].Get_progress(),_screen);
    }
 if((keystates[SDL_SCANCODE_I] || ((controller[1].Pressed_Back_button() || controller[2].Pressed_Back_button()) && _controller_timer.get_ticks()>CONTROLLER_DELAY)) && type!=2)
    {
     _controller_timer.start();
     player_inventory::Print_Inventory(_screen,player[1].Get_name());
     player[1].Fast_Reload();
     player[1].Set_movement_speed(2);
    }
 SDL_Event event;
 SDL_PollEvent(&event);
 changed_window_status=false;
 if(((keystates[SDL_SCANCODE_RALT] || keystates[SDL_SCANCODE_LALT]) && keystates[SDL_SCANCODE_RETURN]) || ((controller[1].Pressed_LeftShoulder() && controller[1].Pressed_RightShoulder()) ||
                                                                                                           (controller[1].Pressed_LeftShoulder() && controller[1].Pressed_RightShoulder())))
    {
     if(DISPLAY_MODE==SDL_WINDOW_FULLSCREEN)
        DISPLAY_MODE=0;
     else
        DISPLAY_MODE=SDL_WINDOW_FULLSCREEN;
     SDL_SetWindowFullscreen(WINDOW,DISPLAY_MODE);
     SCREEN_SURFACE=SDL_GetWindowSurface(WINDOW);
     Fast_Reload();
     Clear_Journal();
     Load_Journal();
     SDL_PumpEvents();
     while(SDL_PollEvent(&event));
    }
 else
    {
     if(event.type==SDL_WINDOWEVENT)
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
             Fast_Reload();
             Clear_Journal();
             Load_Journal();
             SDL_PumpEvents();
            }
        }
    }
 #ifdef PRESENTATION_MODE
 if(type!=2)
    {
     char save_slot_name[TEXT_LENGTH_MAX];
     strcpy(save_slot_name,"presentation mode/save slots/");
     if(keystates[SDL_SCANCODE_F1])
        {
         strcat(save_slot_name,"F1.pwqss");
         //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Save Slot Used","F1",WINDOW);
        }
     if(keystates[SDL_SCANCODE_F2])
        {
         strcat(save_slot_name,"F2.pwqss");
        }
     if(keystates[SDL_SCANCODE_F3])
        {
         strcat(save_slot_name,"F3.pwqss");
        }
     if(keystates[SDL_SCANCODE_F4])
        {
         strcat(save_slot_name,"F4.pwqss");
        }
     if(keystates[SDL_SCANCODE_F5])
        {
         strcat(save_slot_name,"F5.pwqss");
        }
     FILE *save_slot_file=fopen(save_slot_name,"r");
     if(save_slot_file!=NULL)
        {
         char save_name[TEXT_LENGTH_MAX],path[TEXT_LENGTH_MAX];
         fgets(save_name,sizeof save_name,save_slot_file);
         int n=strlen(save_name);
         if(save_name[n-1]=='\n')
            save_name[n-1]=NULL;
         strcpy(path,save_name);
         strcat(path,"gamemodes/Story Mode.pwsav");
         Copy_files(path,"saves/gamemodes/Story Mode.pwsav");

         strcpy(path,save_name);
         strcat(path,"players/Timy.pwp");
         Copy_files(path,"saves/players/Timy.pwp");
         done=true;
         save_slot_used=true;
        }
     fclose(save_slot_file);
    }
 #else
 #ifdef GOD_POWERS
 if(type!=2)
    {
     if(keystates[SDL_SCANCODE_INSERT])
        OBSTACLES=!OBSTACLES;
     char map_name[TEXT_LENGTH_MAX]={NULL};
     if(keystates[SDL_SCANCODE_F1])
        {
         strcpy(map_name,"The Stables");
        }
     if(keystates[SDL_SCANCODE_F2])
        {
         strcpy(map_name,"The Stables Western Exit");
        }
     if(keystates[SDL_SCANCODE_F3])
        {
         strcpy(map_name,"New1");
        }
     if(keystates[SDL_SCANCODE_F4])
        {
         strcpy(map_name,"New2");
        }
     if(keystates[SDL_SCANCODE_F5])
        {
         strcpy(map_name,"New3");
        }
     if(strlen(map_name)!=0)
        {
         Change(map_name);
         reset_lag=true;
        }
    }
 if(type==2)
    {
     if(keystates[SDL_SCANCODE_END])
        player[2].Set_hp(0);
    }
 #endif // GOD_POWERS
 #endif // PRESENTATION_MODE
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

bool Segment_check_intersection(std::pair<int,int> first,std::pair<int,int> second)
{
 if(first.first>second.first)
    std::swap(first,second);
 return (second.first<=first.second);
}

bool Rect_check_intersection(SDL_Rect first,SDL_Rect second)
{
 return (Segment_check_intersection(std::make_pair(first.x,first.x+first.w),std::make_pair(second.x,second.x+second.w)) &&
         Segment_check_intersection(std::make_pair(first.y,first.y+first.h),std::make_pair(second.y,second.y+second.h)));
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
     /*spell_effect.Set_name(_spell.Get_map_name());
     spell_effect.Load(player[1].Get_keys());
     effects.Copy(player[_player].Get_map_positionY()-_spell.Get_range(),player[_player].Get_map_positionX()-_spell.Get_range(),&spell_effect);
     spell_effect.Clear();*/
     effects.Copy(player[_player].Get_map_positionY()-_spell.Get_range(),player[_player].Get_map_positionX()-_spell.Get_range(),&spell_effects[spell_effects_ids[_spell.Get_id()]]);
    }
 bool blocked_up,blocked_down,blocked_left,blocked_right;
 blocked_up=blocked_down=blocked_left=blocked_right=false;
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
         case 2:/*if(std::max(std::abs(player[_player].Get_map_positionX()-player[Other_player(_player)].Get_map_positionX()),std::abs(player[_player].Get_map_positionY()-player[Other_player(_player)].Get_map_positionY()))>_spell.Get_range())
                   break;*/
                SDL_Rect other_player_rect,spell_range_rect;

                other_player_rect.x=player[Other_player(_player)].Get_map_positionX();
                other_player_rect.y=player[Other_player(_player)].Get_map_positionY();
                other_player_rect.w=player[Other_player(_player)].Get_skinW()/PIXELS_PER_INGAME_UNIT-1;
                other_player_rect.h=player[Other_player(_player)].Get_skinH()/PIXELS_PER_INGAME_UNIT-1;

                spell_range_rect.x=player[_player].Get_map_positionX()-_spell.Get_range();
                spell_range_rect.y=player[_player].Get_map_positionY()-_spell.Get_range();
                spell_range_rect.w=2*_spell.Get_range();
                spell_range_rect.h=2*_spell.Get_range();

                if(!Rect_check_intersection(other_player_rect,spell_range_rect))
                   break;

                for(std::vector<Buff>::iterator i=_buffs.begin();i!=_buffs.end();i++)
                    {
                     i->Set_damage(i->Get_damage()+(i->Get_damage()*player[_player].Get_spell_damage()/100));
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
         ///Teleport
         case 5:player[_player].Set_map_position(player[_player].Get_map_positionX()+player[_player].Get_velocityX()*_spell.Get_range(),player[_player].Get_map_positionY()+player[_player].Get_velocityY()*_spell.Get_range());
                if(player[_player].Get_map_positionX()<0)
                   player[_player].Set_map_position(0,player[_player].Get_map_positionY()),blocked_up=true;
                if(player[_player].Get_map_positionX()>=arena.Get_number_of_columns())
                   player[_player].Set_map_position(arena.Get_number_of_columns()-1,player[_player].Get_map_positionY()),blocked_down=true;
                if(player[_player].Get_map_positionY()<0)
                   player[_player].Set_map_position(player[_player].Get_map_positionX(),0),blocked_left=true;
                if(player[_player].Get_map_positionY()>=arena.Get_number_of_lines())
                   player[_player].Set_map_position(player[_player].Get_map_positionX(),arena.Get_number_of_lines()-1),blocked_right=true;
                if((blocked_down || blocked_up) && (blocked_left || blocked_right))
                   {
                    player[_player].Set_velocityX(-player[_player].Get_velocityX());
                    player[_player].Set_velocityY(-player[_player].Get_velocityY());
                    player[_player].Set_map_position(player[_player].Get_map_positionX()+player[_player].Get_velocityX()*_spell.Get_range(),player[_player].Get_map_positionY()+player[_player].Get_velocityY()*_spell.Get_range());
                    if(player[_player].Get_map_positionX()<0)
                       player[_player].Set_map_position(0,player[_player].Get_map_positionY()),blocked_up=true;
                    if(player[_player].Get_map_positionX()>=arena.Get_number_of_columns())
                       player[_player].Set_map_position(arena.Get_number_of_columns()-1,player[_player].Get_map_positionY()),blocked_down=true;
                    if(player[_player].Get_map_positionY()<0)
                       player[_player].Set_map_position(player[_player].Get_map_positionX(),0),blocked_left=true;
                    if(player[_player].Get_map_positionY()>=arena.Get_number_of_lines())
                       player[_player].Set_map_position(player[_player].Get_map_positionX(),arena.Get_number_of_lines()-1),blocked_right=true;
                   }
                while(arena.Is_obstacle(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()) || effects.Is_obstacle(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()) || (player[Other_player(_player)].Get_map_positionX()==player[_player].Get_map_positionX() && player[Other_player(_player)].Get_map_positionY()==player[_player].Get_map_positionY()))
                      player[_player].Set_map_position(player[_player].Get_map_positionX()-player[_player].Get_velocityX(),player[_player].Get_map_positionY()-player[_player].Get_velocityY());
                break;
         default:break;
        }
 _buffs.clear();
 _spell.Play_sound_effect(4);
 return true;
}

void Level::Decrease_all_Spells_time_blocked(int _player)
{
 player[_player].Decrease_all_Spells_time_blocked();
}

//Interaction with map
void Level::Trigger_player_map(int _player)
{
 if(level_changed)
    return;
 if(arena.Get_type(player[_player].Get_map_positionY(),player[_player].Get_map_positionX())!=1)
    return;
 if(!arena.Is_Trigger(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()))
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
 for(int i=0;i<4;i++)
     {
      x=player[_player].Get_map_positionX()+dirx[i];
      y=player[_player].Get_map_positionY()+diry[i];
      if(x<0 || x>=arena.Get_number_of_columns() || y<0 || y>=arena.Get_number_of_lines())
         continue;
      if(arena.Is_Trigger(y,x))
         arena.Trigger(y,x);
      /*int x,y,x1,y1,velocityX,velocityY;
      char _map_name[TEXT_LENGTH_MAX]={NULL};
      switch(arena.Get_map_texture_type(player[_player].Get_map_positionY(),player[_player].Get_map_positionX()))
             {
              case 4:break;
             }*/
     }
}


void Level::Interact_with_map_textures_around_player(int _player)
{
 int dirx[]={1,0,-1,0};
 int diry[]={0,1,0,-1};
 int x,y;
 for(int i=0;i<4;i++)
     {
      x=player[_player].Get_map_positionX()+dirx[i];
      y=player[_player].Get_map_positionY()+diry[i];
      if(x<0 || x>=arena.Get_number_of_columns() || y<0 || y>=arena.Get_number_of_lines())
         continue;
      Interact_with_map_texture(_player,x,y);
     }
}

void Level::Interact_with_map_texture(int _player,int x,int y)
{
 Script_interpreter script_interpreter;
 Puzzle puzzle;
 char aux[TEXT_LENGTH_MAX];
 switch(arena.Get_map_texture_type(y,x))
        {
         case 4:script_interpreter.Start(_screen,"map_texture/Picklock");
                if((*player[_player].Get_keys())[arena.Get_map_texture_key_id(y,x)]==true)
                   return;
                strcpy(aux,"map_texture/");
                strcat(aux,arena.Get_map_texture_puzzle_name(y,x));
                puzzle.Set_name(aux);
                puzzle.Load();
                if(puzzle.Start(_screen))
                   {
                    player[_player].Add_key(arena.Get_map_texture_key_id(y,x));
                    player[_player].Update();
                    script_interpreter.Start(_screen,"map_texture/Picklock succeeded");
                   }
                else
                   {
                    script_interpreter.Start(_screen,"map_texture/Picklock failed");
                   }
                break;
         case 5:script_interpreter.Start(_screen,"map_texture/Picklock");
                break;
         default:return;
        }
 Fast_Reload();
 SDL_Delay(100);
 SDL_PumpEvents();
 reset_lag=true;
}

void Level::Interact_with_NPC(int _player,int _npc)
{
 if(non_playable_characters[_npc].Get_type()==0)
    return;
 int dirx[]={1,0,-1,0};
 int diry[]={0,1,0,-1};
 for(int i=0;i<4;i++)
     {
      if(player[_player].Get_map_positionX()==non_playable_characters[_npc].Get_map_positionX()+dirx[i] &&
         player[_player].Get_map_positionY()==non_playable_characters[_npc].Get_map_positionY()+diry[i])
         {
          non_playable_characters[_npc].Update_skin(i);
          non_playable_character_time_blocked[_npc]=40;
          non_playable_characters[_npc].Block();
          player[_player].Set_velocityX(-dirx[i]);
          player[_player].Set_velocityY(-diry[i]);
          player[_player].Update_skin_image_position();
         }
     }
 int thread_return_value;
 switch(non_playable_characters[_npc].Get_type())
        {
         case 1:break;
         case 2:break;
         case 4:break;
         default:SDL_LockMutex(music_overseer_mutex);
                 Oversee_music_quit=true;
                 SDL_UnlockMutex(music_overseer_mutex);
                 SDL_WaitThread(level_music_overseer,&thread_return_value);
                 SDL_Delay(50);
                 Stop_music();
                 Mix_VolumeMusic(MIX_MAX_VOLUME*std::max(0,VOLUME/2)/100);
                 Mix_PlayMusic(DUEL_MODE_START,-1);
                 break;
        }
 Script_interpreter script_interpreter;
 script_interpreter.Start(_screen,non_playable_characters[_npc].Get_script_name());
 Shop_Screen shop_screen;
 int x=player[_player].Get_map_positionX(),y=player[_player].Get_map_positionY();
 char _map_name[TEXT_LENGTH_MAX]={NULL},_aux[TEXT_LENGTH_MAX]={NULL},afterscript[TEXT_LENGTH_MAX]={NULL};
 strcpy(afterscript,non_playable_characters[_npc].Get_afterscript_name());
 Puzzle puzzle;
 std::bitset<NUMBER_OF_MAX_KEYS> npc_keys_to_take,npc_keys_to_give;
 npc_keys_to_give=(*non_playable_characters[_npc].Get_keys());
 npc_keys_to_take=(*non_playable_characters[_npc].Get_keys_to_take());
 int npc_lose_posX=non_playable_characters[_npc].Get_lose_posX(),npc_lose_posY=non_playable_characters[_npc].Get_lose_posY();
 switch(non_playable_characters[_npc].Get_type())
        {
         case 1:player[_player].Add_keys(non_playable_characters[_npc].Get_keys());
                player[_player].Remove_keys(non_playable_characters[_npc].Get_keys_to_take());
                player[_player].Update();
                break;
         case 2:player[_player].Add_keys(non_playable_characters[_npc].Get_keys());
                player[_player].Remove_keys(non_playable_characters[_npc].Get_keys_to_take());
                player[_player].Update();
                shop_screen.Start(_screen,non_playable_characters[_npc].Get_shop_name(),player[_player].Get_name(),1);
                strcpy(_aux,player[_player].Get_name());
                player[_player].Clear(true);
                player[_player].Set_name(_aux);
                player[_player].Load();
                player[_player].Set_map_position(x,y);
                if(type!=2)
                   player[_player].Set_movement_speed(2);
                SDL_Delay(100);
                SDL_PumpEvents();
                script_interpreter.Start(_screen,afterscript);
                //shop_screen.Reset();
                break;
         case 3:strcpy(_map_name,name);
                strcpy(_aux,non_playable_characters[_npc].Get_duel_mode_level_name());
                Change(_aux);
                Stop_music();
                Mix_VolumeMusic(MIX_MAX_VOLUME*VOLUME/100);
                if(type==2)
                   {
                    Start(_screen,false);
                    if(winner==1)
                       {
                        player[_player].Add_keys(&npc_keys_to_give);
                        player[_player].Remove_keys(&npc_keys_to_take);
                        player[_player].Update();
                       }
                    Cleanup();
                    Change(_map_name);
                    if(winner==1 || npc_lose_posX==-1 || npc_lose_posY==-1)
                       player[_player].Set_map_position(x,y);
                    else
                       player[_player].Set_map_position(npc_lose_posX,npc_lose_posY);
                    level_music_overseer=SDL_CreateThread(Oversee_music,"Music Overseer",NULL);
                    Change_music(1);
                    if(winner==1)
                       script_interpreter.Start(_screen,afterscript);
                   }
                else
                   level_music_overseer=SDL_CreateThread(Oversee_music,"Music Overseer",NULL);
                break;
         case 4:puzzle.Set_name(non_playable_characters[_npc].Get_puzzle_name());
                puzzle.Load();
                if(puzzle.Start(_screen))
                   {
                    player[_player].Add_keys(non_playable_characters[_npc].Get_keys());
                    player[_player].Remove_keys(non_playable_characters[_npc].Get_keys_to_take());
                    player[_player].Update();
                    script_interpreter.Start(_screen,afterscript);
                   }
                else
                   {
                    if(npc_lose_posX!=-1 && npc_lose_posY!=-1)
                       player[_player].Set_map_position(npc_lose_posX,npc_lose_posY);
                   }
                break;
        }
 Fast_Reload();
 SDL_Delay(100);
 SDL_PumpEvents();
 reset_lag=true;
}

void Level::Interact_with_NPC_around_player(int _player)
{
 int dirx[]={1,0,-1,0,2,0,-2,0};
 int diry[]={0,1,0,-1,0,2,0,-2};
 int x,y;
 for(int i=0;i<8;i++)
     {
      x=player[_player].Get_map_positionX()+dirx[i];
      y=player[_player].Get_map_positionY()+diry[i];
      if(x<0 || x>=arena.Get_number_of_columns() || y<0 || y>=arena.Get_number_of_lines())
         continue;
      for(int j=0;j<number_of_non_playable_characters;j++)
          {
           if(x>=non_playable_characters[j].Get_map_positionX() && x<non_playable_characters[j].Get_map_positionX()+non_playable_characters[j].Get_skinW()/PIXELS_PER_INGAME_UNIT && y>=non_playable_characters[j].Get_map_positionY() && y<non_playable_characters[j].Get_map_positionY()+non_playable_characters[j].Get_skinH()/PIXELS_PER_INGAME_UNIT)
              {
               Interact_with_NPC(_player,j);
               return;
              }
          }
     }
}

void Level::Interaction_NPC_player(int _player)
{
 for(int i=0;i<number_of_non_playable_characters;i++)
     {
      if(std::max(std::abs(player[_player].Get_map_positionX()-non_playable_characters[i].Get_map_positionX()),
                  std::abs(player[_player].Get_map_positionY()-non_playable_characters[i].Get_map_positionY()))<=non_playable_characters[i].Get_range())
         {
          Interact_with_NPC(_player,i);
          return;
         }
     }
}

void Level::Interact_with_clues_around_player(int _player)
{
 int dirx[]={1,0,-1,0};
 int diry[]={0,1,0,-1};
 int x,y;
 x=player[_player].Get_map_positionX();
 y=player[_player].Get_map_positionY();
 if(!(x<0 || x>=arena.Get_number_of_columns() || y<0 || y>=arena.Get_number_of_lines()))
    if(Interact_with_clue(_player,y,x))
       return;
 for(int i=0;i<4;i++)
     {
      x=player[_player].Get_map_positionX()+dirx[i];
      y=player[_player].Get_map_positionY()+diry[i];
      if(x<0 || x>=arena.Get_number_of_columns() || y<0 || y>=arena.Get_number_of_lines())
         continue;
      if(Interact_with_clue(_player,y,x))
         return;
     }
}

bool Level::Interact_with_clue(int _player,int x,int y)
{
 if(skeptic_vision_on && arena.Get_Special_Clue_map_texture(x,y)->Get_id()!=0 && arena.Get_Special_Clue_map_texture(x,y)->Get_type()!=0)
    {
     player[_player].Add_keys(arena.Get_Special_Clue_map_texture(x,y)->Get_keys());
     player[_player].Remove_keys(arena.Get_Special_Clue_map_texture(x,y)->Get_keys_to_take());
     player[_player].Update();
     arena.Get_Special_Clue_map_texture(x,y)->Start_action(_screen);
     reset_lag=true;
     if(arena.Get_Special_Clue_map_texture(x,y)->Get_type()!=0)
        Fast_Reload();
     return true;
    }
 if(((arena.Get_Clue_map_texture(x,y))->Get_id())==0)
    return false;
 player[_player].Add_keys(arena.Get_Clue_map_texture(x,y)->Get_keys());
 player[_player].Remove_keys(arena.Get_Clue_map_texture(x,y)->Get_keys_to_take());
 player[_player].Update();
 arena.Get_Clue_map_texture(x,y)->Start_action(_screen);
 reset_lag=true;
 if(arena.Get_Clue_map_texture(x,y)->Get_type()!=0)
    Fast_Reload();
 return true;
}

//Artificial Intelligence

void Level::AI_Make_Move_player(int _player)
{
 player[_player].Set_velocityX(0);
 player[_player].Set_velocityY(0);
 if(player[_player].Is_blocked())
    return;
 int range=0;
 int time[4]={0,0,0,0};
 switch(player_type[_player])
        {
         ///The Greed
         case 1:
                {
                 player[_player].Set_velocityX((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())>0?1:-1);
                 player[_player].Set_velocityY((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())>0?1:-1);
                 if((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0)
                    player[_player].Set_velocityX(0);
                 if((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0)
                    player[_player].Set_velocityY(0);
                 if((std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==1 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0) ||
                    (std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==1))
                    player[_player].Set_velocityX(0),player[_player].Set_velocityY(0);
                 //player[_player].Block();
                 //player_time_blocked[_player]=10;
                 if(player[_player].Can_attack())
                    Player_basic_attack(_player);
                 break;
                }
         ///Werewolf
         case 2:
                {
                 range=4;
                 time[0]=12800;
                 time[1]=12800;
                 if(player[_player].Get_counter()==-1)
                    {
                     for(int i=0;i<4;i++)
                         player[_player].Start_spell_timer(i);
                     player[_player].Set_counter(0);
                    }
                 player[_player].Set_velocityX((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())>0?1:-1);
                 player[_player].Set_velocityY((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())>0?1:-1);
                 if((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0)
                    player[_player].Set_velocityX(0);
                 if((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0)
                    player[_player].Set_velocityY(0);
                 if((std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==1 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0) ||
                    (std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==1))
                    player[_player].Set_velocityX(0),player[_player].Set_velocityY(0);
                 if(player[_player].Get_hp()<=200)
                    {
                     if(player[Other_player(_player)].Get_map_positionX()<player[_player].Get_map_positionX()-range)
                        player[_player].Set_velocityX(1);
                     if(player[Other_player(_player)].Get_map_positionX()>player[_player].Get_map_positionX()+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT+range)
                        player[_player].Set_velocityX(-1);
                     if(player[Other_player(_player)].Get_map_positionX()>=player[_player].Get_map_positionX()-range &&
                        player[Other_player(_player)].Get_map_positionX()<=player[_player].Get_map_positionX()+player[_player].Get_skinW()/PIXELS_PER_INGAME_UNIT+range)
                        {
                         if(player[Other_player(_player)].Get_map_positionX()<std::abs(arena.Get_number_of_columns()-player[Other_player(_player)].Get_map_positionX()))
                            player[_player].Set_velocityX(1);
                         else
                            player[_player].Set_velocityX(-1);
                        }
                     if(player[Other_player(_player)].Get_map_positionY()<player[_player].Get_map_positionY()-range)
                        player[_player].Set_velocityY(1);
                     if(player[Other_player(_player)].Get_map_positionY()>player[_player].Get_map_positionY()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT+range)
                        player[_player].Set_velocityY(-1);
                     if(player[Other_player(_player)].Get_map_positionY()>=player[_player].Get_map_positionY()-range &&
                        player[Other_player(_player)].Get_map_positionY()<=player[_player].Get_map_positionY()+player[_player].Get_skinH()/PIXELS_PER_INGAME_UNIT+range)
                        {
                         if(player[Other_player(_player)].Get_map_positionY()<std::abs(arena.Get_number_of_columns()-player[Other_player(_player)].Get_map_positionY()))
                            player[_player].Set_velocityY(1);
                         else
                            player[_player].Set_velocityY(-1);
                        }
                    }
                 //player[_player].Block();
                 //player_time_blocked[_player]=10;
                 if(player[_player].Can_attack())
                    Player_basic_attack(_player);
                 if(player[_player].Get_spell_timer_ticks(player[_player].Get_counter())>=time[player[_player].Get_counter()])
                    {
                     Cast_Spell(_player,player[_player].Get_counter());
                     player[_player].Start_spell_timer(player[_player].Get_counter());
                     player[_player].Set_counter(player[_player].Get_counter()+1);
                     player[_player].Set_counter(player[_player].Get_counter()%2);
                    }
                 break;
                }
         ///Yukig
         case 3:
                {
                 range=2;
                 time[0]=14000;
                 if(player[_player].Get_counter()==-1)
                    {
                     for(int i=0;i<4;i++)
                         player[_player].Start_spell_timer(i);
                     player[_player].Set_counter(0);
                    }
                 player[_player].Set_velocityX((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())>0?1:-1);
                 player[_player].Set_velocityY((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())>0?1:-1);
                 if((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0)
                    player[_player].Set_velocityX(0);
                 if((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0)
                    player[_player].Set_velocityY(0);
                 if((std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==1 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0) ||
                    (std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==1))
                    player[_player].Set_velocityX(0),player[_player].Set_velocityY(0);
                 //player[_player].Block();
                 //player_time_blocked[_player]=10;
                 if(player[_player].Can_attack())
                    Player_basic_attack(_player);
                 if((player[_player].Get_spell_timer_ticks(player[_player].Get_counter())>=time[player[_player].Get_counter()] &&
                    std::max(std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX()),
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY()))<=range))
                    {
                     if(player[_player].Get_counter()==-1)
                        player[_player].Set_counter(0);
                     Cast_Spell(_player,player[_player].Get_counter());
                     player[_player].Start_spell_timer(player[_player].Get_counter());
                     player[_player].Set_counter(player[_player].Get_counter()+1);
                     player[_player].Set_counter(player[_player].Get_counter()%1);
                    }
                 break;
                }
         ///Yuzug
         case 4:
                {
                 range=2;
                 time[0]=14000;
                 time[1]=14000;
                 if(player[_player].Get_counter()==-1)
                    {
                     for(int i=0;i<4;i++)
                         player[_player].Start_spell_timer(i);
                     player[_player].Set_counter(0);
                    }
                 player[_player].Set_velocityX((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())>0?1:-1);
                 player[_player].Set_velocityY((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())>0?1:-1);
                 if((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0)
                    player[_player].Set_velocityX(0);
                 if((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0)
                    player[_player].Set_velocityY(0);
                 if((std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==1 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0) ||
                    (std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==1))
                    player[_player].Set_velocityX(0),player[_player].Set_velocityY(0);
                 //player[_player].Block();
                 //player_time_blocked[_player]=10;
                 if(player[_player].Can_attack())
                    Player_basic_attack(_player);
                 if((player[_player].Get_spell_timer_ticks(player[_player].Get_counter())>=time[player[_player].Get_counter()] &&
                    std::max(std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX()),
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY()))<=range))
                    {
                     Cast_Spell(_player,player[_player].Get_counter());
                     player[_player].Start_spell_timer(player[_player].Get_counter());
                     player[_player].Set_counter(player[_player].Get_counter()+1);
                     player[_player].Set_counter(player[_player].Get_counter()%2);
                    }
                 break;
                }
         ///Wizard12
         case 5:
                {
                 range=2;
                 time[0]=3000;
                 time[1]=2000;
                 time[2]=1000;
                 if(player[_player].Get_counter()==-1)
                    {
                     for(int i=0;i<4;i++)
                         player[_player].Start_spell_timer(i);
                     player[_player].Set_counter(0);
                    }
                 player[_player].Set_velocityX((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())>0?1:-1);
                 player[_player].Set_velocityY((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())>0?1:-1);
                 if((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0)
                    player[_player].Set_velocityX(0);
                 if((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0)
                    player[_player].Set_velocityY(0);
                 if((std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==1 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0) ||
                    (std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==1))
                    player[_player].Set_velocityX(0),player[_player].Set_velocityY(0);
                 if(player[_player].Can_attack())
                    Player_basic_attack(_player);
                 if(player[_player].Get_spell_timer_ticks(player[_player].Get_counter())>=time[player[_player].Get_counter()] &&
                    std::max(std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX()),
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY()))<=range)
                    {
                     Cast_Spell(_player,player[_player].Get_counter());
                     if(player[_player].Get_counter()<=1)
                        {
                         player[_player].Block();
                         player_time_blocked[_player]=110;
                         if(player[_player].Get_counter()==1)
                            player_time_blocked[_player]+=270;
                        }
                     player[_player].Start_spell_timer(player[_player].Get_counter());
                     player[_player].Set_counter(player[_player].Get_counter()+1);
                     player[_player].Set_counter(player[_player].Get_counter()%3);
                    }
                 break;
                }
         ///Wizard22
         case 6:
                {
                 range=5;
                 time[0]=5000;
                 if(player[_player].Get_counter()==-1)
                    {
                     for(int i=0;i<4;i++)
                         player[_player].Start_spell_timer(i);
                     player[_player].Set_counter(0);
                    }
                 ///Moves away from player
                 player[_player].Set_velocityX((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())>0?-1:1);
                 player[_player].Set_velocityY((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())>0?-1:1);
                 ///If on the same row or column
                 if(player[_player].Get_map_positionX()==player[Other_player(_player)].Get_map_positionX())
                    player[_player].Set_velocityX((player[_player].Get_map_positionX()>arena.Get_number_of_lines()-player[_player].Get_map_positionX()+1)?-1:1);
                 if(player[_player].Get_map_positionY()==player[Other_player(_player)].Get_map_positionY())
                    player[_player].Set_velocityY((player[_player].Get_map_positionY()>arena.Get_number_of_columns()-player[_player].Get_map_positionY()+1)?-1:1);
                 if(player[_player].Get_spell_timer_ticks(player[_player].Get_counter())>=time[player[_player].Get_counter()] &&
                    std::max(std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX()),
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY()))<=range)
                    {
                     Cast_Spell(_player,player[_player].Get_counter());
                     player[_player].Start_spell_timer(player[_player].Get_counter());
                    }
                 break;
                }
         ///Rainbow Sheep1
         case 7:
                {
                 player[_player].Set_velocityX((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())>0?1:-1);
                 player[_player].Set_velocityY((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())>0?1:-1);
                 if((player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0)
                    player[_player].Set_velocityX(0);
                 if((player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0)
                    player[_player].Set_velocityY(0);
                 if((std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==1 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==0) ||
                    (std::abs(player[Other_player(_player)].Get_map_positionX()-player[_player].Get_map_positionX())==0 &&
                    std::abs(player[Other_player(_player)].Get_map_positionY()-player[_player].Get_map_positionY())==1))
                    player[_player].Set_velocityX(0),player[_player].Set_velocityY(0);
                 //player[_player].Block();
                 //player_time_blocked[_player]=10;
                 if(player[_player].Can_attack())
                    Player_basic_attack(_player);
                 break;
                }
        }
}

void Level::AI_Block_player(int _player)
{
 switch(player_type[_player])
        {
         case 1:
                {
                 player_time_blocked[_player]=5-5*player[_player].Get_movement_speed()/100+5;
                 break;
                }
        }
}

//Start
void Level::Pause_Menu()
{
 Menu menu;
 Script_interpreter run_script;
 if(type==2 && (player_type[1]!=0 || player_type[2]!=0))
    {
     Pause_music();
     menu.Load("menu/pause_menu_duel.pwm");
     int option=0;
     while(option!=-2)
           {
            option=menu.Start(_screen,&reload);
            switch(option)
                   {
                    case -2:Unpause_music();
                            break;
                    case 0:Unpause_music();
                           option=-2;
                           break;
                    case 1:run_script.Start(_screen,"Run",0,0);
                           option=-2;
                           done=true;
                           break;
                    case 2:Controls_screen(_screen);
                           break;
                    case 3:exit(0);
                   }
           }
     SDL_Delay(150);
    }
 else
    {
     menu.Load("menu/pause_menu.pwm");
     int option=0;
     while(option!=-2)
           {
            option=menu.Start(_screen,&reload);
            switch(option)
                   {
                    case 0:option=-2;
                           break;
                    case 1:option=-2;
                           done=true;
                           break;
                    case 2:Controls_screen(_screen);
                           break;
                    case 3:exit(0);
                   }
           }
     SDL_Delay(150);
    }
 menu.Clear();
 if(reload)
    {
     reload=false;
     Fast_Reload();
     Clear_Journal();
     Load_Journal();
     SDL_PumpEvents();
    }
}

bool Level::Duel_Mode_Finish_Screen(int _player_winner)
{
 winner=_player_winner;
 player[_player_winner].Increase_number_of_wins();
 Texture *finish_screen=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
 Print_Duel_Mode_Finish_Screen(_player_winner,finish_screen);
 Apply_Texture(0,0,finish_screen,_screen);
 Flip_Buffers(_screen);
 bool quit=false;
 SDL_Event event;
 SDL_Delay(500);
 while(SDL_PollEvent(&event));
 SDL_Delay(500);
 while(SDL_PollEvent(&event));
 while(!quit)
       {
        SDL_PollEvent(&event);
        Update_Controllers_Events();
        if((event.type==SDL_KEYDOWN && (event.key.keysym.scancode==SDL_SCANCODE_RETURN || event.key.keysym.scancode==SDL_SCANCODE_ESCAPE)) ||
            (controller[1].Pressed_Start_button() || controller[1].Pressed_Guide_button() || controller[2].Pressed_Start_button() || controller[2].Pressed_Guide_button()))
           quit=true;
        Apply_Texture(0,0,finish_screen,_screen);
        Flip_Buffers(_screen);
        SDL_Delay(100);
       }
 quit=false;
 /*if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE && (player_type[2]!=0 && winner!=1))
    exit(0);*/
 if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE || controller[1].Pressed_Guide_button() || controller[2].Pressed_Guide_button() || (player_type[2]!=0 && winner==1))
    return false;
 return true;
}

void Level::Print_Duel_Mode_Finish_Screen(int _player_winner,Texture *screen)
{
 Apply_Texture(0,0,LEVEL_background_image,screen);
 if(_player_winner==1)
    {
     Apply_Texture((screen->w/2-LEVEL_WINNER->w)/2,5,LEVEL_WINNER,screen);
     player[1].Print_name((screen->w/2-LEVEL_WINNER->w)/2,5+LEVEL_WINNER->h,screen);
     Apply_Texture(screen->w/2+(screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,screen);
     player[2].Print_name(screen->w/2+(screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,screen);
    }
 if(_player_winner==2)
    {
     Apply_Texture((screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,screen);
     player[1].Print_name((screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,screen);
     Apply_Texture(screen->w/2+(screen->w/2-LEVEL_WINNER->w)/2,5,LEVEL_WINNER,screen);
      player[2].Print_name(screen->w/2+(screen->w/2-LEVEL_WINNER->w)/2,5+LEVEL_WINNER->h,screen);
    }
 if(_player_winner==0)
    {
     Apply_Texture((screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,screen);
     player[1].Print_name((screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,screen);
     Apply_Texture(screen->w/2+(screen->w/2-LEVEL_LOSER->w)/2,5,LEVEL_LOSER,screen);
     player[2].Print_name(screen->w/2+(screen->w/2-LEVEL_LOSER->w)/2,5+LEVEL_LOSER->h,screen);
    }
 Texture *player_xp,*player_money,*player_xp_gain,*player_money_gain;
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Color xp_color={75,0,130},MONEY_COLOR={236,242,4},wins_color={241,188,48};
 char aux[TEXT_LENGTH_MAX]={NULL};

 if(player_type[1]==0)
    {
     itoa(player[1].Get_number_of_wins(),aux);
     player_xp=Create_TTF_Texture(font,aux,wins_color);
     Apply_Texture((screen->w/2-(LEVEL_WINS->w+player_xp->w))/2,screen->h/2-LEVEL_XP->h-LEVEL_WINS->h-25,LEVEL_WINS,screen);
     Apply_Texture((screen->w/2-(LEVEL_WINS->w+player_xp->w))/2+LEVEL_WINS->w,screen->h/2-LEVEL_XP->h-LEVEL_WINS->h-25,player_xp,screen);
     Destroy_Texture(player_xp);

     itoa(player[1].Get_experience(),aux);
     strcat(aux,"  +  ");
     player_xp=Create_TTF_Texture(font,aux,xp_color);

     itoa(10*player[2].Get_experience()/100+10+((_player_winner==2)?20:0),aux);
     player_xp_gain=Create_TTF_Texture(font,aux,xp_color);

     Apply_Texture((screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2,screen->h/2-LEVEL_XP->h,LEVEL_XP,screen);
     Apply_Texture((screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w,screen->h/2-LEVEL_XP->h,player_xp,screen);
     Apply_Texture((screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w+player_xp->w,screen->h/2-LEVEL_XP->h,player_xp_gain,screen);
     Destroy_Texture(player_xp);
     Destroy_Texture(player_xp_gain);

     itoa(player[1].Get_money(),aux);
     strcat(aux,"  +  ");
     player_money=Create_TTF_Texture(font,aux,MONEY_COLOR);

     itoa((10+player[1].Get_extra_money())*player[2].Get_experience()/100+20+((_player_winner==1)?40:0),aux);
     player_money_gain=Create_TTF_Texture(font,aux,MONEY_COLOR);

     Apply_Texture((screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2,screen->h/2+LEVEL_MONEY->h,LEVEL_MONEY,screen);
     Apply_Texture((screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w,screen->h/2+LEVEL_MONEY->h,player_money,screen);
     Apply_Texture((screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w+player_money->w,screen->h/2+LEVEL_MONEY->h,player_money_gain,screen);
     Destroy_Texture(player_money);
     Destroy_Texture(player_money_gain);
    }

 if(player_type[2]==0)
    {
     itoa(player[2].Get_number_of_wins(),aux);
     player_xp=Create_TTF_Texture(font,aux,wins_color);
     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_WINS->w))/2,screen->h/2-LEVEL_WINS->h-LEVEL_WINS->h-25,LEVEL_WINS,screen);
     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_WINS->w))/2+LEVEL_WINS->w,screen->h/2-LEVEL_WINS->h-LEVEL_WINS->h-25,player_xp,screen);
     Destroy_Texture(player_xp);

     itoa(player[2].Get_experience(),aux);
     strcat(aux,"  +  ");
     player_xp=Create_TTF_Texture(font,aux,xp_color);

     itoa(10*player[1].Get_experience()/100+10+((_player_winner==1)?20:0),aux);
     player_xp_gain=Create_TTF_Texture(font,aux,xp_color);

     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2,screen->h/2-LEVEL_XP->h,LEVEL_XP,screen);
     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w,screen->h/2-LEVEL_XP->h,player_xp,screen);
     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_XP->w+player_xp->w+player_xp_gain->w))/2+LEVEL_XP->w+player_xp->w,screen->h/2-LEVEL_XP->h,player_xp_gain,screen);
     Destroy_Texture(player_xp);
     Destroy_Texture(player_xp_gain);

     itoa(player[2].Get_money(),aux);
     strcat(aux,"  +  ");
     player_money=Create_TTF_Texture(font,aux,MONEY_COLOR);

     itoa((10+player[2].Get_extra_money())*player[1].Get_experience()/100+20+((_player_winner==2)?40:0),aux);
     player_money_gain=Create_TTF_Texture(font,aux,MONEY_COLOR);

     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2,screen->h/2+LEVEL_MONEY->h,LEVEL_MONEY,screen);
     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w,screen->h/2+LEVEL_MONEY->h,player_money,screen);
     Apply_Texture(screen->w/2+(screen->w/2-(LEVEL_MONEY->w+player_money->w+player_money_gain->w))/2+LEVEL_MONEY->w+player_money->w,screen->h/2+LEVEL_MONEY->h,player_money_gain,screen);
     Destroy_Texture(player_money);
     Destroy_Texture(player_money_gain);
    }

 if(player_type[1]==0)
    player[1].Set_money(player[1].Get_money()+(10+player[1].Get_extra_money())*player[2].Get_experience()/100+20+((_player_winner==1)?40:0));
 if(player_type[2]==0)
    player[2].Set_money(player[2].Get_money()+(10+player[2].Get_extra_money())*player[1].Get_experience()/100+20+((_player_winner==2)?40:0));
 int a=player[1].Get_experience(),b=player[2].Get_experience();
 if(player_type[1]==0)
    player[1].Set_experience(player[1].Get_experience()+10*player[2].Get_experience()/100+10+((_player_winner==2)?20:0));
 if(player_type[2]==0)
    player[2].Set_experience(player[2].Get_experience()+10*player[1].Get_experience()/100+10+((_player_winner==1)?20:0));

 if(player_type[2]!=0 && winner==1)
    {
     player_money=Create_TTF_Texture(font,"Press ENTER to continue!",{255,255,255});
     Apply_Texture((screen->w-player_money->w)/2,screen->h/2+(screen->h/2+LEVEL_MONEY->h+LEVEL_XP->h)/2,player_money,screen);
     Destroy_Texture(player_money);
    }
 else
    {
     player_money=Create_TTF_Texture(font,"Press ESC to exit or ENTER to rematch!",{255,255,255});
     Apply_Texture((screen->w-player_money->w)/2,screen->h/2+(screen->h/2+LEVEL_MONEY->h+LEVEL_XP->h)/2,player_money,screen);
     Destroy_Texture(player_money);
    }

 TTF_CloseFont(font);
 Flip_Buffers(screen);
}

void Level::Set_screen(Texture *screen)
{
 _screen=screen;
 static_screen=screen;
}

void Level::Setup(char *_level_name)
{
 Set_name(_level_name);
 SDL_Thread *_loading_image=NULL;
 _loading_image=SDL_CreateThread(Loading_image,"Level Setup Loading",NULL);
 Load();
 //SDL_Delay(1000);
 int thread_return_value=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 SDL_WaitThread(_loading_image,&thread_return_value);
 Flip_Buffers(_screen);
 Set_arena_size();
 player_time_blocked[1]=player_time_blocked[2]=0;
 player[1].Unblock();
 player[2].Unblock();
 if(type!=2)
    return;
 level_duration.start();
 Set_player_POSX(1,((RESOLUTION_W-840)/2)/2-100);
 Set_player_LAST_POSX(1,((RESOLUTION_W-840)/2)/2+100);
 Set_player_POSX(2,(RESOLUTION_W+(RESOLUTION_W-840)/2+840)/2-100);
 Set_player_LAST_POSX(2,(RESOLUTION_W+(RESOLUTION_W-840)/2+840)/2+100);
}

const int GAME_FPS=60;

void Level::Start(Texture *screen,bool cleanup)
{
 winner=0;
 bool play=true;
 int thread_return_value=0;
 //Mix_HaltMusic();
 while(play)
       {
        if(type==3)
           {
            Script_interpreter _script;
            char aux[TEXT_LENGTH_MAX]={NULL};
            strcpy(aux,"level/");
            strcat(aux,name);
            SDL_Delay(100);
            _script.Start(screen,aux,0,0);
           }
        if(terrain_type!=last_terrain_type || type==2)
           {
            Change_music(1);
            level_music_time.start();
           }
        level_music_overseer=SDL_CreateThread(Level::Oversee_music,"Music Overseer",NULL);
        done=false;
        _screen=screen;
        static_screen=screen;
        bool quit=false;
        int frame=0;
        Timer fps,arena,buffs,current_time,level_music_time;
        arena.start();
        buffs.start();
        level_duration.start();
        current_time.start();
        level_music_time.start();
        skeptic_vision_timer.start();
        level_name_image_timer.start();
        #ifdef AUDIO
        Change_music(1);
        //Start_music();
        #endif // AUDIO
        int previous_time=current_time.get_ticks(),lag=0;
        while(!quit && !done)
              {
               if(reload)
                  {

                  }
               fps.start();
               if(level_changed || reset_lag)
                  {
                   previous_time=current_time.get_ticks();
                   level_changed=false;
                   reset_lag=false;
                  }
               int elapsed=current_time.get_ticks()-previous_time;
               previous_time=current_time.get_ticks();
               lag+=elapsed;
               SDL_PumpEvents();
               Update_Controllers_Events();
               quit=keystates[SDL_QUIT] || ((keystates[SDL_SCANCODE_RALT] || keystates[SDL_SCANCODE_LALT]) && keystates[SDL_SCANCODE_F4]);
               Handle_Events(screen);
               while(!level_changed && lag>MS_PER_UPDATE && !done)
                     {
                      Time_Pass();
                      Move_all_players();
                      if(arena.get_ticks()>1000/ARENA_FRAMES_PER_SECOND)
                         {
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
               Apply_Texture(0,0,LEVEL_background_image,screen);
               if(type==2)
                  Print_players_informations(screen);
               Print_Map((RESOLUTION_W-840)/2,PIXELS_PER_INGAME_UNIT,screen);
               if(!done)
                  Flip_Buffers(screen);
               if(type==2 && level_duration.get_ticks()>duration)
                  quit=true;
               if(type==2 && ((player[1].Get_hp()<=0 && !player[1].Is_immortal()) || (player[2].Get_hp()<=0 && !player[2].Is_immortal())))
                  quit=true;
               if(fps.get_ticks()<1000/GAME_FPS)
                  {
                   SDL_Delay((1000/GAME_FPS)-fps.get_ticks());
                  }
              }
        play=false;
        SDL_LockMutex(music_overseer_mutex);
        Oversee_music_quit=true;
        SDL_UnlockMutex(music_overseer_mutex);
        SDL_WaitThread(level_music_overseer,&thread_return_value);
        if(quit && type==2)
           {
            Stop_music();
            SDL_Delay(50);
            Mix_PlayChannel(4,DUEL_MODE_hit[1],0);
            if(player[1].Get_hp()<=0 && player[2].Get_hp()<=0)
               play=Duel_Mode_Finish_Screen(winner=0);
            else
               play=Duel_Mode_Finish_Screen(winner=(player[1].Get_hp()<=0?2:1));
           }
        if(!save_slot_used || type!=1)
           player[1].Update();
        if(type==2 && player_type[2]==0)
           player[2].Update();
        player_time_blocked[1]=player_time_blocked[2]=0;
        player[1].Unblock();
        player[2].Unblock();
        if(type==2)
           {
            player[1].Reset(player_map_position[1].first,player_map_position[1].second);
            player[2].Reset(player_map_position[2].first,player_map_position[2].second);
           }
        effects.Clear(false,false);
        effects.Set_name("Empty");
        effects.Load(player[1].Get_keys());
       }
 if(cleanup)
    Cleanup();
 play=false;
}

void Level::Cleanup()
{
 SDL_Thread *_loading_image=NULL;
 _loading_image=SDL_CreateThread(Loading_image,"Level Cleanup Loading",NULL);
 Clear();
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 int thread_return_value=0;
 SDL_WaitThread(_loading_image,&thread_return_value);
 Flip_Buffers(static_screen);
}

void Level::Save_gamemode()
{
 int player_map_position_x,player_map_position_y;
 FILE *where=fopen("saves/gamemodes/Story Mode.pwsav","w");
 player_map_position_x=Get_player_map_position_x(1);
 player_map_position_y=Get_player_map_position_y(1);
 fprintf(where,"%d %d\n",player_map_position_x,player_map_position_y);
 fprintf(where,"%s\n%s\n",Get_name(),player[1].Get_name());
 fclose(where);
}

///Launch
void Launch_Story_Mode(Level *level,Texture *_screen)
{
 bool done=false;
 while(!done)
       {

 int player_map_position_x,player_map_position_y;
 char level_name[TEXT_LENGTH_MAX],player_name[TEXT_LENGTH_MAX];
 FILE *where=fopen("saves/gamemodes/Story Mode.pwsav","r");
 if(where==NULL)
    {
     fclose(where);
     where=fopen("saves/gamemodes/Story Mode.pwsav","w");
     fprintf(where,"%d %d\nTimy's Lab\nTimy",14,6);
     fclose(where);
     where=fopen("saves/gamemodes/Story Mode.pwsav","r");
     if(where==NULL)
        {
         FILE *log_file=fopen("err/logs.txt","w");
         fprintf(log_file,"\"saves/gamemodes/Story Mode.pwsav\" is missing");
         fclose(log_file);
         SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game mode folder missing","\"saves/gamemodes\" is missing\nCreate the folder manually.",NULL);
         exit(5);
        }
    }
 fscanf(where,"%d %d ",&player_map_position_x,&player_map_position_y);
 fgets(level_name,sizeof level_name,where);
 int n=strlen(level_name);
 if(level_name[n-1]=='\n')
    level_name[n-1]=NULL;
 fgets(player_name,sizeof player_name,where);
 n=strlen(player_name);
 if(player_name[n-1]=='\n')
    player_name[n-1]=NULL;
 fclose(where);
 level->Set_screen(_screen);
 Mix_HaltMusic();
 level->Setup(level_name);
 level->Set_player_map_position(player_map_position_x,player_map_position_y,1);
 if(DISPLAY_MODE!=SDL_WINDOW_FULLSCREEN)
    level->Fast_Reload();
 level->Start(_screen,false);
 done=true;
 #ifndef PRESENTATION_MODE
 if(level->Get_terrain_type()!=LEVEL_PUZZLE_TYPE)
    level->Save_gamemode();
 level->Cleanup();
 #else
 if(level->Get_terrain_type()!=LEVEL_PUZZLE_TYPE && !save_slot_used)
    level->Save_gamemode();
 if(save_slot_used)
    save_slot_used=false,done=false;
 level->Cleanup();
 #endif // PRESENTATION_MODE
       }
}

void Launch_Duel_Mode(Level *level,Texture *_screen)
{
 level->Set_screen(_screen);
 level->Setup("Duel Mode");
 if(DISPLAY_MODE!=SDL_WINDOW_FULLSCREEN)
    level->Fast_Reload();
 level->Start(_screen);
}

void Open_Journal(std::bitset<NUMBER_OF_MAX_KEYS> *progress,Texture *_screen)
{
 journal.Start(progress,_screen);
}

int Other_player(int _player)
{
 return _player==1?2:1;
}
