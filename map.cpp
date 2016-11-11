#include "map.h"
#include "settings.h"

/*Map::~Map()
{
 Clear(true);
}*/

void Map::Clear(bool _delete,bool _delete_all)
{
 if(_delete && _delete_all)
    {
     //map_textures[20].Clear();
     //map_textures[0].Clear();
     for(int i=0;i<NUMBER_OF_TEXTURES_IDS;i++)
         {
          if(map_textures[i].Get_id()!=0 && map_textures[i].Get_id()!=20)
             map_textures[i].Clear(true);
         }
    }
 else
    {
     for(int i=0;i<NUMBER_OF_TEXTURES_IDS;i++)
         {
          if(map_textures[i].Get_id()!=0 && map_textures[i].Get_id()!=20)
             map_textures[i].Clear(false);
         }
    }
 for(int i=0;i<=1;i++)
     for(int j=0;j<=1;j++)
         {
          if(map_image[i][j]!=NULL && _delete)
             {
              SDL_FreeSurface(map_image[i][j]);
              map_image[i][j]=NULL;
             }
          if(background_map_image[i][j]!=NULL && _delete)
             {
              SDL_FreeSurface(background_map_image[i][j]);
              background_map_image[i][j]=NULL;
             }
          std::vector<std::pair<int,int> >().swap(fast_access_map_textures_animations[i][j]);
          std::vector<std::pair<int,int> >().swap(fast_access_background_map_textures_animations[i][j]);
         }
 for(int i=0;i<number_of_lines;i++)
     for(int j=0;j<number_of_columns;j++)
         {
          map_textures_ids[i][j].Clear();
          background_map_textures_ids[i][j].Clear();
         }
}

void Map::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Map::Set_map_texture_id(int x,int y,Interactive_map_texture _map_texture_id)
{
 std::vector<Map_texture_id> textures_ids;
 _map_texture_id.Get_all_textures_ids(textures_ids);
 for(std::vector<Map_texture_id>::iterator i=textures_ids.begin();i!=textures_ids.end();i++)
     if(map_textures[i->Get_id()].Get_id()==0 || map_textures[i->Get_id()].Get_id()==20)
        {
         map_textures[i->Get_id()].Set_id(i->Get_id());
         map_textures[i->Get_id()].Load();
        }
 if(map_textures[_map_texture_id.Get_texture_id()].Get_power()>map_textures[map_textures_ids[x][y].Get_texture_id()].Get_power())
    map_textures_ids[x][y]=_map_texture_id;
}

bool Map::Is_obstacle(int x,int y)
{
 if(y>=number_of_columns || x>=number_of_lines)
    return false;
 return map_textures[map_textures_ids[x][y].Get_texture_id()].Is_obstacle();
}

bool Map::Is_obstacle_pixel(int x,int y)
{
 return map_obstacles[x][y];
}

bool Map::Is_light(int x,int y)
{
 return map_textures[map_textures_ids[x][y].Get_texture_id()].Is_light() || map_textures[background_map_textures_ids[x][y].Get_texture_id()].Is_light();
}

char *Map::Get_name()
{
 return name;
}

int Map::Get_type(int x,int y)
{
 return map_textures[map_textures_ids[x][y].Get_texture_id()].Get_type();
}

int Map::Get_number_of_lines()
{
 return number_of_lines;
}

int Map::Get_number_of_columns()
{
 return number_of_columns;
}

Buff Map::Get_map_texture_Buff(int x,int y)
{
 return map_textures[map_textures_ids[x][y].Get_texture_id()].Get_Buff();
}

Interactive_map_texture Map::Get_map_texture_id(int x,int y)
{
 return map_textures_ids[x][y];
}

int Map::Get_map_texture_type(int x,int y)
{
 return map_textures[map_textures_ids[x][y].Get_texture_id()].Get_type();
}

int Map::Get_map_texture_player_pos_x(int x,int y)
{
 return map_textures_ids[x][y].Get_texture_player_map_pos_x();
}

int Map::Get_map_texture_player_pos_y(int x,int y)
{
 return map_textures_ids[x][y].Get_texture_player_map_pos_y();
}

char *Map::Get_map_texture_map_name(int x,int y)
{
 return map_textures_ids[x][y].Get_texture_map_name();
}

int Map::Get_map_texture_key_id(int x,int y)
{
 return map_textures_ids[x][y].Get_key_id();
}

void Map::Copy(int x,int y,Map *source)
{
 for(int i=0;i<source->Get_number_of_lines();i++)
     for(int j=0;j<source->Get_number_of_columns();j++)
         {
          if(x+i<0 || y+j<0 || x+i>=Get_number_of_lines() || y+j>=Get_number_of_columns())
             continue;
          Set_map_texture_id(x+i,y+j,source->Get_map_texture_id(i,j));
         }
}

