#include "load_images.h"

///GLOBAL
SDL_Surface *COIN,*HEART,*MANA;

///LAUNCHER
Big_Background LAUNCHER_BBACKGROUND;
SDL_Surface *MENU_big_background=NULL;
SDL_Surface *MENU_background=NULL,*MENU_background_click=NULL,*MENU_background_selected=NULL;

///SHOP
SDL_Surface *SHOP_title_clear,*SHOP_title_background_line,*SHOP_title_background_click;
SDL_Surface *SHOP_title_background_click_right_frame,*SHOP_title_background_right_frame;
SDL_Surface *SHOP_title_background_selected,*SHOP_title_background,*SHOP_shop_page_background,*SHOP_shop_big_background;
SDL_Surface *SHOP_shop_rope,*SHOP_shop_background,*SHOP_shop_background_selected,*SHOP_description_background;

///PLAYER
SDL_Surface *PLAYER_name_background,*PLAYER_name_background_right_layer,*PLAYER_details_background,*PLAYER_experience_background,*PLAYER_money_background;
SDL_Surface *SHOP_inventory_background,*SHOP_item_background_selected,*SHOP_item_background;
SDL_Surface *PLAYER_HP_background,*PLAYER_MANA_background;

///SETTINGS
SDL_Surface *SETTINGS_option_background,*SETTINGS_option_background_selected,*SETTINGS_background,*SETTINGS_name;

///SCRIPT
SDL_Surface *SCRIPT_default_background_image;

void Load_shop_images()
{
 #ifdef SHOP_IMAGES_LOADED
 return;
 #endif // SHOP_IMAGES_LOADED
 #define SHOP_IMAGES_LOADED
 SHOP_title_clear=make_it_transparent("images/shop/title_clear.bmp");
 SHOP_title_background_line=make_it_transparent("images/shop/title_background_line.bmp");
 SHOP_title_background_click=make_it_transparent("images/shop/title_background_click.bmp");
 SHOP_title_background_click_right_frame=make_it_transparent("images/shop/title_background_click_right_frame.bmp");
 SHOP_title_background_right_frame=make_it_transparent("images/shop/title_background_right_frame.bmp");
 SHOP_title_background_selected=make_it_transparent("images/shop/title_background_selected.bmp");
 SHOP_title_background=make_it_transparent("images/shop/title_background.bmp");
 SHOP_shop_page_background=SDL_LoadBMP("images/shop/shop_page_background.bmp");
 SHOP_shop_big_background=SDL_LoadBMP("images/shop/shop_big_background.bmp");
 SHOP_shop_rope=make_it_transparent("images/shop/shop_rope.bmp");
 SHOP_shop_background=SDL_LoadBMP("images/shop/shop_background.bmp");
 SHOP_shop_background_selected=SDL_LoadBMP("images/shop/shop_background_selected.bmp");
 SHOP_description_background=make_it_transparent("images/shop/description_background.bmp");
 SHOP_inventory_background=make_it_transparent("images/shop/inventory_background.bmp");
 SHOP_item_background_selected=SDL_LoadBMP("images/shop/item_background_selected.bmp");
 SHOP_item_background=SDL_LoadBMP("images/shop/item_background.bmp");
}

void Load_player_images()
{
 #ifdef PLAYER_IMAGES_LOADED
 return;
 #endif // PLAYER_IMAGES_LOADED
 #define PLAYER_IMAGES_LOADED
 PLAYER_name_background=SDL_LoadBMP("images/player/name_background.bmp");
 PLAYER_details_background=SDL_LoadBMP("images/player/details_background.bmp");
 PLAYER_experience_background=SDL_LoadBMP("images/player/experience_background.bmp");
 PLAYER_money_background=SDL_LoadBMP("images/player/money_background.bmp");
}

void Load_global_images()
{
 #ifdef GLOBAL_IMAGES_LOADED
 return;
 #endif //GLOBAL_IMAGES_LOADED
 #define GLOBAL_IMAGES_LOADED
 COIN=make_it_transparent("images/shop/coin.bmp");
}

void Load_settings_images()
{
 #ifdef SETTINGS_IMAGES_LOADED
 return;
 #endif // SETTINGS_IMAGES_LOADED
 #define SETTINGS_IMAGES_LOADED
 SETTINGS_option_background=SDL_LoadBMP("images/settings/option_background.bmp");
 SETTINGS_option_background_selected=SDL_LoadBMP("images/settings/option_background_selected.bmp");
 SETTINGS_background=SDL_LoadBMP("images/settings/background.bmp");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",40);
 SETTINGS_name=TTF_RenderText_Solid(font,"Settings",SDL_Color{255,255,255});
}

void Load_menu_images()
{
 #ifdef MENU_IMAGES_LOADED
 return;
 #endif // MENU_IMAGES_LOADED
 #define MENU_IMAGES_LOADED
 MENU_big_background=SDL_LoadBMP("images/menu/menu_big_background.bmp");
 MENU_background=SDL_LoadBMP("images/menu/menu_background.bmp");
 MENU_background_click=SDL_LoadBMP("images/menu/menu_background_click.bmp");
 MENU_background_selected=SDL_LoadBMP("images/menu/menu_background_selected.bmp");
}

void Load_script_images()
{
 #ifdef SCRIPT_IMAGES_LOADED
 return;
 #endif // SCRIPT_IMAGES_LOADED
 #define SCRIPT_IMAGES_LOADED
 SCRIPT_default_background_image=make_it_transparent("images/script/default_background_image.bmp");
}

void Load_all_images()
{
 Load_global_images();
 Load_menu_images();
 Load_shop_images();
 Load_player_images();
 Load_settings_images();
 Load_script_images();
}
