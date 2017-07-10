#include "load_images.h"

///GLOBAL
Texture *COIN,*HEART,*MANA;
bool GLOBAL_IMAGES_LOADED;

///LAUNCHER
Big_Background LAUNCHER_BBACKGROUND;
Texture *MENU_background=NULL,*MENU_background_click=NULL,*MENU_background_selected=NULL;
bool LAUNCHER_IMAGES_LOADED,MENU_IMAGES_LOADED;

///SHOP
Texture *SHOP_title_clear,*SHOP_title_background_line,*SHOP_title_background_click;
Texture *SHOP_title_background_click_right_frame,*SHOP_title_background_right_frame;
Texture *SHOP_title_background_selected,*SHOP_title_background,*SHOP_shop_page_background,*SHOP_shop_big_background;
Texture *SHOP_shop_rope,*SHOP_shop_background,*SHOP_shop_background_selected,*SHOP_description_background;
Texture *SHOP_inventory_background,*SHOP_item_background_selected,*SHOP_item_background;
Texture *SHOP_inventory_spell_background,*SHOP_inventory_spell_background_equipped;
Texture *INVENTORY_EQUIP,*INVENTORY_EQUIPPED,*INVENTORY_SELL;
Texture *INVENTORY_spell_position[4];
bool SHOP_IMAGES_LOADED;
const SDL_Color EQUIP_COLOR={125,78,12},BUY_COLOR={145,145,145},EQUIPPED_COLOR={255,128,0};

///PLAYER
Texture *PLAYER_name_background,*PLAYER_name_background_right_layer,*PLAYER_details_background,*PLAYER_experience_background,*PLAYER_money_background;
Texture *PLAYER_HP_background,*PLAYER_MANA_background,*PLAYER_CASE_background,*PLAYER_CASE_front;
Texture *PLAYER_name_background_shop;
Texture *PLAYER_SPELLS_background,*PLAYER_SPELLS_front,*PLAYER_SPELLS_no_mana,*PLAYER_SPELLS_not_ready;
bool PLAYER_IMAGES_LOADED;

///LEVEL
Texture *LEVEL_background_image,*LEVEL_loading_image,*LEVEL_meditation_image;
Texture *LEVEL_WINNER,*LEVEL_LOSER,*LEVEL_MONEY,*LEVEL_XP,*LEVEL_LINE,*LEVEL_WINS;
Texture *MAP_NAME_background,*SKEPTIC_VISION_image;
bool LEVEL_IMAGES_LOADED;

///SETTINGS
Texture *SETTINGS_option_background,*SETTINGS_option_background_selected,*SETTINGS_background,*SETTINGS_name;
bool SETTINGS_IMAGES_LOADED;

///SCRIPT
Texture *SCRIPT_default_background_image;
bool SCRIPT_IMAGES_LOADED;

///JOURNAL
Texture *JOURNAL_ENTRY_BACKGROUND,*JOURNAL_ENTRY_HOVER_BACKGROUND,*JOURNAL_ENTRY_CLICK_BACKGROUND;
bool JOURNAL_IMAGES_LOADED;

///INVENTORY
Texture *INVENTORY_LARROW,*INVENTORY_RARROW;
bool INVENTORY_IMAGES_LOADED;

///CONTROLS SCREEN
Texture *CONTROLS_keyboard_image,*CONTROLS_controller_image,*CONTROLS_background_image;
bool CONTROLS_SCREEN_IMAGES_LOADED;

