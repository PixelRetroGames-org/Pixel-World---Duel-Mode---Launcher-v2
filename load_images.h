#ifndef LOAD_IMAGES_H
#define LOAD_IMAGES_H

//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_thread.h"
//Library
#include "library.h"

#include "big_background.h"

///GLOBAL
extern SDL_Surface *COIN,*HEART,*MANA;
extern bool GLOBAL_IMAGES_LOADED;
void Load_global_images();
void Clear_global_images();

///LAUNCHER
extern Big_Background LAUNCHER_BBACKGROUND;
extern SDL_Surface *MENU_background,*MENU_background_click,*MENU_background_selected;
extern SDL_Rect MENU_big_background_image_position;
extern bool LAUNCHER_IMAGES_LOADED,MENU_IMAGES_LOADED;
void Load_menu_images();
void Clear_menu_images();

///SHOP
extern SDL_Surface *SHOP_title_clear,*SHOP_title_background_line,*SHOP_title_background_click;
extern SDL_Surface *SHOP_title_background_click_right_frame,*SHOP_title_background_right_frame;
extern SDL_Surface *SHOP_title_background_selected,*SHOP_title_background,*SHOP_shop_page_background,*SHOP_shop_big_background;
extern SDL_Surface *SHOP_shop_rope,*SHOP_shop_background,*SHOP_shop_background_selected,*SHOP_description_background;
extern SDL_Surface *SHOP_inventory_background,*SHOP_item_background_selected,*SHOP_item_background;
extern SDL_Surface *SHOP_inventory_spell_background,*SHOP_inventory_spell_background_equipped;
extern SDL_Surface *INVENTORY_EQUIP,*INVENTORY_EQUIPPED,*INVENTORY_SELL;
extern SDL_Surface *INVENTORY_spell_position[4];
extern bool SHOP_IMAGES_LOADED;
void Load_shop_images();
void Clear_shop_images();

///PLAYER
extern SDL_Surface *PLAYER_name_background,*PLAYER_name_background_right_layer,*PLAYER_details_background,*PLAYER_experience_background,*PLAYER_money_background;
extern SDL_Surface *PLAYER_HP_background,*PLAYER_MANA_background,*PLAYER_CASE_background,*PLAYER_CASE_front;
extern SDL_Surface *PLAYER_name_background_shop;
extern SDL_Surface *PLAYER_SPELLS_background,*PLAYER_SPELLS_front,*PLAYER_SPELLS_no_mana,*PLAYER_SPELLS_not_ready;
extern bool PLAYER_IMAGES_LOADED;
void Load_player_images();
void Clear_player_images();

///LEVEL
extern SDL_Surface *LEVEL_background_image,*LEVEL_loading_image;
extern SDL_Surface *LEVEL_WINNER,*LEVEL_LOSER,*LEVEL_MONEY,*LEVEL_XP,*LEVEL_LINE,*LEVEL_WINS;
extern SDL_Surface *MAP_NAME_background,*SKEPTIC_VISION_image;
extern bool LEVEL_IMAGES_LOADED;
void Load_level_images();
void Clear_level_images();

///SETTINGS
extern SDL_Surface *SETTINGS_option_background,*SETTINGS_option_background_selected,*SETTINGS_background,*SETTINGS_name;
extern bool SETTINGS_IMAGES_LOADED;
void Load_settings_images();
void Clear_settings_images();

///SCRIPT
extern SDL_Surface *SCRIPT_default_background_image;
extern bool SCRIPT_IMAGES_LOADED;
void Load_script_images();
void Clear_script_images();

///ALL
void Load_all_images();
void Clear_all_images();

///Loading Screen
extern SDL_Surface *static_screen;
extern bool Loading_image_quit;
extern SDL_mutex *loading_image_mutex;
int Loading_image(void *data);


#endif // LOAD_IMAGES_H