void Map::Decrease_map_textures_ids_remaining_time()
{
 for(int i=0;i<=1;i++)
     for(int j=0;j<=1;j++)
         {
          Decrease_map_texture_id_remaining_time(i,j);
          Decrease_background_map_texture_id_remaining_time(i,j);
         }
}

void Map::Decrease_map_texture_id_remaining_time(bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_map_textures_animations[before_player][lights].begin();it!=fast_access_map_textures_animations[before_player][lights].end();it++)
     {
      if(!map_textures_ids[it->first][it->second].Is_done())
         {
          map_textures_ids[it->first][it->second].Decrease_remaining_duration();
          map_textures_ids[it->first][it->second].Update_texture_frame(map_textures[map_textures_ids[it->first][it->second].Get_texture_id()].Get_number_of_frames());
         }
     }
}

void Map::Decrease_background_map_texture_id_remaining_time(bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_background_map_textures_animations[before_player][lights].begin();it!=fast_access_background_map_textures_animations[before_player][lights].end();it++)
     {
      if(!map_textures_ids[it->first][it->second].Is_done())
         {
          background_map_textures_ids[it->first][it->second].Decrease_remaining_duration();
          background_map_textures_ids[it->first][it->second].Update_texture_frame(map_textures[background_map_textures_ids[it->first][it->second].Get_texture_id()].Get_number_of_frames());
         }
     }
}


void Map::Update_all_frames()
{
 current_number_of_updates++;
 Decrease_map_textures_ids_remaining_time();
}

bool Map::Is_done()
{
 return (current_number_of_updates>number_of_updates && number_of_updates>0);
}

void Map::Load(std::bitset<NUMBER_OF_MAX_KEYS> *_keys)
{
 char path[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"maps/");
 strcat(path,name);
 strcat(path,".pwm");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d %d %d %d ",&is_static,&number_of_updates,&number_of_lines,&number_of_columns);

 for(int i=0;i<=1 && is_static;i++)
     for(int j=0;j<=1;j++)
         {
          map_image[i][j]=make_it_transparent("images/game/empty.bmp");
          background_map_image[i][j]=make_it_transparent("images/game/empty.bmp");
         }

 int number_of_locks=0;
 fscanf(where,"%d ",&number_of_locks);
 for(int i=0;i<number_of_locks;i++)
     {
      int x,y,key_id,texture_id;
      fscanf(where,"%d %d %d %d ",&x,&y,&key_id,&texture_id);
      if((*_keys)[key_id])
         map_textures_ids[x][y].Set_id(texture_id);
     }

 std::vector<Map_texture_id> textures_ids;

 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int __id;
           fscanf(where,"%d ",&__id);
           if(map_textures_ids[i][j].Get_id()!=0)
              map_textures_ids[i][j].Load();
           else
              map_textures_ids[i][j].Load(__id);

           map_textures_ids[i][j].Get_all_textures_ids(textures_ids);

           for(std::vector<Map_texture_id>::iterator it=textures_ids.begin();it!=textures_ids.end();it++)
               {
                if(map_textures[it->Get_id()].Get_id()==0 && it->Get_id()!=0)
                   {
                    map_textures[it->Get_id()].Set_id(it->Get_id());
                    map_textures[it->Get_id()].Load();
                   }
               }

           if(map_textures[map_textures_ids[i][j].Get_texture_id()].Is_animation())
              fast_access_map_textures_animations[map_textures[map_textures_ids[i][j].Get_texture_id()].Get_print_before_player()][map_textures[map_textures_ids[i][j].Get_texture_id()].Is_light()].push_back(std::make_pair(i,j));
           else
              Print_image(j*40,i*40,map_image[map_textures[map_textures_ids[i][j].Get_texture_id()].Get_print_before_player()][map_textures[map_textures_ids[i][j].Get_texture_id()].Is_light()],&map_textures_ids[i][j]);
          }
     }
 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int __id;
           fscanf(where,"%d ",&__id);
           background_map_textures_ids[i][j].Load(__id);

           background_map_textures_ids[i][j].Get_all_textures_ids(textures_ids);

           for(std::vector<Map_texture_id>::iterator it=textures_ids.begin();it!=textures_ids.end();it++)
               {
                if(map_textures[it->Get_id()].Get_id()==0)
                   {
                    map_textures[it->Get_id()].Set_id(it->Get_id());
                    map_textures[it->Get_id()].Load();
                   }
               }

           if(map_textures[background_map_textures_ids[i][j].Get_texture_id()].Is_animation())
              fast_access_background_map_textures_animations[map_textures[background_map_textures_ids[i][j].Get_texture_id()].Get_print_before_player()][map_textures[background_map_textures_ids[i][j].Get_texture_id()].Is_light()].push_back(std::make_pair(i,j));
           else
              Print_image(j*40,i*40,background_map_image[map_textures[background_map_textures_ids[i][j].Get_texture_id()].Get_print_before_player()][map_textures[background_map_textures_ids[i][j].Get_texture_id()].Is_light()],&background_map_textures_ids[i][j]);
          }
     }
 int number_of_triggers=0;
 fscanf(where,"%d ",&number_of_triggers);
 for(int i=0;i<number_of_triggers;i++)
     {
      int x,y,number_of_targets;
      fscanf(where,"%d %d %d",&x,&y,&number_of_targets);
      for(int j=0;j<number_of_targets;j++)
          {
           int x1,y1;
           fscanf(where,"%d %d ",&x1,&y1);
           map_textures_ids[x][y].Add_target(x1,y1);
          }
     }
 int number_of_map_changers=0;
 fscanf(where,"%d ",&number_of_map_changers);
 for(int i=0;i<number_of_map_changers;i++)
     {
      int x,y,x1,y1,_texture;
      char map_name[TEXT_LENGHT_MAX];
      fscanf(where,"%d %d %d %d %d ",&_texture,&x,&y,&x1,&y1);
      fgets(map_name,sizeof map_name,where);
      if(map_name[strlen(map_name)-1]=='\n')
         map_name[strlen(map_name)-1]=NULL;
      map_textures_ids[x][y].Set_texture_player_map_pos(_texture,x1,y1);
      map_textures_ids[x][y].Set_texture_map_name(_texture,map_name);
     }
 int number_of_keys=0;
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_locks;i++)
     {
      int x,y,key_id,_texture;
      fscanf(where,"%d %d %d %d ",&_texture,&x,&y,&key_id);
      map_textures_ids[x][y].Set_key_id(_texture,key_id);
     }
 fclose(where);
}

