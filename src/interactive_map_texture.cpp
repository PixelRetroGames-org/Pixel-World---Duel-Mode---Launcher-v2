#include "interactive_map_texture.h"

///Map texture id
Map_texture_id::Map_texture_id()
{
 id=duration=remaining_duration=0;
 player_map_pos_x=player_map_pos_y=-1;
}

void Map_texture_id::Set_id(int _id)
{
 id=_id;
}

void Map_texture_id::Set_duration(int _duration)
{
 remaining_duration=duration=_duration;
}

void Map_texture_id::Set_frame(int _frame)
{
 frame=_frame;
}

void Map_texture_id::Set_frame_change_elapsed(int _frame_change_elapsed)
{
 frame_change_elapsed=_frame_change_elapsed;
}

void Map_texture_id::Set_player_map_pos(int x,int y)
{
 player_map_pos_x=x;
 player_map_pos_y=y;
}

void Map_texture_id::Set_map_name(char *_map_name)
{
 strcpy(map_name,_map_name);
}

void Map_texture_id::Set_puzzle_name(char *_puzzle_name)
{
 strcpy(puzzle_name,_puzzle_name);
}

void Map_texture_id::Set_key_id(int _key_id)
{
 key_id=_key_id;
}

int Map_texture_id::Get_id()
{
 return id;
}

int Map_texture_id::Get_frame()
{
 return frame;
}

int Map_texture_id::Get_player_map_pos_x()
{
 return player_map_pos_x;
}

int Map_texture_id::Get_player_map_pos_y()
{
 return player_map_pos_y;
}

char *Map_texture_id::Get_map_name()
{
 return map_name;
}

char *Map_texture_id::Get_puzzle_name()
{
 return puzzle_name;
}

int Map_texture_id::Get_key_id()
{
 return key_id;
}

int Map_texture_id::Get_frame_change_elapsed()
{
 return frame_change_elapsed;
}

bool Map_texture_id::Is_done()
{
 return remaining_duration==0;
}

void Map_texture_id::Decrease_remaining_duration()
{
 if(remaining_duration>0)
    remaining_duration--;
}

void Map_texture_id::Update_frame(int _number_of_frames,int _frame_change_delay)
{
 frame_change_elapsed++;
 if(frame_change_elapsed>=_frame_change_delay)
    {
     frame++;
     frame%=_number_of_frames;
     frame_change_elapsed=0;
    }
}

///Interactive map texture
Interactive_map_texture::Interactive_map_texture()
{
 std::vector<std::pair<int,int> >().swap(target_map_position);
 //target_map_position.clear();
 std::vector<Map_texture_id>().swap(map_textures_ids);
 //map_textures_ids.clear();
 current_map_texture_id_position=number_of_map_textures_ids_positions=0;
}

Interactive_map_texture::~Interactive_map_texture()
{
 std::vector<std::pair<int,int> >().swap(target_map_position);
 //target_map_position.clear();
 std::vector<Map_texture_id>().swap(map_textures_ids);
 //map_textures_ids.clear();
 current_map_texture_id_position=number_of_map_textures_ids_positions=0;
}

void Interactive_map_texture::Clear()
{
 std::vector<std::pair<int,int> >().swap(target_map_position);
 //target_map_position.clear();
 std::vector<Map_texture_id>().swap(map_textures_ids);
 //map_textures_ids.clear();
 current_map_texture_id_position=number_of_map_textures_ids_positions=0;
 id=0;
}

void Interactive_map_texture::Set_id(int _id)
{
 id=_id;
}

void Interactive_map_texture::Set_texture_frame(int _frame)
{
 map_textures_ids[current_map_texture_id_position].Set_frame(_frame);
}

void Interactive_map_texture::Set_texture_player_map_pos(int _texture,int x,int y)
{
 map_textures_ids[_texture].Set_player_map_pos(x,y);
}

void Interactive_map_texture::Set_texture_map_name(int _texture,char *_map_name)
{
 map_textures_ids[_texture].Set_map_name(_map_name);
}

void Interactive_map_texture::Set_texture_puzzle_name(int _texture,char *_puzzle_name)
{
 map_textures_ids[_texture].Set_puzzle_name(_puzzle_name);
}

