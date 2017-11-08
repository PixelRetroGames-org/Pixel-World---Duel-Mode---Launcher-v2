#ifndef INTERACTIVE_MAP_TEXTURE_H
#define INTERACTIVE_MAP_TEXTURE_H

#include "library.h"

#include <vector>

class Map_texture_id
{
 private:
 int id;
 int duration=0,remaining_duration=0;
 int frame=0;
 int frame_change_elapsed=0;
 char map_name[TEXT_LENGTH_MAX]={NULL};
 char puzzle_name[TEXT_LENGTH_MAX]={NULL};
 int player_map_pos_x,player_map_pos_y;
 int key_id=0;

 public:
 Map_texture_id();
 void Set_id(int _id);
 void Set_duration(int _duration);
 void Set_frame(int _frame);
 void Set_frame_change_elapsed(int _frame_change_elapsed);
 void Set_player_map_pos(int x,int y);
 void Set_map_name(char *_map_name);
 void Set_puzzle_name(char *_puzzle_name);
 void Set_key_id(int _key_id);
 int Get_id();
 int Get_frame();
 int Get_player_map_pos_x();
 int Get_player_map_pos_y();
 char *Get_map_name();
 char *Get_puzzle_name();
 int Get_key_id();
 int Get_frame_change_elapsed();
 bool Is_done();
 void Decrease_remaining_duration();
 void Update_frame(int _number_of_frames,int _frame_change_delay);
};

class Interactive_map_texture
{
 private:
 int id;
 std::vector<std::pair<int,int> > target_map_position;
 std::vector<Map_texture_id> map_textures_ids;
 int current_map_texture_id_position,number_of_map_textures_ids_positions;
 bool done=false;

 public:
 Interactive_map_texture();
 ~Interactive_map_texture();
 void Clear();
 void Load();
 void Load(int _id);
 void Set_id(int _id);
 void Set_texture_frame(int _frame);
 void Set_texture_player_map_pos(int _texture,int x,int y);
 void Set_texture_map_name(int _texture,char *_map_name);
 void Set_texture_puzzle_name(int _texture,char *_map_name);
 void Set_key_id(int _texture,int _key_id);
 int Get_id();
 int Get_texture_id();
 int Get_texture_id(int _texture_position);
 void Get_all_textures_ids(std::vector<Map_texture_id> &textures_ids);
 void Get_all_targets_map_positions(std::vector<std::pair<int,int> > &_target_map_position);
 int Get_texture_frame();
 int Get_texture_player_map_pos_x();
 int Get_texture_player_map_pos_y();
 int Get_key_id();
 char *Get_texture_map_name();
 char *Get_texture_puzzle_name();
 int Get_texture_frame_change_elapsed();
 bool Is_done();
 void Decrease_remaining_duration();
 void Update_texture_frame(int _number_of_frames,int _frame_change_delay);

 void Add_target(int x,int y);
 void Trigger();
};

#endif // INTERACTIVE_MAP_TEXTURE_H
