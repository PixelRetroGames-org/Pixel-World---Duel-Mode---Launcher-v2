#include "item.h"
///CSTRING
#include <cstring>

SDL_Color NAME_COLOR={255,255,255};
SDL_Color MONEY_COLOR={236,242,4};
SDL_Color DESCRIPTION_COLOR={50,70,90};

const char *type_name[11]={"Helmet","Chestplate","Trousers","Boots","Sword","Shield","Amulet","Ring","Timy Skin","Potion","Spell"};

Item::Item()
{
 inventory_image=image=name_image=price_image=NULL;
 for(int i=0;i<DESCRIPTION_LINES_MAX;i++)
     description_image[i]=NULL;
 description_lines=0;
 id=cost=0;
 image=NULL;
 name[0]=description[0]=NULL;
 attack=defense=extra_money=spell_damage=spell_resistance=mana=hp=movement_speed=0;
}

void Item::Clear(bool _delete)
{
 if(_delete)
    {
     if(inventory_image!=NULL)
        Destroy_Texture(inventory_image);
     if(image!=NULL)
        Destroy_Texture(image);
     if(name_image!=NULL)
        Destroy_Texture(name_image);
     if(price_image!=NULL)
        Destroy_Texture(price_image);
     inventory_image=image=name_image=price_image=NULL;
    }
 for(int i=0;i<DESCRIPTION_LINES_MAX && _delete;i++)
     if(description_image[i]!=NULL)
        {
         Destroy_Texture(description_image[i]);
         description_image[i]=NULL;
        }
 description_lines=0;
 id=cost=0;
 image=NULL;
 name[0]=description[0]=NULL;
 attack=defense=extra_money=spell_damage=spell_resistance=mana=hp=movement_speed=0;
 if(buff.Get_id()!=0)
    buff.Clear();
}

int Item::Get_spell_id()
{
 return spell_id;
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

int Item::Get_attack()
{
 return attack;
}

int Item::Get_defense()
{
 return defense;
}

int Item::Get_extra_money()
{
 return extra_money;
}

int Item::Get_spell_damage()
{
 return spell_damage;
}

int Item::Get_spell_resistance()
{
 return spell_resistance;
}

int Item::Get_movement_speed()
{
 return movement_speed;
}

int Item::Get_mana()
{
 return mana;
}

int Item::Get_hp()
{
 return hp;
}

Texture *Item::Get_image()
{
 return image;
}

Texture *Item::Get_inventory_image()
{
 return inventory_image;
}

Texture *Item::Get_skin()
{
 if(type!=8)
    return NULL;
 Texture *_skin;
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"shop/skins/");
 strcat(path,name);
 strcat(path,".png");
 _skin=Load_Transparent_Texture(path);
 return _skin;
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
 char path[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 TTF_Font *font=NULL;
 if(id==0)
    {
     strcpy(path,"shop/items/images/");
     strcat(path,type_name[type]);
     strcat(path,".png");
     image=Load_Transparent_Texture(path);
     font=TTF_OpenFont("fonts/pixel.ttf",15);
     strcpy(name,type_name[type]);
     name_image=Create_TTF_Texture(font,name,NAME_COLOR);
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
 fscanf(where,"%d %d ",&cost,&spell_id);
 fgets(name,sizeof name,where);
 if(name[strlen(name)-1]=='\n')
    name[strlen(name)-1]=NULL;
 fgets(description,sizeof description,where);
 description[strlen(description)-1]=NULL;
 int buff_id;
 fscanf(where,"%d %d %d %d %d %d %d %d %d %d",&attack,&defense,&extra_money,&spell_damage,&spell_resistance,&mana,&hp,&movement_speed,&type,&buff_id);
 buff.Set_id(buff_id);
 buff.Load();
 strcpy(path,"shop/items/images/");
 strcat(path,name);
 strcat(path,".png");
 image=Load_Transparent_Texture(path);
 font=TTF_OpenFont("fonts/pixel.ttf",15);
 name_image=Create_TTF_Texture(font,name,NAME_COLOR);
 itoa(cost,aux);
 price_image=Create_TTF_Texture(font,aux,MONEY_COLOR);
 TTF_CloseFont(font);
 strcpy(path,"shop/items/inventory/images/");
 strcat(path,name);
 strcat(path,".png");
 inventory_image=Load_Transparent_Texture(path);
 fclose(where);
 //Create description image
 Texture *message=NULL;
 font=TTF_OpenFont("fonts/pixel3.ttf",25);
 int N=strlen(description);
 memset(aux,0,sizeof aux);
 int j=0;
 for(int i=0;i<N;i++)
     {
      if(description[i]=='~')
         {
          aux[j+1]=NULL;
          message=Create_TTF_Texture(font,aux,DESCRIPTION_COLOR);
          description_image[description_lines++]=message;
          j=0;
          memset(aux,0,sizeof aux);
         }
      else
         aux[j++]=description[i];
     }
 aux[j]=NULL;
 message=Create_TTF_Texture(font,aux,DESCRIPTION_COLOR);
 description_image[description_lines++]=message;
 TTF_CloseFont(font);
 return false;
}

void Item::Print(int x,int y,Texture *_screen,bool selected=false)
{
 Texture *message=NULL;
 if(!selected)
    message=SHOP_shop_background;
 else
    message=SHOP_shop_background_selected;
 Apply_Texture(x,y,message,_screen);
 Apply_Texture(x+((180-name_image->w)/2),y+3,name_image,_screen);
 Print_image(x+10,y+30,_screen);
 if(id!=0)
    {
     Apply_Texture(x+100,y+30,price_image,_screen);
     Apply_Texture(x+100,y+60,COIN,_screen);
    }
 if(selected && id!=0)
    Print_description(x,y,_screen,true);
}

void Item::Print_description(int x,int y,Texture *_screen,bool selected=false)
{
 Texture *_image=NULL;
 _image=SHOP_shop_rope;
 int _x=x,_y=y;
 _y=y+120;
 if(_y>768-160)
    _y=y-160;
 Apply_Texture(_x,_y,SHOP_description_background,_screen);
 _x+=10,_y+=0;
 for(int i=0;i<description_lines;i++)
     {
      if(description_image[i]!=NULL)
         Apply_Texture(_x,_y,description_image[i],_screen);
      _y+=20;
     }
}

void Item::Print_image(int x,int y,Texture *_screen)
{
 Apply_Texture(x,y,image,_screen);
}

void Item::Print_inventory_image(int x,int y,Texture *_screen)
{
 Apply_Texture(x,y,inventory_image,_screen);
}

//BUFFS
int Item::Get_buff_id()
{
 return buff.Get_id();
}

Buff Item::Get_Buff()
{
 return buff;
}

void Item::Set_Buff(Buff _buff)
{
 buff=_buff;
}

void Item::Buff_decrease_remaining_duration()
{
 buff.Decrease_remaining_duration();
}

void Item::Buff_Reset()
{
 if(Buff_Is_done())
    buff.Reset();
}

bool Item::Buff_Is_done()
{
 return buff.Is_done();
}

bool Item::Buff_Is_printable()
{
 return buff.Is_printable();
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