void Interactive_map_texture::Set_key_id(int _texture,int _key_id)
{
 map_textures_ids[_texture].Set_key_id(_key_id);
}

void Interactive_map_texture::Load()
{
 if(id==0)
    return;
 char path[TEXT_LENGTH_MAX]={NULL};
 char aux[TEXT_LENGTH_MAX]={NULL};
 itoa(id,aux);
 strcpy(path,"maps/interactive textures/");
 strcat(path,aux);
 strcat(path,".pwit");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    {
     Map_texture_id _id;
     _id.Set_id(id);
     _id.Set_duration(-1);
     _id.Set_frame(0);
     map_textures_ids.push_back(_id);
     number_of_map_textures_ids_positions=1;
     current_map_texture_id_position=0;
     return;
    }
 fscanf(where,"%d %d ",&number_of_map_textures_ids_positions,&current_map_texture_id_position);
 map_textures_ids.clear();
 Map_texture_id _id;
 for(int i=0;i<number_of_map_textures_ids_positions;i++)
     {
      int __id,duration;
      fscanf(where,"%d %d ",&__id,&duration);
      _id.Set_id(__id);
      _id.Set_duration(duration);
      _id.Set_frame(0);
      _id.Set_frame_change_elapsed(0);
      map_textures_ids.push_back(_id);
     }
 fclose(where);
}

void Interactive_map_texture::Load(int _id)
{
 Set_id(_id);
 Load();
}

int Interactive_map_texture::Get_id()
{
 return id;
}

int Interactive_map_texture::Get_texture_id()
{
 return map_textures_ids[current_map_texture_id_position].Get_id();
}

int Interactive_map_texture::Get_texture_id(int _texture_position)
{
 return map_textures_ids[_texture_position].Get_id();
}

void Interactive_map_texture::Get_all_textures_ids(std::vector<Map_texture_id> &textures_ids)
{
 textures_ids=map_textures_ids;
}

void Interactive_map_texture::Get_all_targets_map_positions(std::vector<std::pair<int,int> > &_target_map_position)
{
 _target_map_position=target_map_position;
}

int Interactive_map_texture::Get_texture_frame()
{
 return map_textures_ids[current_map_texture_id_position].Get_frame();
}

int Interactive_map_texture::Get_texture_player_map_pos_x()
{
 return map_textures_ids[current_map_texture_id_position].Get_player_map_pos_x();
}

int Interactive_map_texture::Get_texture_player_map_pos_y()
{
 return map_textures_ids[current_map_texture_id_position].Get_player_map_pos_y();
}

char *Interactive_map_texture::Get_texture_map_name()
{
 return map_textures_ids[current_map_texture_id_position].Get_map_name();
}

char *Interactive_map_texture::Get_texture_puzzle_name()
{
 return map_textures_ids[current_map_texture_id_position].Get_puzzle_name();
}

int Interactive_map_texture::Get_texture_frame_change_elapsed()
{
 return map_textures_ids[current_map_texture_id_position].Get_frame_change_elapsed();
}

int Interactive_map_texture::Get_key_id()
{
 return map_textures_ids[current_map_texture_id_position].Get_key_id();
}

bool Interactive_map_texture::Is_done()
{
 if(done)
    return done;
 if(map_textures_ids[current_map_texture_id_position].Is_done())
    {
     Trigger();
     if(map_textures_ids[current_map_texture_id_position].Is_done())
        {
         done=true;
         Load(20);
        }
    }
 return done;
}

void Interactive_map_texture::Decrease_remaining_duration()
{
 map_textures_ids[current_map_texture_id_position].Decrease_remaining_duration();
}

void Interactive_map_texture::Update_texture_frame(int _number_of_frames,int _frame_change_delay)
{
 map_textures_ids[current_map_texture_id_position].Update_frame(_number_of_frames,_frame_change_delay);
}

void Interactive_map_texture::Add_target(int x,int y)
{
 target_map_position.push_back(std::make_pair(x,y));
}

void Interactive_map_texture::Trigger()
{
 map_textures_ids[current_map_texture_id_position].Set_frame(0);
 current_map_texture_id_position++;
 current_map_texture_id_position%=number_of_map_textures_ids_positions;
}