void Load_shop_images()
{
 if(SHOP_IMAGES_LOADED)
    return;
 SHOP_IMAGES_LOADED=true;
 SHOP_title_clear=Load_Transparent_Texture("images/shop/title_clear.png");
 SHOP_title_background_line=Load_Transparent_Texture("images/shop/title_background_line.png");
 SHOP_title_background_click=Load_Transparent_Texture("images/shop/title_background_click.png");
 SHOP_title_background_click_right_frame=Load_Transparent_Texture("images/shop/title_background_click_right_frame.png");
 SHOP_title_background_right_frame=Load_Transparent_Texture("images/shop/title_background_right_frame.png");
 SHOP_title_background_selected=Load_Transparent_Texture("images/shop/title_background_selected.png");
 SHOP_title_background=Load_Transparent_Texture("images/shop/title_background.png");
 SHOP_shop_page_background=Load_Texture("images/shop/shop_page_background.png");
 SHOP_shop_big_background=Load_Texture("images/shop/shop_big_background.png");
 SHOP_shop_rope=Load_Transparent_Texture("images/shop/shop_rope.png");
 SHOP_shop_background=Load_Texture("images/shop/shop_background.png");
 SHOP_shop_background_selected=Load_Texture("images/shop/shop_background_selected.png");
 SHOP_description_background=Load_Transparent_Texture("images/shop/description_background.png");
 SHOP_inventory_background=Load_Transparent_Texture("images/shop/inventory_background.png");
 SHOP_item_background_selected=Load_Transparent_Texture("images/shop/item_background_selected.png");
 Set_Texture_Blend_Mode(SHOP_item_background_selected,SDL_BLENDMODE_BLEND);
 Set_Texture_Alpha(SHOP_item_background_selected,40);
 SHOP_item_background=Load_Texture("images/shop/item_background.png");
 SHOP_inventory_spell_background=Load_Texture("images/shop/spell_background.png");
 SHOP_inventory_spell_background_equipped=Load_Texture("images/shop/spell_background_equipped.png");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",15);
 SDL_Color color{65,105,225};
 INVENTORY_EQUIP=Create_TTF_Texture(font,"Equip",EQUIP_COLOR);
 Set_Texture_Blend_Mode(INVENTORY_EQUIP,SDL_BLENDMODE_BLEND);
 Set_Texture_Alpha(INVENTORY_EQUIP,200);
 INVENTORY_EQUIPPED=Create_TTF_Texture(font,"Equipped",EQUIPPED_COLOR);
 INVENTORY_SELL=Create_TTF_Texture(font,"Sell",BUY_COLOR);
 Set_Texture_Blend_Mode(INVENTORY_SELL,SDL_BLENDMODE_BLEND);
 Set_Texture_Alpha(INVENTORY_SELL,200);
 INVENTORY_spell_position[0]=Create_TTF_Texture(font,"1",color);
 INVENTORY_spell_position[1]=Create_TTF_Texture(font,"2",color);
 INVENTORY_spell_position[2]=Create_TTF_Texture(font,"3",color);
 INVENTORY_spell_position[3]=Create_TTF_Texture(font,"4",color);
 TTF_CloseFont(font);
}

void Clear_shop_images()
{
 if(!SHOP_IMAGES_LOADED)
    return;
 Destroy_Texture(SHOP_title_clear);
 Destroy_Texture(SHOP_title_background_line);
 Destroy_Texture(SHOP_title_background_click);
 Destroy_Texture(SHOP_title_background_click_right_frame);
 Destroy_Texture(SHOP_title_background_right_frame);
 Destroy_Texture(SHOP_title_background_selected);
 Destroy_Texture(SHOP_title_background);
 Destroy_Texture(SHOP_shop_page_background);
 Destroy_Texture(SHOP_shop_big_background);
 Destroy_Texture(SHOP_shop_rope);
 Destroy_Texture(SHOP_shop_background);
 Destroy_Texture(SHOP_shop_background_selected);
 Destroy_Texture(SHOP_description_background);
 Destroy_Texture(SHOP_inventory_background);
 Destroy_Texture(SHOP_item_background_selected);
 Destroy_Texture(SHOP_item_background);
 Destroy_Texture(SHOP_inventory_spell_background);
 Destroy_Texture(SHOP_inventory_spell_background_equipped);
 Destroy_Texture(INVENTORY_EQUIP);
 Destroy_Texture(INVENTORY_EQUIPPED);
 Destroy_Texture(INVENTORY_SELL);
 for(int i=0;i<4;i++)
     Destroy_Texture(INVENTORY_spell_position[i]);
 SHOP_IMAGES_LOADED=false;
}

