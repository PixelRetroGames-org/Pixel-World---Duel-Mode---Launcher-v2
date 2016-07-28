#include "player.h"
//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
//CSTRING
#include<cstring>

const SDL_Color NAME_COLOR={27,100,250},EXPERIENCE_COLOR={235,20,20},MONEY_COLOR={125,125,125},MONEY_COLOR1={236,242,4},NUMBER_OF_ITEMS_COLOR={255,128,0};
const SDL_Color EQUIP_COLOR={15,30,90},BUY_COLOR={40,80,160},EQUIPPED_COLOR={255,128,0},SKIN_COLOR={193,20,20};

const int INVENTORY_MAX_NUMBER_OF_ITEMS=10;

Player::Player()
{
 skin_image_position.w=skin_image_position.h=40;
 money=experience=number_of_items=0;
 name[0]=NULL;
 memset(number_of_items_bought,0,sizeof number_of_items_bought);
}

void Player::Clear()
{
 memset(name,0,sizeof name);
 money=experience=number_of_items=inventory_number_of_items=0;
 memset(number_of_items_bought,0,sizeof number_of_items_bought);

 //Item items_bought[NUMBER_OF_ITEMS_IDS];
 memset(items_bought,0,sizeof items_bought);
 memset(equipped_items_ids,0,sizeof equipped_items_ids);
 //Item equipped_items[10];
 memset(equipped_items,0,sizeof equipped_items);
 inventory_item_selected=inventory_item_click=pos_last_y=0;
 basic_attack=5,basic_defense=0,basic_spell_damage=0,basic_spell_resistance=0,basic_movement_speed=10;
 skin_image_position.h=skin_image_position.w=skin_image_position.x=skin_image_position.y=0;
}

void Player::Set_PLAYER_INFO_POSX(int _x)
{
 PLAYER_INFO_POSX=_x;
}

void Player::Set_PLAYER_INFO_LAST_POSX(int _x)
{
 PLAYER_INFO_LAST_POSX=_x;
}

void Player::Set_SKIN_POSX(int _x)
{
 SKIN_POSX=_x;
}

void Player::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Player::Load()
{
 char path[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"saves/players/");
 strcat(path,name);
 strcat(path,".pwp");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    {
     fclose(where);
     /*for(int i=1;i<=6;i++)
         {
          equipped_items_ids[i-1]=i;
          number_of_items_bought[i]=1;
          items_bought[i].Set_id(i);
          items_bought[i].Load();
         }
     for(int i=0;i<6;i++)
         {
          equipped_items[i].Set_id(equipped_items_ids[i]);
          equipped_items[i].Load();
         }
     number_of_items=6;*/
     for(int i=0;i<6;i++)
         {
          equipped_items[i].Set_type(i);
          equipped_items[i].Load();
         }
     inventory_number_of_items=7;
     equipped_items[8].Set_type(8);
     equipped_items[8].Load();
     skin_image_position.w=40;
     skin_image_position.h=40;
     basic_attack=5,basic_defense=0,basic_spell_damage=0,basic_spell_resistance=0,basic_movement_speed=10;
     Update();
     return;
    }
 fscanf(where,"%d %d %d",&money,&experience,&number_of_items);
 inventory_number_of_items=number_of_items;
 int id,quantity;
 for(int i=1;i<=number_of_items;i++)
     {
      fscanf(where,"%d %d ",&id,&quantity);
      number_of_items_bought[id]=quantity;
      items_bought[id].Set_id(id);
      items_bought[id].Load();
     }
 for(int i=0;i<9;i++)
     fscanf(where,"%d ",&equipped_items_ids[i]);
 for(int i=0;i<9;i++)
     {
      equipped_items[i].Set_id(equipped_items_ids[i]);
      equipped_items[i].Set_type(i);
      equipped_items[i].Load();
     }
 fscanf(where,"%d %d %d %d %d",&basic_attack,&basic_defense,&basic_spell_damage,&basic_spell_resistance,&basic_movement_speed);
 int w,h;
 fscanf(where,"%d %d ",&w,&h);
 skin_image_position.w=w;
 skin_image_position.h=h;
 fclose(where);
}

void Player::Update()
{
 char path[TEXT_LENGHT_MAX]={NULL};
 strcpy(path,"saves/players/");
 strcat(path,name);
 strcat(path,".pwp");
 FILE *where=fopen(path,"w");
 fprintf(where,"%d\n%d\n%d\n",money,experience,number_of_items);
 for(int i=0;i<NUMBER_OF_ITEMS_IDS;i++)
     {
      if(number_of_items_bought[i]!=0)
         fprintf(where,"%d %d\n",i,number_of_items_bought[i]);
     }
 for(int i=0;i<9;i++)
     fprintf(where,"%d ",equipped_items_ids[i]);
 fprintf(where,"\n%d %d %d %d %d\n",basic_attack,basic_defense,basic_spell_damage,basic_spell_resistance,basic_movement_speed);
 fprintf(where,"%d %d ",(int)skin_image_position.w,(int)skin_image_position.h);
 fclose(where);
}

