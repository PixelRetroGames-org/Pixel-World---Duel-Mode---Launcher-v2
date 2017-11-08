#ifndef LOAD_IMAGES_H
#define LOAD_IMAGES_H

#include "library.h"
#include "texture.h"
#include "big_background.h"
#include "settings.h"
#include "darkness.h"

///GLOBAL
extern Texture *COIN,*HEART,*MANA;
extern bool GLOBAL_IMAGES_LOADED;
void Load_global_images();
void Clear_global_images();

///LAUNCHER
extern Big_Background LAUNCHER_BBACKGROUND;
extern Texture *MENU_background,*MENU_background_click,*MENU_background_selected;
extern bool LAUNCHER_IMAGES_LOADED,MENU_IMAGES_LOADED;
void Load_menu_images();
void Clear_menu_images();

///SHOP
extern Texture *SHOP_title_clear,*SHOP_title_background_line,*SHOP_title_background_click;
extern Texture *SHOP_title_background_click_right_frame,*SHOP_title_background_right_frame;
extern Texture *SHOP_title_background_selected,*SHOP_title_background,*SHOP_shop_page_background,*SHOP_shop_big_background;
extern Texture *SHOP_shop_rope,*SHOP_shop_background,*SHOP_shop_background_selected,*SHOP_description_background;
extern Texture *SHOP_inventory_background,*SHOP_item_background_selected,*SHOP_item_background;
extern Texture *SHOP_inventory_spell_background,*SHOP_inventory_spell_background_equipped;
extern Texture *INVENTORY_EQUIP,*INVENTORY_EQUIPPED,*INVENTORY_SELL;
extern Texture *INVENTORY_spell_position[4];
extern bool SHOP_IMAGES_LOADED;
void Load_shop_images();
void Clear_shop_images();

///PLAYER
extern Texture *PLAYER_name_background,*PLAYER_name_background_right_layer,*PLAYER_details_background,*PLAYER_experience_background,*PLAYER_money_background;
extern Texture *PLAYER_HP_background,*PLAYER_MANA_background,*PLAYER_CASE_background,*PLAYER_CASE_front;
extern Texture *PLAYER_name_background_shop;
extern Texture *PLAYER_SPELLS_background,*PLAYER_SPELLS_front,*PLAYER_SPELLS_no_mana,*PLAYER_SPELLS_not_ready;
extern bool PLAYER_IMAGES_LOADED;
void Load_player_images();
void Clear_player_images();

///LEVEL
extern Texture *LEVEL_background_image,*LEVEL_loading_image,*LEVEL_meditation_image;
extern Texture *LEVEL_WINNER,*LEVEL_LOSER,*LEVEL_MONEY,*LEVEL_XP,*LEVEL_LINE,*LEVEL_WINS;
extern Texture *MAP_NAME_background,*SKEPTIC_VISION_image;
extern bool LEVEL_IMAGES_LOADED;
void Load_level_images();
void Clear_level_images();

///SETTINGS
extern Texture *SETTINGS_option_background,*SETTINGS_option_background_selected,*SETTINGS_background,*SETTINGS_name;
extern bool SETTINGS_IMAGES_LOADED;
void Load_settings_images();
void Clear_settings_images();

///SCRIPT
extern Texture *SCRIPT_default_background_image;
extern bool SCRIPT_IMAGES_LOADED;
void Load_script_images();
void Clear_script_images();

///JOURNAL
extern Texture *JOURNAL_ENTRY_BACKGROUND,*JOURNAL_ENTRY_HOVER_BACKGROUND,*JOURNAL_ENTRY_CLICK_BACKGROUND;
extern bool JOURNAL_IMAGES_LOADED;
void Load_journal_images();
void Clear_journal_images();

///INVENTORY
extern Texture *INVENTORY_LARROW,*INVENTORY_RARROW;
extern bool INVENTORY_IMAGES_LOADED;
void Load_inventory_images();
void Clear_inventory_images();

///CONTROLS SCREEN
extern Texture *CONTROLS_keyboard_image,*CONTROLS_controller_image,*CONTROLS_background_image;
extern bool CONTROLS_SCREEN_IMAGES_LOADED;
void Load_controls_screen_images();
void Clear_controls_screen_images();

///ALL
void Load_all_images();
void Clear_all_images();

///Loading Screen
extern Texture *static_screen;
extern bool Loading_image_quit;
extern SDL_mutex *loading_image_mutex;
int Loading_image(void *data);

///Meditation Screen
int Meditation_Screen(void *data);

///Splash Screen
extern bool splash_screen_quit;
extern SDL_mutex *splash_screen_mutex;
int Splash_Screen(void *data);


#endif // LOAD_IMAGES_H
