#ifndef CLUE_MAP_POSITION_H
#define CLUE_MAP_POSITION_H

#include "library.h"
#include "interactive_map_texture.h"

class Clue_map_position
{
 private:
 Interactive_map_texture interactive_map_texture;
 int type=0;
 char action_name[TEXT_LENGTH_MAX];


 public:
 Interactive_map_texture *Get_interactive_map_texture();

}

#endif // CLUE_MAP_POSITION_H