void Load_player_images()
{
 if(PLAYER_IMAGES_LOADED)
    return;
 PLAYER_IMAGES_LOADED=true;
 PLAYER_name_background=Load_Transparent_Texture("images/player/name_background.png");
 PLAYER_name_background_shop=Load_Transparent_Texture("images/player/name_background_shop.png");
 PLAYER_name_background_right_layer=Load_Texture("images/player/name_background_right_layer.png");
 PLAYER_details_background=Load_Transparent_Texture("images/player/details_background.png");
 PLAYER_experience_background=Load_Texture("images/player/experience_background.png");
 PLAYER_money_background=Load_Texture("images/player/money_background.png");
 PLAYER_HP_background=Load_Transparent_Texture("images/player/hp_background.png");
 PLAYER_MANA_background=Load_Transparent_Texture("images/player/mana_background.png");
 PLAYER_CASE_background=Load_Transparent_Texture("images/player/case_background.png");
 PLAYER_CASE_front=Load_Transparent_Texture("images/player/case_front.png");
 PLAYER_SPELLS_background=Load_Transparent_Texture("images/player/spells_background.png");
 PLAYER_SPELLS_front=Load_Transparent_Texture("images/player/spells_front.png");
 PLAYER_SPELLS_no_mana=Load_Transparent_Texture("images/player/spells_no_mana.png");
 Set_Texture_Alpha(PLAYER_SPELLS_no_mana,200);
 PLAYER_SPELLS_not_ready=Load_Transparent_Texture("images/player/spells_not_ready.png");
 Set_Texture_Alpha(PLAYER_SPELLS_not_ready,200);
}

void Clear_player_images()
{
 if(!PLAYER_IMAGES_LOADED)
    return;
 Destroy_Texture(PLAYER_name_background);
 Destroy_Texture(PLAYER_name_background_shop);
 Destroy_Texture(PLAYER_name_background_right_layer);
 Destroy_Texture(PLAYER_details_background);
 Destroy_Texture(PLAYER_experience_background);
 Destroy_Texture(PLAYER_money_background);
 Destroy_Texture(PLAYER_HP_background);
 Destroy_Texture(PLAYER_MANA_background);
 Destroy_Texture(PLAYER_CASE_background);
 Destroy_Texture(PLAYER_CASE_front);
 Destroy_Texture(PLAYER_SPELLS_background);
 Destroy_Texture(PLAYER_SPELLS_front);
 Destroy_Texture(PLAYER_SPELLS_no_mana);
 Destroy_Texture(PLAYER_SPELLS_not_ready);
 PLAYER_IMAGES_LOADED=false;
}

void Load_level_images()
{
 if(LEVEL_IMAGES_LOADED)
    return;
 loading_image_mutex=SDL_CreateMutex();
 LEVEL_IMAGES_LOADED=true;
 LEVEL_background_image=Load_Texture("images/game/background.png");
 LEVEL_loading_image=Load_Transparent_Texture("images/game/loading.png");
 LEVEL_meditation_image=Load_Transparent_Texture("images/game/meditation.png");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",50),*font1=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Color winner_color={0,205,0},loser_color={207,0,0},xp_color={75,0,130},MONEY_COLOR={125,125,125},wins_color={241,188,48};
 LEVEL_WINNER=Create_TTF_Texture(font,"Winner",winner_color);
 LEVEL_LOSER=Create_TTF_Texture(font,"Loser",loser_color);
 LEVEL_WINS=Create_TTF_Texture(font1,"WINS:   ",wins_color);
 LEVEL_XP=Create_TTF_Texture(font1,"XP:   ",xp_color);
 LEVEL_MONEY=Create_TTF_Texture(font1,"MONEY:   ",MONEY_COLOR);
 LEVEL_LINE=Load_Transparent_Texture("images/game/line.png");
 MAP_NAME_background=Load_Transparent_Texture("images/game/map_name_background.png");
 SKEPTIC_VISION_image=Load_Transparent_Texture("images/game/skeptic_vision.png");
 Set_Texture_Alpha(SKEPTIC_VISION_image,0);
 TTF_CloseFont(font);
 TTF_CloseFont(font1);
}

void Clear_level_images()
{
 if(!LEVEL_IMAGES_LOADED)
    return;
 SDL_DestroyMutex(loading_image_mutex);
 Destroy_Texture(LEVEL_background_image);
 Destroy_Texture(LEVEL_loading_image);
 Destroy_Texture(LEVEL_WINNER);
 Destroy_Texture(LEVEL_LOSER);
 Destroy_Texture(LEVEL_XP);
 Destroy_Texture(LEVEL_MONEY);
 Destroy_Texture(LEVEL_LINE);
 Destroy_Texture(MAP_NAME_background);
 Destroy_Texture(LEVEL_WINS);
 Destroy_Texture(SKEPTIC_VISION_image);
 LEVEL_IMAGES_LOADED=false;
}

