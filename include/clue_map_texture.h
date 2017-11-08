#ifndef CLUE_MAP_POSITION_H
#define CLUE_MAP_POSITION_H

#include "library.h"
#include "texture.h"
#include "interactive_map_texture.h"

#include <bitset>

class Clue_map_texture
{
 private:
 int id;
 Interactive_map_texture interactive_map_texture;
 int type=0;
 char script_name[TEXT_LENGTH_MAX];
 std::bitset<NUMBER_OF_MAX_KEYS> keys_to_give,keys_to_take;

 public:
 void Clear();
 void Load();
 void Load(int _id);
 void Set_id(int _id);
 Interactive_map_texture *Get_interactive_map_texture();
 std::bitset<NUMBER_OF_MAX_KEYS> *Get_keys();
 std::bitset<NUMBER_OF_MAX_KEYS> *Get_keys_to_take();
 int Get_id();
 int Get_type();
 char *Get_script_name();
 void Start_action(Texture *_screen=NULL);
};

#endif // CLUE_MAP_POSITION_H