bool Player::Is_bought(int _item_id)
{
 return number_of_items_bought[_item_id]>0;
}

bool Player::Is_equipped(int _item_id)
{
 bool pp=false;
 for(int i=0;i<8;i++)
     if(_item_id==equipped_items_ids[i])
        pp=true;
 return pp;
}

void Player::Equip(int _item_id)
{
 Item _item;
 _item.Set_id(_item_id);
 _item.Load();
 equipped_items_ids[_item.Get_type()]=_item_id;
 equipped_items[_item.Get_type()]=_item;
}

void Player::Unequip(int _item_id)
{
 Item _item;
 _item.Set_id(_item_id);
 _item.Load();
 equipped_items_ids[_item.Get_type()]=_item.Get_type()<6?(_item.Get_type()+1):0;
 equipped_items[_item.Get_type()].Set_id(equipped_items_ids[_item.Get_type()]);
 equipped_items[_item.Get_type()].Load();
}

int Player::Buy(int _item_id)
{
 Item _item;
 _item.Set_id(_item_id);
 _item.Load();
 if(money<_item.Get_cost())
    return 1;
 if(inventory_number_of_items>=INVENTORY_MAX_NUMBER_OF_ITEMS && !Is_bought(_item_id))
    return 2;
 number_of_items_bought[_item_id]++,money-=_item.Get_cost();
 if(number_of_items_bought[_item_id]==1)
    {
     number_of_items++;
     inventory_number_of_items++;
     items_bought[_item_id]=_item;
    }
 return 0;
}

void Player::Sell(int _item_id)
{
 Item _item;
 _item.Set_id(_item_id);
 _item.Load();
 number_of_items_bought[_item_id]--;
 if(number_of_items_bought[_item_id]==0)
    {
     number_of_items--;
     inventory_number_of_items--;
     items_bought[_item_id].Set_id(0);
    }
 money+=_item.Get_cost();
 if(equipped_items_ids[_item.Get_type()]==_item_id)
    {
     equipped_items_ids[_item.Get_type()]=0;
     equipped_items[_item.Get_type()].Set_id(equipped_items_ids[_item.Get_type()]);
     equipped_items[_item.Get_type()].Load();
    }
}

int Player::Get_PLAYER_INFO_POSX()
{
 return PLAYER_INFO_POSX;
}

int Player::Get_PLAYER_INFO_LAST_POSX()
{
 return PLAYER_INFO_LAST_POSX;
}

int Player::Get_SKIN_POSX()
{
 return SKIN_POSX;
}

int Player::Get_pos_last_y()
{
 return pos_last_y;
}

void Player::Print_Character(int x,int y,SDL_Surface *_screen)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Surface *_image;
 int _x,_y;
 char message[TEXT_LENGHT_MAX]={NULL},aux[TEXT_LENGHT_MAX]={NULL};

 equipped_items[8].Print(SKIN_POSX,y,_screen,false);

 _image=PLAYER_name_background;
 apply_surface(x,y,_image,_screen);
 strcpy(message,name);
 _image=TTF_RenderText_Solid(font,message,NAME_COLOR);
 apply_surface(x+(SKIN_POSX-x+1-_image->w+10)/2,y,_image,_screen);
 y+=_image->h+20;

 _image=PLAYER_details_background;
 apply_surface(x,y-10,_image,_screen);

 _image=PLAYER_experience_background;
 apply_surface(x,y,_image,_screen);
 itoa(experience,aux);
 strcpy(message,"Experience: ");
 strcat(message,aux);
 _image=TTF_RenderText_Solid(font,message,EXPERIENCE_COLOR);
 apply_surface(x+10,y,_image,_screen);
 y+=_image->h;

 _image=PLAYER_money_background;
 apply_surface(x,y,_image,_screen);
 strcpy(message,"Money: ");
 _image=TTF_RenderText_Solid(font,message,MONEY_COLOR);
 apply_surface(x+10,y+10,_image,_screen);
 _x=x+_image->w+10,_y=y+10;
 itoa(money,aux);
 _image=TTF_RenderText_Solid(font,aux,MONEY_COLOR1);
 apply_surface(_x,_y,_image,_screen);
 _x+=_image->w,_y=y;
 _image=COIN;
 apply_surface(_x,_y+5,_image,_screen);
 y+=_image->h+20;

 equipped_items[0].Print(x,y,_screen,false);
 //item.Print_description(x+190,y,_screen,false);
 y+=120;


 equipped_items[1].Print(x,y,_screen,false);
 //item.Print_description(x+190,y,_screen,false);
 equipped_items[4].Print(x+180,y,_screen,false);
 //item.Print_description(x+190,y,_screen,false);
 y+=120;

 equipped_items[2].Print(x,y,_screen,false);
 //item.Print_description(x+190,y,_screen,false);
 equipped_items[5].Print(x+180,y,_screen,false);
 y+=120;

 equipped_items[3].Print(x,y,_screen,false);
 //item.Print_description(x+190,y,_screen,false);
 y+=120;
 pos_last_y=y;
}