void Load_journal_images()
{
 if(JOURNAL_IMAGES_LOADED)
    return;
 LEVEL_IMAGES_LOADED=true;
 JOURNAL_ENTRY_BACKGROUND=Load_Transparent_Texture("images/journal/background.png");
 JOURNAL_ENTRY_CLICK_BACKGROUND=Load_Transparent_Texture("images/journal/click_background.png");
 JOURNAL_ENTRY_HOVER_BACKGROUND=Load_Transparent_Texture("images/journal/hover_background.png");
}

void Clear_journal_images()
{
 if(!JOURNAL_IMAGES_LOADED)
    return;
 JOURNAL_IMAGES_LOADED=false;
 Destroy_Texture(JOURNAL_ENTRY_BACKGROUND);
 Destroy_Texture(JOURNAL_ENTRY_CLICK_BACKGROUND);
 Destroy_Texture(JOURNAL_ENTRY_HOVER_BACKGROUND);
}

void Load_inventory_images()
{
 if(INVENTORY_IMAGES_LOADED)
    return;
 INVENTORY_IMAGES_LOADED=true;
 INVENTORY_LARROW=Load_Transparent_Texture("images/inventory/left_arrow.png");
 INVENTORY_RARROW=Load_Transparent_Texture("images/inventory/right_arrow.png");
}

void Clear_inventory_images()
{
 if(!INVENTORY_IMAGES_LOADED)
    return;
 INVENTORY_IMAGES_LOADED=false;
 Destroy_Texture(INVENTORY_LARROW);
 Destroy_Texture(INVENTORY_RARROW);
}

void Load_global_images()
{
 if(GLOBAL_IMAGES_LOADED)
    return;
 GLOBAL_IMAGES_LOADED=true;
 COIN=Load_Transparent_Texture("images/shop/coin.png");
}

void Clear_global_images()
{
 if(!GLOBAL_IMAGES_LOADED)
    return;
 Destroy_Texture(COIN);
 GLOBAL_IMAGES_LOADED=false;
}

void Load_settings_images()
{
 if(SETTINGS_IMAGES_LOADED)
    return;
 SETTINGS_IMAGES_LOADED=true;
 SETTINGS_option_background=Load_Texture("images/settings/option_background.png");
 SETTINGS_option_background_selected=Load_Texture("images/settings/option_background_selected.png");
 SETTINGS_background=Load_Texture("images/settings/background.png");
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",40);
 SETTINGS_name=Create_TTF_Texture(font,"Settings",SDL_Color{255,255,255});
 TTF_CloseFont(font);
}

void Clear_settings_images()
{
 if(!SETTINGS_IMAGES_LOADED)
    return;
 Destroy_Texture(SETTINGS_option_background);
 Destroy_Texture(SETTINGS_option_background_selected);
 Destroy_Texture(SETTINGS_background);
 Destroy_Texture(SETTINGS_name);
 SETTINGS_IMAGES_LOADED=false;
}

void Load_menu_images()
{
 if(MENU_IMAGES_LOADED)
    return;
 MENU_IMAGES_LOADED=true;
 MENU_background=Load_Texture("images/menu/menu_background.png");
 MENU_background_click=Load_Texture("images/menu/menu_background_click.png");
 MENU_background_selected=Load_Texture("images/menu/menu_background_selected.png");
}

void Clear_menu_images()
{
 if(!MENU_IMAGES_LOADED)
    return;
 Destroy_Texture(MENU_background);
 Destroy_Texture(MENU_background_click);
 Destroy_Texture(MENU_background_selected);
 MENU_IMAGES_LOADED=false;
}

void Load_script_images()
{
 if(SCRIPT_IMAGES_LOADED)
    return;
 SCRIPT_IMAGES_LOADED=true;
 SCRIPT_default_background_image=Load_Transparent_Texture("images/game/empty.png");
}

void Clear_script_images()
{
 if(!SCRIPT_IMAGES_LOADED)
    return;
 Destroy_Texture(SCRIPT_default_background_image);
 SCRIPT_IMAGES_LOADED=false;
}

