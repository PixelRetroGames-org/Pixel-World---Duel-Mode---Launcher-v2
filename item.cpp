#include "item.h"
///CSTRING
#include<cstring>

SDL_Color NAME_COLOR={255,255,255};
SDL_Color MONEY_COLOR={236,242,4};
SDL_Color DESCRIPTION_COLOR={50,70,90};

const char *type_name[10]={"Helmet","Chestplate","Trousers","Boots","Weapon","Shield","Amulet","Ring","Timy Skin","Potions"};

Item::Item()
{
 inventory_image=image=name_image=price_image=NULL;
 for(int i=0;i<DESCRIPTION_LINES_MAX;i++)
     description_image[i]=NULL;
 description_lines=0;
 id=cost=0;
 image=NULL;
 name[0]=description[0]=NULL;
 attack=defense=extra_money=fire_damage=fire_resistance=mana=hp=movement_speed=0;
}

void Item::Clear()
{
 inventory_image=image=name_image=NULL;
 for(int i=0;i<DESCRIPTION_LINES_MAX;i++)
     if(description_image[i]!=NULL)
        description_image[i]=NULL;
 description_lines=0;
 id=cost=0;
 image=NULL;
 name[0]=description[0]=NULL;
 attack=defense=extra_money=fire_damage=fire_resistance=mana=hp=movement_speed=0;
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
 TTF_Font *font=NULL;
 if(id==0)
    {
     strcpy(path,"shop/items/images/");
     strcat(path,type_name[type]);
     strcat(path,".bmp");
     image=make_it_transparent(path);
     font=TTF_OpenFont("fonts/pixel.ttf",15);
     strcpy(name,type_name[type]);
     name_image=TTF_RenderText_Solid(font,name,NAME_COLOR);
     TTF_CloseFont(font);
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
 fgets(name,sizeof name,where);
 if(name[strlen(name)-1]=='\n')
    name[strlen(name)-1]=NULL;
 fgets(description,sizeof description,where);
 description[strlen(description)-1]=NULL;
 int buff_id;
 fscanf(where,"%d %d %d %d %d %d %d %d %d %d",&attack,&defense,&extra_money,&fire_damage,&fire_resistance,&mana,&hp,&movement_speed,&type,&buff_id);
 buff.Set_id(buff_id);
 buff.Load();
 strcpy(path,"shop/items/images/");
 strcat(path,name);
 strcat(path,".bmp");
 image=make_it_transparent(path);
 font=TTF_OpenFont("fonts/pixel.ttf",15);
 name_image=TTF_RenderText_Solid(font,name,NAME_COLOR);
 TTF_CloseFont(font);
 itoa(cost,aux);
 font=TTF_OpenFont("fonts/pixel.ttf",15);
 price_image=TTF_RenderText_Solid(font,aux,MONEY_COLOR);
 TTF_CloseFont(font);
 strcpy(path,"shop/items/inventory/images/");
 strcat(path,name);
 strcat(path,".bmp");
 inventory_image=make_it_transparent(path);
 fclose(where);
 //Create description image
 SDL_Surface *message=NULL;
 font=TTF_OpenFont("fonts/pixel3.ttf",25);
 if(font==NULL)
    exit(100);
 int N=strlen(description);
 memset(aux,0,sizeof aux);
 int j=0;
 for(int i=0;i<N;i++)
     {
      if(description[i]=='~')
         {
          aux[j+1]=NULL;
          message=TTF_RenderText_Solid(font,aux,DESCRIPTION_COLOR);
          description_image[description_lines++]=message;
          j=0;
          memset(aux,0,sizeof aux);
         }
      else
         aux[j++]=description[i];
     }
 aux[j]=NULL;
 message=TTF_RenderText_Solid(font,aux,DESCRIPTION_COLOR);
 description_image[description_lines++]=message;
 TTF_CloseFont(font);
 return false;
}

void Item::Print(int x,int y,SDL_Surface *_screen,bool selected=false)
{
 SDL_Surface *message=NULL;
 if(!selected)
    message=SHOP_shop_background;
 else
    message=SHOP_shop_background_selected;
 apply_surface(x,y,message,_screen);
 apply_surface(x+((180-name_image->w)/2),y+3,name_image,_screen);
 Print_image(x+10,y+30,_screen);
 if(id!=0)
    {
     apply_surface(x+100,y+30,price_image,_screen);
     apply_surface(x+100,y+60,COIN,_screen);
    }
 if(selected && id!=0)
    Print_description(x,y,_screen,true);
}

void Item::Print_description(int x,int y,SDL_Surface *_screen,bool selected=false)
{
 SDL_Surface *_image=NULL;
 _image=SHOP_shop_rope;
 int _x=x,_y=y;
 _y=y+120;
 if(_y>768-160)
    _y=y-160;
 apply_surface(_x,_y,SHOP_description_background,_screen);
 _x+=10,_y+=0;
 for(int i=0;i<description_lines;i++)
     {
      if(description_image[i]!=NULL)
         apply_surface(_x,_y,description_image[i],_screen);
      _y+=20;
     }
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
