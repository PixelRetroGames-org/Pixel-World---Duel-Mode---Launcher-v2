#include "shop_page.h"
///CSTRING
#include<cstring>

const SDL_Color ITEM_NAME_COLOR={27,100,250};

Shop_Page::Shop_Page()
{
 name[0]=NULL;
 image=NULL;
 item_selected=item_click=-1;
 number_of_items=number_of_lines=number_of_columns=title_size=0;
}

void Shop_Page::Set_POSX(int _x)
{
 POSX=_x;
}

void Shop_Page::Set_LAST_POSX(int _x)
{
 LAST_POSX=_x;
}

void Shop_Page::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Shop_Page::Load()
{
 char path[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"shop/shop_page/");
 strcat(path,name);
 strcat(path,".pwsp");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d %d %d ",&number_of_items,&number_of_lines,&number_of_columns);
 int _id;
 Item aux;
 for(int i=0;i<number_of_items;i++)
     {
      fscanf(where,"%d ",&_id);
      item_ids[number_of_items]=_id;
      aux.Clear();
      aux.Set_id(_id);
      aux.Load();
      items[i]=aux;
     }
 fclose(where);
 strcpy(path,"shop/shop_page/images/");
 strcat(path,name);
 strcat(path,".bmp");
 image=make_it_transparent(path);
}

int Shop_Page::Get_title_size()
{
 return title_size;
}

void Shop_Page::Print_Title(int x,int y,SDL_Surface *_screen,bool selected=false,bool click=false)
{
 SDL_Surface *_image=NULL,*right_frame=NULL;
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",36);
 if(click)
    {
     _image=SHOP_title_background_click;
     right_frame=SHOP_title_background_click_right_frame;
    }
 else
    {
     right_frame=SHOP_title_background_right_frame;
     if(selected)
        _image=SHOP_title_background_selected;
     else
        _image=SHOP_title_background;
    }
 title_size=image->w;
 if(click)
    {
     SDL_Surface *__image=TTF_RenderText_Solid(font,name,ITEM_NAME_COLOR);
     title_size+=__image->w+10;
     SDL_FreeSurface(__image);
    }
 else
    title_size+=-5;
 apply_surface(x,y,title_size+20,50,_image,_screen);
 //apply_surface(x,y,_image,_screen);
 if(click)
    {
     _image=TTF_RenderText_Solid(font,name,ITEM_NAME_COLOR);
     title_size=_image->w;
     apply_surface(x+10,y+8,_image,_screen);
     apply_surface(x+title_size+20,y+5,image,_screen);
     title_size+=20+image->w;
     Print(POSX,y+60,_screen);
    }
 else
    {
     apply_surface(x+10,y+5,image,_screen);
     title_size=image->w+5;
    }
 title_size+=20;
 apply_surface(x+title_size-10,y,right_frame,_screen);
 TTF_CloseFont(font);
}

void Shop_Page::Print(int x,int y,SDL_Surface *_screen)
{
 SDL_Surface *_image=NULL;
 _image=SHOP_shop_page_background;
 apply_surface(x,y-10,LAST_POSX-x,900,_image,_screen);
 _image=SHOP_shop_rope;
 int _x=x,_y=y;
 for(int i=0;i<number_of_items;i++)
     {
      if((i)%number_of_columns==0 && i!=0)
         _y+=120+_image->h,_x=x;
      apply_surface(_x,_y,_image,_screen);
      items[i].Print(_x,_y+_image->h,_screen,i==item_selected);
      _x+=180;
     }
 _x=x,_y=y;
 for(int i=0;i<number_of_items;i++)
     {
      if((i)%number_of_columns==0 && i!=0)
         _y+=120+_image->h,_x=x;
      if(i==item_selected)
         items[i].Print(_x,_y+_image->h,_screen,i==item_selected);
      _x+=180;
     }
}

int Shop_Page::Start(SDL_Surface *_screen,SDL_Event *event)
{
 SDL_Surface *_image=NULL;
 _image=SHOP_shop_rope;
 int _x=POSX,_y=60,x,y;
 if(event->type==SDL_MOUSEMOTION)
    {
     x=event->button.x;
     y=event->button.y;
     item_selected=item_click=-1;
     for(int i=0;i<number_of_items;i++)
         {
          if((i)%number_of_columns==0 && i!=0)
             _y+=120+_image->h,_x=20;
          if(x>=_x && x<=_x+180 && y>=_y+_image->h && y<=_y+120+_image->h)
             item_selected=i;
          _x+=180;
         }
    }
 if(event->type==SDL_MOUSEBUTTONDOWN)
    {
     item_click=item_selected;
    }
 Print(POSX,60,_screen);
 if(item_click==-1)
    return -1;
 return items[item_click].Get_id();
}
