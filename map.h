#ifndef MAP_H
#define MAP_H

#include "map_texture.h"
#include "interactive_map_texture.h"
#include<vector>
#include<bitset>

const int NUMBER_OF_LINES_MAX=1000,NUMBER_OF_COLUMNS_MAX=1000,NUMBER_OF_TEXTURES_IDS=100;

#ifndef PLAYER_H
const int NUMBER_OF_MAX_KEYS=10;
#endif // PLAYER_H

class Map
{
 private:
 char name[TEXT_LENGHT_MAX]={NULL};
 int number_of_lines=0,number_of_columns=0;
 Texture map_textures[NUMBER_OF_TEXTURES_IDS];
 Interactive_map_texture map_textures_ids[NUMBER_OF_LINES_MAX][NUMBER_OF_COLUMNS_MAX],background_map_textures_ids[NUMBER_OF_LINES_MAX][NUMBER_OF_COLUMNS_MAX];
 std::vector<std::pair<int,int> > fast_access_map_textures_animations[2][2],fast_access_background_map_textures_animations[2][2];
 SDL_Surface *map_image[2][2],*background_map_image[2][2];
 int number_of_updates,current_number_of_updates;
 bool map_obstacles[NUMBER_OF_LINES_MAX][NUMBER_OF_COLUMNS_MAX];
 bool is_static=true;

 public:
 //~Map();
 void Clear(bool _delete=false,bool _delete_all=true);
 void Set_name(char *_name);
 void Set_map_texture_id(int x,int y,Interactive_map_texture _map_texture_id);
 bool Is_obstacle(int x,int y);
 bool Is_obstacle_pixel(int x,int y);
 bool Is_light(int x,int y);
 char *Get_name();
 int Get_type(int x,int y);
 int Get_number_of_lines();
 int Get_number_of_columns();
 Buff Get_map_texture_Buff(int x,int y);
 Interactive_map_texture Get_map_texture_id(int x,int y);
 int Get_map_texture_type(int x,int y);
 int Get_map_texture_player_pos_x(int x,int y);
 int Get_map_texture_player_pos_y(int x,int y);
 char *Get_map_texture_map_name(int x,int y);
 int Get_map_texture_key_id(int x,int y);
 void Update_frame(int _texture_id);
 void Decrease_map_textures_ids_remaining_time();
 void Decrease_map_texture_id_remaining_time(bool before_player,bool lights);
 void Decrease_background_map_texture_id_remaining_time(bool before_player,bool lights);
 void Update_all_frames();
 bool Is_done();
 void Load(std::bitset<NUMBER_OF_MAX_KEYS> _keys);
 void Copy(int x,int y,Map *source);
 void Print(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights=false);
 void Print_Animations(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights=false);
 void Print_background(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights=false);
 void Print_background_Animations(int screen_x,int screen_y,int map_x,int map_y,SDL_Surface *_screen,bool before_player,bool lights=false);
 void Print_image(int screen_x,int screen_y,SDL_Surface *_screen,Interactive_map_texture *source);

 void Trigger(int x,int y);
};

#endif // MAP_H
