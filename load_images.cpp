#include "load_images.h"
#include "library.h"

///GLOBAL
SDL_Surface *COIN,*HEART,*MANA;
bool GLOBAL_IMAGES_LOADED;

///LAUNCHER
Big_Background LAUNCHER_BBACKGROUND;
SDL_Surface *MENU_big_background=NULL;
SDL_Surface *MENU_background=NULL,*MENU_background_click=NULL,*MENU_background_selected=NULL;
bool LAUNCHER_IMAGES_LOADED,MENU_IMAGES_LOADED;

///SHOP
SDL_Surface *SHOP_title_clear,*SHOP_title_background_line,*SHOP_title_background_click;
SDL_Surface *SHOP_title_background_click_right_frame,*SHOP_title_background_right_frame;
SDL_Surface *SHOP_title_background_selected,*SHOP_title_background,*SHOP_shop_page_background,*SHOP_shop_big_background;
SDL_Surface *SHOP_shop_rope,*SHOP_shop_background,*SHOP_shop_background_selected,*SHOP_description_background;
SDL_Surface *SHOP_inventory_background,*SHOP_item_background_selected,*SHOP_item_background;
SDL_Surface *SHOP_inventory_spell_background,*SHOP_inventory_spell_background_equipped;
SDL_Surface *INVENTORY_EQUIP,*INVENTORY_EQUIPPED,*INVENTORY_SELL;
SDL_Surface *INVENTORY_spell_position[4];
bool SHOP_IMAGES_LOADED;
const SDL_Color EQUIP_COLOR={15,30,90},BUY_COLOR={40,80,160},EQUIPPED_COLOR={255,128,0};

///PLAYER
SDL_Surface *PLAYER_name_background,*PLAYER_name_background_right_layer,*PLAYER_details_background,*PLAYER_experience_background,*PLAYER_money_background;
SDL_Surface *PLAYER_HP_background,*PLAYER_MANA_background,*PLAYER_CASE_background,*PLAYER_CASE_front;
SDL_Surface *PLAYER_name_background_shop;
SDL_Surface *PLAYER_SPELLS_background,*PLAYER_SPELLS_front,*PLAYER_SPELLS_no_mana,*PLAYER_SPELLS_not_ready;
bool PLAYER_IMAGES_LOADED;

///LEVEL
SDL_Surface *LEVEL_background_image,*LEVEL_loading_image;
SDL_Surface *LEVEL_WINNER,*LEVEL_LOSER,*LEVEL_MONEY,*LEVEL_XP,*LEVEL_LINE,*LEVEL_WINS;
SDL_Surface *MAP_NAME_background,*SKEPTIC_VISION_image;
bool LEVEL_IMAGES_LOADED;

///JOURNAL
SDL_Surface *JOURNAL_ENTRY_BACKGROUND,*JOURNAL_ENTRY_HOVER_BACKGROUND,*JOURNAL_ENTRY_CLICK_BACKGROUND;
bool JOURNAL_IMAGES_LOADED;

///SETTINGS
SDL_Surface *SETTINGS_option_background,*SETTINGS_option_background_selected,*SETTINGS_background,*SETTINGS_name;
bool SETTINGS_IMAGES_LOADED;

///SCRIPT
SDL_Surface *SCRIPT_default_background_image;
bool SCRIPT_IMAGES_LOADED;