void Player::Print_Inventory(int x,int y,SDL_Surface *_screen)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",15);
 char message[TEXT_LENGHT_MAX]={'x',NULL};
 SDL_Surface *_image=SHOP_inventory_background;
 SDL_Surface *sell_image=TTF_RenderText_Solid(font,"Sell",BUY_COLOR),*equip_image=TTF_RenderText_Solid(font,"Equip",EQUIP_COLOR);;
 apply_surface(x,y,_image,_screen);
 int _x=x,_y=y;
 for(int i=0;i<=NUMBER_OF_ITEMS_IDS;i++)
     {
      if(number_of_items_bought[i]!=0)
         {
          if(items_bought[i].Get_id()==0/* || items_bought[i].Get_type()<6*/)
             continue;
          if(inventory_item_selected==i)
             _image=SHOP_item_background_selected;
          else
             _image=SHOP_item_background;
          apply_surface(_x,_y,_image,_screen);
          items_bought[i].Print_inventory_image(_x,_y,_screen);
          itoa(number_of_items_bought[i],message+1);
          _image=TTF_RenderText_Solid(font,message,NUMBER_OF_ITEMS_COLOR);
          apply_surface(_x+42,_y+25,_image,_screen);
          if(!Is_potion(i))
             {
              if(equipped_items_ids[items_bought[i].Get_type()]!=i)
                 apply_surface(_x+40,_y+2,equip_image,_screen);
              else
                 {
                  _image=TTF_RenderText_Solid(font,"Equipped",EQUIPPED_COLOR);
                  apply_surface(_x+40,_y+2,_image,_screen);
                 }
             }
          apply_surface(_x+42,_y+15,sell_image,_screen);
          _x+=110;
          if(_x+110>PLAYER_INFO_LAST_POSX)
             _x=x,_y+=60;
         }
     }
}

int Player::Start_inventory(int x,int y,SDL_Surface *_screen,SDL_Event *event)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",15);
 SDL_Surface *sell_image=TTF_RenderText_Solid(font,"Sell",BUY_COLOR),*equip_image=TTF_RenderText_Solid(font,"Equip",EQUIP_COLOR);
 inventory_item_selected=inventory_item_click=-1;
 int _x=x,_y=y;
 int mouse_x=event->button.x,mouse_y=event->button.y;
 bool _sell=false,_equip=false;
 for(int i=0;i<=NUMBER_OF_ITEMS_IDS;i++)
     {
      if(number_of_items_bought[i]!=0)
         {
          if(items_bought[i].Load() /*|| items_bought[i].Get_type()<6*/)
             continue;
          if(mouse_x>=_x && mouse_x<_x+80 && mouse_y>=_y && mouse_y<_y+60)
             inventory_item_selected=i;
          if(mouse_x>=_x+40 && mouse_x<=_x+40+equip_image->w && mouse_y>=_y+2 && mouse_y<=_y+2+equip_image->h)
             _equip=true;
          if(mouse_x>=_x+42 && mouse_x<=_x+42+equip_image->w && mouse_y>=_y+15 && mouse_y<=_y+15+equip_image->h)
             _sell=true;
          _x+=110;
          if(_x+110>PLAYER_INFO_LAST_POSX)
             _x=x,_y+=60;
         }
     }
 if(event->type==SDL_MOUSEBUTTONDOWN)
    inventory_item_click=inventory_item_selected;
 if(inventory_item_click!=-1)
    {
     if(_sell)
        Sell(inventory_item_click);
     if(_equip)
        Equip(inventory_item_click);
    }
 Print_Character(PLAYER_INFO_POSX,0,_screen);
 Print_Inventory(x,y,_screen);
 if(_sell)
    return inventory_item_click;
 if(_equip)
    return -inventory_item_click;
 return 0;
}