void Load_controls_screen_images()
{
 if(CONTROLS_SCREEN_IMAGES_LOADED)
    return;
 CONTROLS_SCREEN_IMAGES_LOADED=true;
 CONTROLS_keyboard_image=Load_Transparent_Texture("images/menu/controls.png");
 CONTROLS_controller_image=Load_Transparent_Texture("images/menu/controller.png");
 CONTROLS_background_image=Load_Texture("images/launcher/launcher background.png");
}

void Clear_controls_screen_images()
{
 if(!CONTROLS_SCREEN_IMAGES_LOADED)
    return;
 CONTROLS_SCREEN_IMAGES_LOADED=false;
 Destroy_Texture(CONTROLS_keyboard_image);
 Destroy_Texture(CONTROLS_controller_image);
 Destroy_Texture(CONTROLS_background_image);
}

void Load_all_images()
{
 Load_global_images();
 Load_menu_images();
 Load_shop_images();
 Load_player_images();
 Load_level_images();
 Load_journal_images();
 Load_inventory_images();
 Load_settings_images();
 Load_script_images();
 Load_controls_screen_images();
}

void Clear_all_images()
{
 Clear_global_images();
 Clear_menu_images();
 Clear_shop_images();
 Clear_player_images();
 Clear_level_images();
 Clear_journal_images();
 Clear_inventory_images();
 Clear_settings_images();
 Clear_script_images();
 Clear_controls_screen_images();
}

Texture *static_screen;
bool Loading_image_quit=false;
SDL_mutex *loading_image_mutex;
int Loading_image(void *data)
{
 const int loading_image_frameW=160;
 int frame=0;
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=false;
 for(int i=0;i<25 && !Loading_image_quit;i++)
     {
      SDL_UnlockMutex(loading_image_mutex);
      if(i%4==0)
         {
          Apply_Texture(0,0,last_frame,static_screen);
          Flip_Buffers(static_screen);
         }
      SDL_Delay(10);
      SDL_LockMutex(loading_image_mutex);
     }
 while(!Loading_image_quit)
       {
        SDL_UnlockMutex(loading_image_mutex);
        Apply_Texture(0,0,LEVEL_background_image,static_screen);
        Apply_Texture(loading_image_frameW*frame,0,((static_screen->w)-loading_image_frameW)/2,((static_screen->h)-LEVEL_loading_image->h)/2,loading_image_frameW,LEVEL_loading_image->h,LEVEL_loading_image,static_screen);
        Flip_Buffers(static_screen);
        SDL_Delay(100);
        frame++;
        frame%=8;
        SDL_LockMutex(loading_image_mutex);
       }
 SDL_UnlockMutex(loading_image_mutex);
}

bool splash_screen_quit;
SDL_mutex *splash_screen_mutex;
const int FOG_UPDATE_INTERVAL=125,FOG_OPACITY_DECREASE_INTERVAL=200;
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
 fog[1].Set_frameH(1080);
 fog[0].Set_image_name("launcher_fog1");
 fog[0].Load_image();
 fog[0].Set_number_of_frames(8);
 fog[0].Set_aplha(SDL_ALPHA_OPAQUE/2);
 fog[0].Update_image();
 fog[0].Set_frameW(1920);
 fog[0].Set_frameH(1080);
 SDL_Rect area;
 area.x=area.y=0;
 area.w=RESOLUTION_W;
 area.h=RESOLUTION_H;
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
        Flip_Buffers(static_screen);
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
 SDL_UnlockMutex(splash_screen_mutex);
 fog[0].Clear();
 fog[1].Clear();
}

int Meditation_Screen(void *data)
{
 const int meditation_frameW=320;
 const int number_of_frames=8;
 int frame=0;
 while(frame!=number_of_frames)
       {
        Apply_Texture(0,0,LEVEL_background_image,static_screen);
        Apply_Texture(meditation_frameW*frame,0,((static_screen->w)-meditation_frameW)/2,((static_screen->h)-LEVEL_meditation_image->h)/2,meditation_frameW,LEVEL_meditation_image->h,LEVEL_meditation_image,static_screen);
        Flip_Buffers(static_screen);
        SDL_Delay(180);
        frame++;
       }
}
