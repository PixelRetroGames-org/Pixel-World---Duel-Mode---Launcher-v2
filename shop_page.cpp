#include "shop_page.h"
///CSTRING
#include<cstring>

const SDL_Color ITEM_NAME_COLOR={27,100,250};

Shop_Page::Shop_Page()
{
 name[0]=NULL;
 image=name_image=NULL;
 item_selected=item_click=-1;
 number_of_items=number_of_lines=number_of_columns=title_size=0;
}

void Shop_Page::Clear(bool _delete)
{
 for(int i=0;i<number_of_items;i++)
     items[i].Clear(_delete);
 name[0]=NULL;
 item_selected=item_click=-1;
 number_of_items=number_of_lines=number_of_columns=title_size=0;
 POSX=0,LAST_POSX=0;
 type=0;
 if(_delete)
    {
     SDL_FreeSurface(image);
     SDL_FreeSurface(name_image);
     image=NULL,name_image=NULL;
    }
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
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"shop/shop_page/");
 strcat(path,name);
 strcat(path,".pwsp");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d ",&type);
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
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",36);
 name_image=TTF_RenderText_Solid(font,name,ITEM_NAME_COLOR);
 TTF_CloseFont(font);
}

int Shop_Page::Get_title_size()
{
 return title_size;
}

int Shop_Page::Get_type()
{
 return type;
}

void Shop_Page::Print_Title(int x,int y,SDL_Surface *_screen,bool selected=false,bool click=false)
{
 title_size=image->w;
 if(click)
    {
     title_size+=name_image->w+10;
    }
 else
    title_size+=-5;
 if(click)
    {
     apply_surface(x,y,title_size+20,50,SHOP_title_background_click,_screen);
    }
 else
    {
     if(selected)
        apply_surface(x,y,title_size+20,50,SHOP_title_background_selected,_screen);
     else
        apply_surface(x,y,title_size+20,50,SHOP_title_background,_screen);
    }
 //apply_surface(x,y,_image,_screen);
 if(click)
    {
     title_size=name_image->w;
     apply_surface(x+10,y+8,name_image,_screen);
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
 if(click)
    apply_surface(x+title_size-10,y,SHOP_title_background_click_right_frame,_screen);
 else
    apply_surface(x+title_size-10,y,SHOP_title_background_right_frame,_screen);
}

void Shop_Page::Print(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y-10,LAST_POSX-x,900,SHOP_shop_page_background,_screen);
 int _x=x,_y=y;
 for(int i=0;i<number_of_items;i++)
     {
      if((i)%number_of_columns==0 && i!=0)
         _y+=120+SHOP_shop_rope->h,_x=x;
      apply_surface(_x,_y,SHOP_shop_rope,_screen);
      items[i].Print(_x,_y+SHOP_shop_rope->h,_screen,i==item_selected);
      _x+=180;
     }
 _x=x,_y=y;
 for(int i=0;i<number_of_items;i++)
     {
      if((i)%number_of_columns==0 && i!=0)
         _y+=120+SHOP_shop_rope->h,_x=x;
      if(i==item_selected)
         items[i].Print(_x,_y+SHOP_shop_rope->h,_screen,i==item_selected);
      _x+=180;
     }
}

void Shop_Page::Reset()
{
 item_click=item_selected=-1;
}

int Shop_Page::Start(SDL_Surface *_screen,SDL_Event *event)
{
 int _x=POSX,_y=60,x,y;
 item_click=-1;
 if(event->type==SDL_MOUSEMOTION || event->type==SDL_MOUSEBUTTONDOWN)
    {
     x=event->button.x;
     y=event->button.y;
     item_selected=item_click=-1;
     for(int i=0;i<number_of_items;i++)
         {
          if((i)%number_of_columns==0 && i!=0)
             _y+=120+SHOP_shop_rope->h,_x=POSX;
          if(x>=_x && x<=_x+180 && y>=_y+SHOP_shop_rope->h && y<=_y+120+SHOP_shop_rope->h)
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