void Load_shop_images()
{
 if(SHOP_IMAGES_LOADED)
    return;
 SHOP_IMAGES_LOADED=true;
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

void Clear_shop_images()
{
 if(!SHOP_IMAGES_LOADED)
    return;
 SDL_FreeSurface(SHOP_title_clear);
 SDL_FreeSurface(SHOP_title_background_line);
 SDL_FreeSurface(SHOP_title_background_click);
 SDL_FreeSurface(SHOP_title_background_click_right_frame);
 SDL_FreeSurface(SHOP_title_background_right_frame);
 SDL_FreeSurface(SHOP_title_background_selected);
 SDL_FreeSurface(SHOP_title_background);
 SDL_FreeSurface(SHOP_shop_page_background);
 SDL_FreeSurface(SHOP_shop_big_background);
 SDL_FreeSurface(SHOP_shop_rope);
 SDL_FreeSurface(SHOP_shop_background);
 SDL_FreeSurface(SHOP_shop_background_selected);
 SDL_FreeSurface(SHOP_description_background);
 SDL_FreeSurface(SHOP_inventory_background);
 SDL_FreeSurface(SHOP_item_background_selected);
 SDL_FreeSurface(SHOP_item_background);
 SDL_FreeSurface(SHOP_inventory_spell_background);
 SDL_FreeSurface(SHOP_inventory_spell_background_equipped);
 SDL_FreeSurface(INVENTORY_EQUIP);
 SDL_FreeSurface(INVENTORY_EQUIPPED);
 SDL_FreeSurface(INVENTORY_SELL);
 for(int i=0;i<4;i++)
     SDL_FreeSurface(INVENTORY_spell_position[i]);
 SHOP_IMAGES_LOADED=false;
}

void Load_player_images()
{
 if(PLAYER_IMAGES_LOADED)
    return;
 PLAYER_IMAGES_LOADED=true;
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

void Clear_player_images()
{
 if(!PLAYER_IMAGES_LOADED)
    return;
 SDL_FreeSurface(PLAYER_name_background);
 SDL_FreeSurface(PLAYER_name_background_shop);
 SDL_FreeSurface(PLAYER_name_background_right_layer);
 SDL_FreeSurface(PLAYER_details_background);
 SDL_FreeSurface(PLAYER_experience_background);
 SDL_FreeSurface(PLAYER_money_background);
 SDL_FreeSurface(PLAYER_HP_background);
 SDL_FreeSurface(PLAYER_MANA_background);
 SDL_FreeSurface(PLAYER_CASE_background);
 SDL_FreeSurface(PLAYER_CASE_front);
 SDL_FreeSurface(PLAYER_SPELLS_background);
 SDL_FreeSurface(PLAYER_SPELLS_front);
 SDL_FreeSurface(PLAYER_SPELLS_no_mana);
 SDL_FreeSurface(PLAYER_SPELLS_not_ready);
 PLAYER_IMAGES_LOADED=false;
}

void Load_level_images()
{
 if(LEVEL_IMAGES_LOADED)
    return;
 loading_image_mutex=SDL_CreateMutex();
 LEVEL_IMAGES_LOADED=true;
 LEVEL_background_image=load_image("images/game/background.bmp");
 LEVEL_loading_image=make_it_transparent("images/game/loading.bmp");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",50),*font1=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Color winner_color={0,205,0},loser_color={207,0,0},xp_color={75,0,130},MONEY_COLOR={125,125,125},wins_color={241,188,48};
 LEVEL_WINNER=TTF_RenderText_Solid(font,"Winner",winner_color);
 LEVEL_LOSER=TTF_RenderText_Solid(font,"Loser",loser_color);
 LEVEL_WINS=TTF_RenderText_Solid(font1,"WINS:   ",wins_color);
 LEVEL_XP=TTF_RenderText_Solid(font1,"XP:   ",xp_color);
 LEVEL_MONEY=TTF_RenderText_Solid(font1,"MONEY:   ",MONEY_COLOR);
 LEVEL_LINE=make_it_transparent("images/game/line.bmp");
 MAP_NAME_background=make_it_transparent("images/game/map_name_background.bmp");
 SKEPTIC_VISION_image=make_it_transparent("images/game/skeptic_vision.bmp");
 SDL_SetAlpha(SKEPTIC_VISION_image,SDL_SRCALPHA,0);
 TTF_CloseFont(font);
 TTF_CloseFont(font1);
}

void Clear_level_images()
{
 if(!LEVEL_IMAGES_LOADED)
    return;
 SDL_DestroyMutex(loading_image_mutex);
 SDL_FreeSurface(LEVEL_background_image);
 SDL_FreeSurface(LEVEL_loading_image);
 SDL_FreeSurface(LEVEL_WINNER);
 SDL_FreeSurface(LEVEL_LOSER);
 SDL_FreeSurface(LEVEL_XP);
 SDL_FreeSurface(LEVEL_MONEY);
 SDL_FreeSurface(LEVEL_LINE);
 SDL_FreeSurface(MAP_NAME_background);
 SDL_FreeSurface(LEVEL_WINS);
 SDL_FreeSurface(SKEPTIC_VISION_image);
 LEVEL_IMAGES_LOADED=false;
}

void Load_journal_images()
{
 if(JOURNAL_IMAGES_LOADED)
    return;
 LEVEL_IMAGES_LOADED=true;
 JOURNAL_ENTRY_BACKGROUND=make_it_transparent("images/journal/background.bmp");
 JOURNAL_ENTRY_CLICK_BACKGROUND=make_it_transparent("images/journal/click_background.bmp");
 JOURNAL_ENTRY_HOVER_BACKGROUND=make_it_transparent("images/journal/hover_background.bmp");
}

void Clear_journal_images()
{
 if(!JOURNAL_IMAGES_LOADED)
    return;
 JOURNAL_IMAGES_LOADED=false;
 SDL_FreeSurface(JOURNAL_ENTRY_BACKGROUND);
 SDL_FreeSurface(JOURNAL_ENTRY_CLICK_BACKGROUND);
 SDL_FreeSurface(JOURNAL_ENTRY_HOVER_BACKGROUND);
}

void Load_global_images()
{
 if(GLOBAL_IMAGES_LOADED)
    return;
 GLOBAL_IMAGES_LOADED=true;
 COIN=make_it_transparent("images/shop/coin.bmp");
}

void Clear_global_images()
{
 if(!GLOBAL_IMAGES_LOADED)
    return;
 SDL_FreeSurface(COIN);
 GLOBAL_IMAGES_LOADED=false;
}

void Load_settings_images()
{
 if(SETTINGS_IMAGES_LOADED)
    return;
 SETTINGS_IMAGES_LOADED=true;
 SETTINGS_option_background=load_image("images/settings/option_background.bmp");
 SETTINGS_option_background_selected=load_image("images/settings/option_background_selected.bmp");
 SETTINGS_background=load_image("images/settings/background.bmp");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",40);
 SETTINGS_name=TTF_RenderText_Solid(font,"Settings",SDL_Color{255,255,255});
}

void Clear_settings_images()
{
 if(!SETTINGS_IMAGES_LOADED)
    return;
 SDL_FreeSurface(SETTINGS_option_background);
 SDL_FreeSurface(SETTINGS_option_background_selected);
 SDL_FreeSurface(SETTINGS_background);
 SDL_FreeSurface(SETTINGS_name);
 SETTINGS_IMAGES_LOADED=false;
}

void Load_menu_images()
{
 if(MENU_IMAGES_LOADED)
    return;
 MENU_IMAGES_LOADED=true;
 MENU_big_background=load_image("images/menu/menu_big_background.bmp");
 MENU_background=load_image("images/menu/menu_background.bmp");
 MENU_background_click=load_image("images/menu/menu_background_click.bmp");
 MENU_background_selected=load_image("images/menu/menu_background_selected.bmp");
}

void Clear_menu_images()
{
 if(!MENU_IMAGES_LOADED)
    return;
 SDL_FreeSurface(MENU_big_background);
 SDL_FreeSurface(MENU_background);
 SDL_FreeSurface(MENU_background_click);
 SDL_FreeSurface(MENU_background_selected);
 MENU_IMAGES_LOADED=false;
}

void Load_script_images()
{
 if(SCRIPT_IMAGES_LOADED)
    return;
 SCRIPT_IMAGES_LOADED=true;
 SCRIPT_default_background_image=make_it_transparent("images/game/empty.bmp");
}

void Clear_script_images()
{
 if(!SCRIPT_IMAGES_LOADED)
    return;
 SDL_FreeSurface(SCRIPT_default_background_image);
 SCRIPT_IMAGES_LOADED=false;
}

void Load_all_images()
{
 Load_global_images();
 Load_menu_images();
 Load_shop_images();
 Load_player_images();
 Load_level_images();
 Load_journal_images();
 Load_settings_images();
 Load_script_images();
}

void Clear_all_images()
{
 Clear_global_images();
 Clear_menu_images();
 Clear_shop_images();
 Clear_player_images();
 Clear_level_images();
 Clear_journal_images();
 Clear_settings_images();
 Clear_script_images();
}

SDL_Surface *static_screen;
bool Loading_image_quit=false;
SDL_mutex *loading_image_mutex;
int Loading_image(void *data)
{
 int frame=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=false;
 while(!Loading_image_quit)
       {
        SDL_UnlockMutex(loading_image_mutex);
        apply_surface(0,0,LEVEL_background_image,static_screen);
        apply_surface(160*frame,0,((static_screen->w)-160)/2,((static_screen->h)-LEVEL_loading_image->h)/2,160,LEVEL_loading_image->h,LEVEL_loading_image,static_screen);
        SDL_Flip(static_screen);
        SDL_Delay(100);
        frame++;
        frame%=8;
        SDL_LockMutex(loading_image_mutex);
       }
}

bool splash_screen_quit;
SDL_mutex *splash_screen_mutex;
const int FOG_UPDATE_INTERVAL=100,FOG_OPACITY_DECREASE_INTERVAL=200;
const int FRAMES_PER_SECOND=30,FRAMES_PER_SECOND_FIRE=25;
int Splash_Screen(void *data)
{
 SDL_LockMutex(splash_screen_mutex);
 splash_screen_quit=false;
 Darkness fog[2];
 fog[1].Set_image_name("launcher_fog");
 fog[1].Load_image();
 fog[1].Set_number_of_frames(8);
 fog[1].Set_aplha(SDL_ALPHA_OPAQUE/2);
 fog[1].Update_image();
 fog[1].Set_frameW(1920);
 fog[0].Set_image_name("launcher_fog1");
 fog[0].Load_image();
 fog[0].Set_number_of_frames(8);
 fog[0].Set_aplha(SDL_ALPHA_OPAQUE/2);
 fog[0].Update_image();
 fog[0].Set_frameW(1920);
 SDL_Rect area;
 area.x=area.y=0;
 area.w=RESOLUTION_X;
 area.h=RESOLUTION_Y;
 Timer fog_update_timer,fog_opacity_decrease_timer,fps,fps1;
 fog_update_timer.start();
 fog_opacity_decrease_timer.start();
 fps1.start();
 int poz=1;
 while(!splash_screen_quit || fog[poz].Get_alpha()!=0)
       {
        fps.start();
        SDL_UnlockMutex(splash_screen_mutex);
        LAUNCHER_BBACKGROUND.Print_image(0,0,static_screen);
        LAUNCHER_BBACKGROUND.Update_image_frame();
        if(fps1.get_ticks()>=1000/FRAMES_PER_SECOND_FIRE)
           {
            LAUNCHER_BBACKGROUND.Update_fire_frame();
            fps1.start();
           }
        fog[poz].Enshroud(area,static_screen);
        if(fog_opacity_decrease_timer.get_ticks()>=FOG_OPACITY_DECREASE_INTERVAL)
           {
            fog_opacity_decrease_timer.start();
            fog[poz].Decrease();
           }
        if(fog_update_timer.get_ticks()>=FOG_UPDATE_INTERVAL)
           {
            fog[poz].Update_frame();
            fog_update_timer.start();
           }
        fog[poz].Update_image();
        SDL_Flip(static_screen);
        if(fog[poz].Get_frame()==7)
           {
            poz++;
            poz%=2;
            fog[poz].Set_aplha(fog[(poz+1)%2].Get_alpha());
            fog[poz].Set_current_frame(0);
            fog[poz].Update_image();
           }
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND)
           {
            SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
           }
        SDL_LockMutex(splash_screen_mutex);
       }
 fog[0].Clear();
 fog[1].Clear();
}
