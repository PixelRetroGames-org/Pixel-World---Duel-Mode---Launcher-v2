#ifndef LOAD_IMAGES_H
#define LOAD_IMAGES_H

//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
//Library
#include "library.h"

#include "big_background.h"

///GLOBAL
extern SDL_Surface *COIN,*HEART,*MANA;
void Load_global_images();

///LAUNCHER
extern Big_Background LAUNCHER_BBACKGROUND;
extern SDL_Surface *MENU_background,*MENU_background_click,*MENU_background_selected;
extern SDL_Rect MENU_big_background_image_position;
void Load_menu_images();

///SHOP
extern SDL_Surface *SHOP_title_clear,*SHOP_title_background_line,*SHOP_title_background_click;
extern SDL_Surface *SHOP_title_background_click_right_frame,*SHOP_title_background_right_frame;
extern SDL_Surface *SHOP_title_background_selected,*SHOP_title_background,*SHOP_shop_page_background,*SHOP_shop_big_background;
extern SDL_Surface *SHOP_shop_rope,*SHOP_shop_background,*SHOP_shop_background_selected,*SHOP_description_background;
extern SDL_Surface *INVENTORY_EQUIP,*INVENTORY_EQUIPPED,*INVENTORY_SELL;
void Load_shop_images();

///PLAYER
extern SDL_Surface *PLAYER_name_background,*PLAYER_name_background_right_layer,*PLAYER_details_background,*PLAYER_experience_background,*PLAYER_money_background;
extern SDL_Surface *SHOP_inventory_background,*SHOP_item_background_selected,*SHOP_item_background;
extern SDL_Surface *PLAYER_HP_background,*PLAYER_MANA_background,*PLAYER_CASE_background,*PLAYER_CASE_front;
void Load_player_images();

///LEVEL
extern SDL_Surface *LEVEL_background_image,*LEVEL_loading_image;
extern SDL_Surface *LEVEL_WINNER,*LEVEL_LOSER,*LEVEL_MONEY,*LEVEL_XP,*LEVEL_LINE;
void Load_level_images();

///SETTINGS
extern SDL_Surface *SETTINGS_option_background,*SETTINGS_option_background_selected,*SETTINGS_background,*SETTINGS_name;
void Load_settings_images();

///SCRIPT
extern SDL_Surface *SCRIPT_default_background_image;
void Load_script_images();

///ALL
void Load_all_images();

#endif // LOAD_IMAGES_H
