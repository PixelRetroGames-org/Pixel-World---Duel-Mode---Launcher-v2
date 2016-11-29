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
SDL_Surface *SHOP_inventory_background,*SHOP_item_background_selected,*SHOP_item_background;
SDL_Surface *SHOP_inventory_spell_background,*SHOP_inventory_spell_background_equipped;
SDL_Surface *INVENTORY_EQUIP,*INVENTORY_EQUIPPED,*INVENTORY_SELL;
SDL_Surface *INVENTORY_spell_position[4];
const SDL_Color EQUIP_COLOR={15,30,90},BUY_COLOR={40,80,160},EQUIPPED_COLOR={255,128,0};

///PLAYER
SDL_Surface *PLAYER_name_background,*PLAYER_name_background_right_layer,*PLAYER_details_background,*PLAYER_experience_background,*PLAYER_money_background;
SDL_Surface *PLAYER_HP_background,*PLAYER_MANA_background,*PLAYER_CASE_background,*PLAYER_CASE_front;
SDL_Surface *PLAYER_name_background_shop;
SDL_Surface *PLAYER_SPELLS_background,*PLAYER_SPELLS_front,*PLAYER_SPELLS_no_mana,*PLAYER_SPELLS_not_ready;

///LEVEL
SDL_Surface *LEVEL_background_image,*LEVEL_loading_image;
SDL_Surface *LEVEL_WINNER,*LEVEL_LOSER,*LEVEL_MONEY,*LEVEL_XP,*LEVEL_LINE;
SDL_Surface *MAP_NAME_background;

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
 SHOP_shop_page_background=load_image("images/shop/shop_page_background.bmp");
 SHOP_shop_big_background=load_image("images/shop/shop_big_background.bmp");
 SHOP_shop_rope=make_it_transparent("images/shop/shop_rope.bmp");
 SHOP_shop_background=load_image("images/shop/shop_background.bmp");
 SHOP_shop_background_selected=load_image("images/shop/shop_background_selected.bmp");
 SHOP_description_background=make_it_transparent("images/shop/description_background.bmp");
 SHOP_inventory_background=make_it_transparent("images/shop/inventory_background.bmp");
 SHOP_item_background_selected=load_image("images/shop/item_background_selected.bmp");
 SHOP_item_background=load_image("images/shop/item_background.bmp");
 SHOP_inventory_spell_background=load_image("images/shop/spell_background.bmp");
 SHOP_inventory_spell_background_equipped=load_image("images/shop/spell_background_equipped.bmp");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",15);
 SDL_Color color{65,105,225};
 INVENTORY_EQUIP=TTF_RenderText_Solid(font,"Equip",EQUIP_COLOR);
 INVENTORY_EQUIPPED=TTF_RenderText_Solid(font,"Equipped",EQUIPPED_COLOR);
 INVENTORY_SELL=TTF_RenderText_Solid(font,"Sell",BUY_COLOR);
 INVENTORY_spell_position[0]=TTF_RenderText_Solid(font,"1",color);
 INVENTORY_spell_position[1]=TTF_RenderText_Solid(font,"2",color);
 INVENTORY_spell_position[2]=TTF_RenderText_Solid(font,"3",color);
 INVENTORY_spell_position[3]=TTF_RenderText_Solid(font,"4",color);
 TTF_CloseFont(font);
}

void Load_player_images()
{
 #ifdef PLAYER_IMAGES_LOADED
 return;
 #endif // PLAYER_IMAGES_LOADED
 #define PLAYER_IMAGES_LOADED
 PLAYER_name_background=make_it_transparent("images/player/name_background.bmp");
 PLAYER_name_background_shop=make_it_transparent("images/player/name_background_shop.bmp");
 PLAYER_name_background_right_layer=load_image("images/player/name_background_right_layer.bmp");
 PLAYER_details_background=make_it_transparent("images/player/details_background.bmp");
 PLAYER_experience_background=load_image("images/player/experience_background.bmp");
 PLAYER_money_background=load_image("images/player/money_background.bmp");
 PLAYER_HP_background=make_it_transparent("images/player/hp_background.bmp");
 PLAYER_MANA_background=make_it_transparent("images/player/mana_background.bmp");
 PLAYER_CASE_background=make_it_transparent("images/player/case_background.bmp");
 PLAYER_CASE_front=make_it_transparent("images/player/case_front.bmp");
 PLAYER_SPELLS_background=make_it_transparent("images/player/spells_background.bmp");
 PLAYER_SPELLS_front=make_it_transparent("images/player/spells_front.bmp");
 PLAYER_SPELLS_no_mana=make_it_transparent("images/player/spells_no_mana.bmp");
 SDL_SetAlpha(PLAYER_SPELLS_no_mana,SDL_SRCALPHA,200);
 PLAYER_SPELLS_not_ready=make_it_transparent("images/player/spells_not_ready.bmp");
 SDL_SetAlpha(PLAYER_SPELLS_not_ready,SDL_SRCALPHA,200);
}

void Load_level_images()
{
 #ifdef LEVEL_IMAGES_LOADED
 return
 #endif // LEVEL_IMAGES_LOADED
 #define LEVEL_IMAGES_LOADED
 LEVEL_background_image=load_image("images/game/background.bmp");
 LEVEL_loading_image=make_it_transparent("images/game/loading.bmp");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",50),*font1=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Color winner_color={0,205,0},loser_color={207,0,0},xp_color={75,0,130},MONEY_COLOR={125,125,125};
 LEVEL_WINNER=TTF_RenderText_Solid(font,"Winner",winner_color);
 LEVEL_LOSER=TTF_RenderText_Solid(font,"Loser",loser_color);
 LEVEL_XP=TTF_RenderText_Solid(font1,"XP:   ",xp_color);
 LEVEL_MONEY=TTF_RenderText_Solid(font1,"MONEY:   ",MONEY_COLOR);
 LEVEL_LINE=make_it_transparent("images/game/line.bmp");
 MAP_NAME_background=make_it_transparent("images/game/map_name_background.bmp");
 TTF_CloseFont(font);
 TTF_CloseFont(font1);
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
 SETTINGS_option_background=load_image("images/settings/option_background.bmp");
 SETTINGS_option_background_selected=load_image("images/settings/option_background_selected.bmp");
 SETTINGS_background=load_image("images/settings/background.bmp");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",40);
 SETTINGS_name=TTF_RenderText_Solid(font,"Settings",SDL_Color{255,255,255});
}

void Load_menu_images()
{
 #ifdef MENU_IMAGES_LOADED
 return;
 #endif // MENU_IMAGES_LOADED
 #define MENU_IMAGES_LOADED
 MENU_big_background=load_image("images/menu/menu_big_background.bmp");
 MENU_background=load_image("images/menu/menu_background.bmp");
 MENU_background_click=load_image("images/menu/menu_background_click.bmp");
 MENU_background_selected=load_image("images/menu/menu_background_selected.bmp");
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
 Load_level_images();
 Load_settings_images();
 Load_script_images();
}

SDL_Surface *static_screen;
bool Loading_image_quit=false;
int Loading_image(void *data)
{
 int frame=0;
 Loading_image_quit=false;
 while(!Loading_image_quit)
       {
        apply_surface(0,0,LEVEL_background_image,static_screen);
        apply_surface(160*frame,0,((static_screen->w)-160)/2,((static_screen->h)-LEVEL_loading_image->h)/2,160,LEVEL_loading_image->h,LEVEL_loading_image,static_screen);
        SDL_Flip(static_screen);
        SDL_Delay(100);
        frame++;
        frame%=8;
       }
}
