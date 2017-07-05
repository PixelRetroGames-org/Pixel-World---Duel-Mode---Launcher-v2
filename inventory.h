#ifndef INVENTORY_H
#define INVENTORY_H

#include "player.h"
#include "shop.h"
#include "load_images.h"

namespace player_inventory
{
 const int FRAMES_PER_SECOND=60;
 void Print_Inventory(Texture *screen,char *_player_name);
}
#endif // INVENTORY_H
