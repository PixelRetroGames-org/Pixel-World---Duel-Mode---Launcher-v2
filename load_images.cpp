#include "load_images.h"

Big_Background LAUNCHER_BBACKGROUND;
SDL_Surface *MENU_big_background=NULL;
SDL_Surface *MENU_background=NULL,*MENU_background_click=NULL,*MENU_background_selected=NULL;

void Load_Menu_images()
{
 MENU_big_background=SDL_LoadBMP("images/menu/menu_big_background.bmp");
 MENU_background=SDL_LoadBMP("images/menu/menu_background.bmp");
 MENU_background_click=SDL_LoadBMP("images/menu/menu_background_click.bmp");
 MENU_background_selected=SDL_LoadBMP("images/menu/menu_background_selected.bmp");
}
