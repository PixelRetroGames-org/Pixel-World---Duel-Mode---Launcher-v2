#ifndef LOAD_IMAGES_H
#define LOAD_IMAGES_H

#include "library.h"
#include "big_background.h"

extern Big_Background LAUNCHER_BBACKGROUND;

extern SDL_Surface *MENU_background,*MENU_background_click,*MENU_background_selected;

extern SDL_Rect MENU_big_background_image_position;

void Load_Menu_images();

#endif //LOAD_IMAGES_H