const int MAP_IMAGE_HEIGHT=840,MAP_IMAGE_WEIGHT=680;

void Map::Print(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights)
{
 //apply_surface(screen_x,screen_y,map_image[before_player][lights],_screen);
 apply_surface(map_x*40,map_y*40,screen_x,screen_y,MAP_IMAGE_HEIGHT,MAP_IMAGE_WEIGHT,map_image[before_player][lights],_screen);
}

void Map::Print_Animations(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_map_textures_animations[before_player][lights].begin();it!=fast_access_map_textures_animations[before_player][lights].end();it++)
     {
      if(!map_textures_ids[it->first][it->second].Is_done() && it->second>=map_x && it->first>=map_y && it->first<map_y+MAP_IMAGE_WEIGHT/40 && it->second<map_x+MAP_IMAGE_HEIGHT/40)
         Print_image((it->second-map_x)*40+screen_x,(it->first-map_y)*40+screen_y,_screen,&map_textures_ids[it->first][it->second]);
     }
}

void Map::Print_background(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights)
{
 //apply_surface(screen_x,screen_y,background_map_image[before_player][lights],_screen);
 apply_surface(map_x*40,map_y*40,screen_x,screen_y,MAP_IMAGE_HEIGHT,MAP_IMAGE_WEIGHT,background_map_image[before_player][lights],_screen);
}

void Map::Print_background_Animations(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights)
{
 for(std::vector<std::pair<int,int> >::iterator it=fast_access_background_map_textures_animations[before_player][lights].begin();it!=fast_access_background_map_textures_animations[before_player][lights].end();it++)
     {
      if(!background_map_textures_ids[it->first][it->second].Is_done() && it->second>=map_x && it->first>=map_y && it->first<map_y+MAP_IMAGE_WEIGHT/40 && it->second<map_x+MAP_IMAGE_HEIGHT/40)
         Print_image((it->second-map_x)*40+screen_x,(it->first-map_y)*40+screen_y,_screen,&background_map_textures_ids[it->first][it->second]);
     }
}

void Map::Print_image(int screen_x,int screen_y,SDL_Surface *_screen,Interactive_map_texture *source)
{
 map_textures[source->Get_texture_id()].Print_image(screen_x,screen_y,_screen,source->Get_texture_frame());
}

void Map::Trigger(int x,int y)
{
 std::vector<std::pair<int,int> > target_map_position;
 map_textures_ids[x][y].Trigger();
 map_textures_ids[x][y].Get_all_targets_map_positions(target_map_position);
 for(std::vector<std::pair<int,int> >::iterator it=target_map_position.begin();it!=target_map_position.end();it++)
     Trigger(it->first,it->second);

 background_map_textures_ids[x][y].Trigger();
 background_map_textures_ids[x][y].Get_all_targets_map_positions(target_map_position);
 for(std::vector<std::pair<int,int> >::iterator it=target_map_position.begin();it!=target_map_position.end();it++)
     Trigger(it->first,it->second);
}
