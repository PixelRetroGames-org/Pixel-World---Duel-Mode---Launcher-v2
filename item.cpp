#include "item.h"
///CSTRING
#include<cstring>

SDL_Color NAME_COLOR={255,255,255};
SDL_Color MONEY_COLOR={236,242,4};
SDL_Color DESCRIPION_COLOR={50,70,90};

const char *type_name[10]={"Helmet","Chestplate","Trousers","Boots","Weapon","Shield","Amulet","Ring","Timy Skin","Potions"};

Item::Item()
{
 inventory_image=image=name_image=NULL;
 id=cost=0;
 image=NULL;
 name[0]=description[0]=NULL;
 attack=defense=extra_money=fire_damage=fire_resistance=mana=hp=0;
}

int Item::Get_id()
{
 return id;
}

int Item::Get_type()
{
 return type;
}

int Item::Get_cost()
{
 return cost;
}

void Item::Set_type(int _type)
{
 type=_type;
}

void Item::Set_id(int _id)
{
 id=_id;
}

int Item::Load()
{
 char path[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};
 if(id==0)
    {
     strcpy(path,"shop/items/images/");
     strcat(path,type_name[type]);
     strcat(path,".bmp");
     image=make_it_transparent(path);
     return false;
    }
 itoa(id,aux);
 strcpy(path,"shop/items/");
 strcat(path,aux);
 strcat(path,".pwi");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    return true;
 fscanf(where,"%d ",&cost);
 fgets(name,TEXT_LENGHT_MAX,where);
 if(name[strlen(name)-1]=='\n')
    name[strlen(name)-1]=NULL;
 fgets(description,TEXT_LENGHT_MAX,where);
 description[strlen(description)-1]=NULL;
 fscanf(where,"%d %d %d %d %d %d %d %d",&attack,&defense,&extra_money,&fire_damage,&fire_resistance,&mana,&hp,&type);
 strcpy(path,"shop/items/images/");
 strcat(path,name);
 strcat(path,".bmp");
 image=make_it_transparent(path);
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",20);
 name_image=TTF_RenderText_Solid(font,name,NAME_COLOR);
 TTF_CloseFont(font);
 strcpy(path,"shop/items/inventory/images/");
 strcat(path,name);
 strcat(path,".bmp");
 inventory_image=make_it_transparent(path);
 fclose(where);
 return false;
}

void Item::Print(int x,int y,SDL_Surface *_screen,bool selected=false)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",15);
 SDL_Surface *message=NULL;
 if(!selected)
    message=SHOP_shop_background;
 else
    message=SHOP_shop_background_selected;
 apply_surface(x,y,message,_screen);
 if(id==0)
    {
     strcpy(name,type_name[type]);
    }
 message=TTF_RenderText_Solid(font,name,NAME_COLOR);
 apply_surface(x+((180-message->w)/2),y+3,message,_screen);
 apply_surface(x+10,y+30,image,_screen);
 char aux[TEXT_LENGHT_MAX]={NULL};
 itoa(cost,aux);
 TTF_CloseFont(font);
 font=TTF_OpenFont("fonts/pixel.ttf",15);
 if(id!=0)
    {
     message=TTF_RenderText_Solid(font,aux,MONEY_COLOR);
     apply_surface(x+100,y+30,message,_screen);
     apply_surface(x+100,y+60,COIN,_screen);
    }
 if(selected && id!=0)
    Print_description(x,y,_screen,true);
 TTF_CloseFont(font);
 SDL_FreeSurface(message);
}

void Item::Print_description(int x,int y,SDL_Surface *_screen,bool selected=false)
{
 SDL_Surface *_image=NULL;
 _image=SHOP_shop_rope;
 SDL_Surface *message=NULL;
 TTF_Font *font=TTF_OpenFont("fonts/pixel3.ttf",25);
 if(font==NULL)
    exit(100);
 int _x=x,_y=y;
 _y=y+120;
 if(_y>768-160)
    _y=y-160;
 int N=strlen(description);
 char aux[TEXT_LENGHT_MAX]={NULL};
 apply_surface(_x,_y,SHOP_description_background,_screen);
 _x+=10,_y+=0;
 int j=0;
 for(int i=0;i<N;i++)
     {
      if(description[i]=='~')
         {
          aux[j+1]=NULL;
          message=TTF_RenderText_Solid(font,aux,DESCRIPION_COLOR);
          apply_surface(_x,_y,message,_screen);
          _y+=20;
          j=0;
          memset(aux,0,sizeof aux);
         }
      else
         aux[j++]=description[i];
     }
 aux[j]=NULL;
 message=TTF_RenderText_Solid(font,aux,DESCRIPION_COLOR);
 apply_surface(_x,_y,message,_screen);
 SDL_FreeSurface(message);
 TTF_CloseFont(font);
}

void Item::Print_image(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y,image,_screen);
}

void Item::Print_inventory_image(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y,inventory_image,_screen);
}

const int NUMBER_OF_POTIONS=2,POTIONS[]={15,16};

bool Is_potion(Item _item)
{
 return (_item.Get_type()==9);
}

bool Is_potion(int _item_id)
{
 bool ret=false;
 for(int i=0;i<NUMBER_OF_POTIONS;i++)
     if(_item_id==POTIONS[i])
        return true;
 return false;
}
