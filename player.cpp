#include "player.h"
//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
//CSTRING
#include <cstring>

const SDL_Color NAME_COLOR={255,255,255},EXPERIENCE_COLOR={235,20,20},MONEY_COLOR={125,125,125},MONEY_COLOR1={236,242,4},NUMBER_OF_ITEMS_COLOR={255,128,0};
const SDL_Color EQUIP_COLOR={15,30,90},BUY_COLOR={40,80,160},EQUIPPED_COLOR={255,128,0},SKIN_COLOR={193,20,20};
const SDL_Color HP_COLOR={255,255,255},MANA_COLOR={255,255,255};

const int INVENTORY_MAX_NUMBER_OF_ITEMS=10,INVENTORY_MAX_NUMBER_OF_POTIONS=5;

Player::Player()
{
 skin_image_position.x=skin_image_position.y=0;
 skin_image_position.h=skin_image_position.w=PIXELS_PER_INGAME_UNIT;
 money=experience=number_of_items=0;
 name[0]=NULL;
 memset(number_of_items_bought,0,sizeof number_of_items_bought);
 keys.reset();
 progress.reset();
}

void Player::Clear(bool _delete)
{
 if(_delete)
    {
     SDL_FreeSurface(skin);
     SDL_FreeSurface(name_image);
     SDL_FreeSurface(hp_image);
     SDL_FreeSurface(mana_image);
     skin=name_image=hp_image=mana_image=NULL;
    }

 memset(name,0,sizeof name);
 money=experience=number_of_items=inventory_number_of_items=inventory_number_of_spells=0;
 memset(number_of_items_bought,0,sizeof number_of_items_bought);

 memset(items_bought,0,sizeof items_bought);
 memset(equipped_items_ids,0,sizeof equipped_items_ids);
 memset(equipped_items,0,sizeof equipped_items);
 inventory_item_selected=inventory_item_click=pos_last_y=0;
 basic_hp=100,basic_mana=100,basic_mental_health=100;
 basic_attack=5,basic_defense=0,basic_spell_damage=10,basic_spell_resistance=0,basic_movement_speed=2,basic_life_steal=0;
 attack=defense=spell_damage=spell_resistance=movement_speed=life_steal=0;
 skin_image_position.h=skin_image_position.w=PIXELS_PER_INGAME_UNIT;
 skin_image_position.x=skin_image_position.y=0;
 for(int i=0;!active_buffs.empty() && i<active_buffs.size();i++)
     active_buffs[i].Clear(false);
 //active_buffs.clear();
 std::vector<Buff>().swap(active_buffs);
 for(int i=0;i<10;i++)
     equipped_items[i].Clear(_delete);
 for(int i=0;i<NUMBER_OF_ITEMS_IDS;i++)
     items_bought[i].Clear(_delete);
 //printable_item_buffs_id.clear();
 std::vector<int>().swap(printable_item_buffs_id);
 for(int i=0;i<4;i++)
     spells[i].Clear(_delete);
 map_positionX=map_positionY=-10;
 keys.reset();
 progress.reset();
 counter=-1;
}

void Player::Set_name(char *_name)
{
 strcpy(name,_name);
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

void Player::Set_money(int _money)
{
 money=_money;
}

void Player::Set_experience(int _experience)
{
 experience=_experience;
}

void Player::Load()
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 name_image=TTF_RenderText_Solid(font,name,NAME_COLOR);
 TTF_CloseFont(font);
 char path[TEXT_LENGTH_MAX]={NULL};
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
     inventory_number_of_items=0;
     equipped_items[8].Set_type(8);
     equipped_items[8].Load();
     number_of_spells=4;
     for(int spell_pos=0;spell_pos<number_of_spells;spell_pos++)
         spells[spell_pos].Set_id(0);
     Load_skin();
     Set_hp(basic_hp);
     Set_mana(basic_mana);
     Set_mental_health(basic_mental_health);
     original_skin_image_position=skin_image_position;
     attack=basic_attack;
     defense=basic_defense;
     spell_damage=basic_spell_damage;
     spell_resistance=basic_spell_resistance;
     movement_speed=basic_movement_speed;
     life_steal=basic_life_steal;
     money=600;
     keys[1]=progress[1]=true;
     keys[15]=progress[15]=true;
     keys[12]=progress[12]=true;
     keys[17]=progress[17]=true;
     keys[20]=progress[20]=true;
     keys[23]=progress[23]=true;
     keys[34]=progress[34]=true;
     keys[35]=progress[35]=true;
     keys[36]=progress[36]=true;
     keys[37]=progress[37]=true;
     keys[38]=progress[38]=true;
     keys[41]=progress[41]=true;
     keys[45]=progress[45]=true;
     keys[49]=progress[49]=true;
     keys[53]=progress[53]=true;
     keys[55]=progress[55]=true;
     spells[0].Set_id(6);
     spells[0].Load();
     spells[0].Unblock();
     number_of_items=1;
     number_of_items_bought[44]=1;
     items_bought[44].Set_id(44);
     items_bought[44].Load();
     Update();
     //Load();
     return;
    }
 fscanf(where,"%d %d %d",&money,&experience,&number_of_items);
 inventory_number_of_items=0;
 inventory_number_of_spells=0;
 int id,quantity;
 for(int i=1;i<=number_of_items;i++)
     {
      fscanf(where,"%d %d ",&id,&quantity);
      number_of_items_bought[id]=quantity;
      items_bought[id].Set_id(id);
      items_bought[id].Load();
      if(items_bought[id].Get_type()!=10)
         inventory_number_of_items++;
      if(items_bought[id].Get_type()==10)
         inventory_number_of_spells++;
     }
 for(int i=0;i<9;i++)
     fscanf(where,"%d ",&equipped_items_ids[i]);
 for(int i=0;i<9;i++)
     {
      equipped_items[i].Set_id(equipped_items_ids[i]);
      equipped_items[i].Set_type(i);
      equipped_items[i].Load();
     }
 fscanf(where,"%d %d %d",&basic_hp,&basic_mana,&basic_mental_health);
 fscanf(where,"%d %d %d %d %d %d",&basic_attack,&basic_defense,&basic_spell_damage,&basic_spell_resistance,&basic_movement_speed,&basic_life_steal);
 int w,h;
 fscanf(where,"%d %d ",&w,&h);
 skin_image_position.w=w;
 skin_image_position.h=h;
 original_skin_image_position=skin_image_position;
 attack=basic_attack;
 defense=basic_defense;
 spell_damage=basic_spell_damage;
 spell_resistance=basic_spell_resistance;
 movement_speed=basic_movement_speed;
 life_steal=basic_life_steal;
 fscanf(where,"%d ",&number_of_spells);
 for(int i=0;i<number_of_spells;i++)
     {
      int spell_id;
      fscanf(where,"%d ",&spell_id);
      spells[i].Set_id(spell_id);
      spells[i].Load();
     }
 int number_of_keys=0;
 keys.reset();
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_keys;i++)
     {
      int _key_id;
      fscanf(where,"%d ",&_key_id);
      keys[_key_id]=true;
     }
 number_of_keys=0;
 progress.reset();
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_keys;i++)
     {
      int _key_id;
      fscanf(where,"%d ",&_key_id);
      progress[_key_id]=true;
     }
 fscanf(where,"%d ",&number_of_wins);
 fclose(where);
 for(int i=0;i<number_of_spells;i++)
     spells[i].Unblock();
 Equip_items();
 Load_skin();
 Set_hp(basic_hp);
 Set_mana(basic_mana);
 Set_mental_health(basic_mental_health);
}

void Player::Fast_Reload()
{
 int _map_positionX=map_positionX,_map_positionY=map_positionY;
 char _name[TEXT_LENGTH_MAX]={NULL};
 strcpy(_name,name);
 Clear(true);
 Set_name(_name);
 Load();
 Set_map_position(_map_positionX,_map_positionY);
}

const int MAX_MONEY=666013,MAX_EXPERIENCE=2000;

void Player::Update()
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"saves/players/");
 strcat(path,name);
 strcat(path,".pwp");
 FILE *where=fopen(path,"w");
 money=std::min(money,MAX_MONEY);
 experience=std::min(experience,MAX_EXPERIENCE);
 fprintf(where,"%d\n%d\n%d\n",money,experience,number_of_items);
 for(int i=0;i<NUMBER_OF_ITEMS_IDS;i++)
     {
      if(number_of_items_bought[i]!=0)
         fprintf(where,"%d %d\n",i,number_of_items_bought[i]);
     }
 for(int i=0;i<9;i++)
     fprintf(where,"%d ",equipped_items_ids[i]);
 if(basic_hp<1000 && experience!=0)
    {
     basic_hp=100+experience*500/100;
     basic_hp=std::min(1000,basic_hp);
    }
 fprintf(where,"\n%d %d %d\n",basic_hp,basic_mana,basic_mental_health);
 fprintf(where,"%d %d %d %d %d %d\n",basic_attack,basic_defense,basic_spell_damage,basic_spell_resistance,basic_movement_speed,basic_life_steal);
 int w=original_skin_image_position.w,h=original_skin_image_position.h;
 fprintf(where,"%d %d\n",w,h);
 fprintf(where,"%d\n",number_of_spells);
 for(int i=0;i<number_of_spells;i++)
     {
      fprintf(where,"%d\n",spells[i].Get_id());
     }
 int number_of_keys=0;
 for(int i=0;i<NUMBER_OF_MAX_KEYS;i++)
     number_of_keys+=keys[i];
 fprintf(where,"%d\n",number_of_keys);
 for(int i=0;i<NUMBER_OF_MAX_KEYS;i++)
     if(keys[i])
        fprintf(where,"%d\n",i);
 number_of_keys=0;
 for(int i=0;i<progress.size();i++)
     number_of_keys+=progress[i];
 fprintf(where,"%d\n",number_of_keys);
 for(int i=0;i<progress.size();i++)
     if(progress[i])
        fprintf(where,"%d\n",i);
 fprintf(where,"%d\n",number_of_wins);
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
 if(_item.Get_type()==10 && inventory_number_of_spells>=INVENTORY_MAX_NUMBER_OF_ITEMS)
    return 3;
 if(Is_potion(_item) && number_of_items_bought[_item.Get_id()]>=INVENTORY_MAX_NUMBER_OF_POTIONS)
    return 4;
 if(Is_potion(_item) && number_of_items_bought[_item.Get_id()]==0 && inventory_number_of_items>=INVENTORY_MAX_NUMBER_OF_ITEMS)
    return 2;
 if(inventory_number_of_items>=INVENTORY_MAX_NUMBER_OF_ITEMS && (!Is_bought(_item_id) && !Is_potion(_item_id) && _item.Get_type()!=10))
    return 2;
 number_of_items_bought[_item_id]++,money-=_item.Get_cost();
 if(number_of_items_bought[_item_id]==1)
    {
     number_of_items++;
     if(_item.Get_type()!=10)
        inventory_number_of_items++;
     else
        inventory_number_of_spells++;
     items_bought[_item_id]=_item;
    }
 //_item.Clear(true);
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
     if(_item.Get_type()!=10)
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
 if(_item.Get_type()==10)
    for(int spell_pos=0;spell_pos<4;spell_pos++)
        if(spells[spell_pos].Get_id()==_item.Get_spell_id())
           {
            spells[spell_pos].Set_id(0);
            spells[spell_pos].Load();
           }
 //_item.Clear(true);
}

int Player::Get_pos_last_y()
{
 return pos_last_y;
}

int Player::Get_PLAYER_INFO_POSX()
{
 return PLAYER_INFO_POSX;
}

int Player::Get_PLAYER_INFO_LAST_POSX()
{
 return PLAYER_INFO_LAST_POSX;
}

int Player::Get_money()
{
 return money;
}

int Player::Get_experience()
{
 return experience;
}

void Player::Print_Character(int x,int y,SDL_Surface *_screen)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Surface *_image;
 int _x,_y;
 char message[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};

 equipped_items[8].Print(SKIN_POSX,y,_screen,false);

 apply_surface(x,y,PLAYER_name_background_shop,_screen);
 strcpy(message,name);
 _image=TTF_RenderText_Solid(font,message,NAME_COLOR);
 apply_surface(x+(SKIN_POSX-x+1-_image->w+10)/2,y,_image,_screen);
 y+=_image->h+20;
 SDL_FreeSurface(_image);

 apply_surface(x,y-10,PLAYER_details_background,_screen);

 apply_surface(x,y,PLAYER_experience_background,_screen);
 itoa(experience,aux);
 strcpy(message,"Experience: ");
 strcat(message,aux);
 _image=TTF_RenderText_Solid(font,message,EXPERIENCE_COLOR);
 apply_surface(x+10,y,_image,_screen);
 y+=_image->h;
 SDL_FreeSurface(_image);

 apply_surface(x,y,PLAYER_money_background,_screen);
 strcpy(message,"Money: ");
 _image=TTF_RenderText_Solid(font,message,MONEY_COLOR);
 apply_surface(x+10,y+10,_image,_screen);
 _x=x+_image->w+10,_y=y+10;
 SDL_FreeSurface(_image);
 itoa(money,aux);
 _image=TTF_RenderText_Solid(font,aux,MONEY_COLOR1);
 apply_surface(_x,_y,_image,_screen);
 _x+=_image->w,_y=y;
 SDL_FreeSurface(_image);
 apply_surface(_x,_y+5,COIN,_screen);
 y+=COIN->h+20;

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
 TTF_CloseFont(font);
}

void Player::Print_items(int x,int y,SDL_Surface *_screen)
{
  equipped_items[0].Print_image(x,y,_screen);
 //item.Print_description(x+190,y,_screen,false);
 y+=100;


 equipped_items[1].Print_image(x,y,_screen);
 //item.Print_description(x+190,y,_screen,false);
 equipped_items[4].Print_image(x+90,y,_screen);
 //item.Print_description(x+190,y,_screen,false);
 y+=100;

 equipped_items[2].Print_image(x,y,_screen);
 //item.Print_description(x+190,y,_screen,false);
 equipped_items[5].Print_image(x+90,y,_screen);
 y+=100;

 equipped_items[3].Print_image(x,y,_screen);
 //item.Print_description(x+190,y,_screen,false);
 y+=100;
 pos_last_y=y;
}

void Player::Print_Inventory(int x,int y,SDL_Surface *_screen,bool options,int type,bool allow_sales)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",15);
 char message[TEXT_LENGTH_MAX]={'x',NULL};
 SDL_Surface *_image=NULL;
 apply_surface(x,y,SHOP_inventory_background,_screen);
 int _x=x,_y=y;
 for(int i=0;i<=NUMBER_OF_ITEMS_IDS;i++)
     {
      if(number_of_items_bought[i]!=0)
         {
          if(items_bought[i].Get_id()==0/* || items_bought[i].Get_type()<6*/)
             continue;

          switch(type)
                 {
                  case 1:if(items_bought[i].Get_type()==10)
                            continue;
                         break;
                  case 2:if(items_bought[i].Get_type()!=10)
                            continue;
                         break;
                 }

          if(inventory_item_selected==i)
             apply_surface(_x,_y,SHOP_item_background_selected,_screen);
          else
             apply_surface(_x,_y,SHOP_item_background,_screen);
          items_bought[i].Print_inventory_image(_x,_y,_screen);
          itoa(number_of_items_bought[i],message+1);
          _image=TTF_RenderText_Solid(font,message,NUMBER_OF_ITEMS_COLOR);
          apply_surface(_x+42,_y+25,_image,_screen);
          SDL_FreeSurface(_image);
          if(!options)
             {
              _x+=60;
              if(_x>PLAYER_INFO_LAST_POSX)
                 _x=x,_y+=60;
              continue;
             }
          if(!Is_potion(i))
             {
              switch(type)
                     {
                      case 1:if(equipped_items_ids[items_bought[i].Get_type()]!=i)
                                apply_surface(_x+40,_y+2,INVENTORY_EQUIP,_screen);
                             else
                                apply_surface(_x+40,_y+2,INVENTORY_EQUIPPED,_screen);
                             break;

                      case 2:for(int spell_pos=0;spell_pos<4;spell_pos++)
                                 {
                                  if(spells[spell_pos].Get_id()==items_bought[i].Get_spell_id())
                                     apply_surface(_x+40+spell_pos*SHOP_inventory_spell_background->w,_y+2,SHOP_inventory_spell_background_equipped,_screen);
                                  else
                                     apply_surface(_x+40+spell_pos*SHOP_inventory_spell_background->w,_y+2,SHOP_inventory_spell_background,_screen);
                                  apply_surface(_x+40+spell_pos*SHOP_inventory_spell_background->w,_y+2,INVENTORY_spell_position[spell_pos],_screen);
                                 }
                             break;
                     }
             }
          if(allow_sales)
             apply_surface(_x+42,_y+15,INVENTORY_SELL,_screen);
          _x+=110;
          if(_x+110>PLAYER_INFO_LAST_POSX)
             _x=x,_y+=60;
         }
     }
 TTF_CloseFont(font);
}

void Player::Print_Inventory_equipped_items(int x,int y,SDL_Surface *_screen,bool options,int type,bool allow_sales)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",15);
 char message[TEXT_LENGTH_MAX]={'x',NULL};
 SDL_Surface *_image=NULL;
 apply_surface(x,y,SHOP_inventory_background,_screen);
 int _x=x,_y=y;
 for(int i=0;i<=NUMBER_OF_ITEMS_IDS;i++)
     {
      if(number_of_items_bought[i]!=0)
         {
          if(items_bought[i].Get_id()==0 || ((items_bought[i].Get_type()<6 || items_bought[i].Get_type()>9) || items_bought[i].Get_type()==8)/* || items_bought[i].Get_type()<6*/)
             continue;
          if(equipped_items_ids[items_bought[i].Get_type()]!=i && items_bought[i].Get_type()!=9)
             continue;
          switch(type)
                 {
                  case 1:if(items_bought[i].Get_type()==10)
                            continue;
                         break;
                  case 2:if(items_bought[i].Get_type()!=10)
                            continue;
                         break;
                 }

          if(inventory_item_selected==i)
             apply_surface(_x,_y,SHOP_item_background_selected,_screen);
          else
             apply_surface(_x,_y,SHOP_item_background,_screen);
          items_bought[i].Print_inventory_image(_x,_y,_screen);
          itoa(number_of_items_bought[i],message+1);
          _image=TTF_RenderText_Solid(font,message,NUMBER_OF_ITEMS_COLOR);
          apply_surface(_x+42,_y+25,_image,_screen);
          SDL_FreeSurface(_image);
          if(!options)
             {
              _x+=60;
              if(_x>PLAYER_INFO_LAST_POSX)
                 _x=x,_y+=60;
              continue;
             }
          if(!Is_potion(i))
             {
              if(equipped_items_ids[items_bought[i].Get_type()]!=i)
                 apply_surface(_x+40,_y+2,INVENTORY_EQUIP,_screen);
              else
                 {
                  apply_surface(_x+40,_y+2,INVENTORY_EQUIPPED,_screen);
                 }
             }
          if(allow_sales)
             apply_surface(_x+42,_y+15,INVENTORY_SELL,_screen);
          _x+=110;
          if(_x+110>PLAYER_INFO_LAST_POSX)
             _x=x,_y+=60;
         }
     }
 TTF_CloseFont(font);
}

int Player::Start_inventory(int x,int y,SDL_Surface *_screen,SDL_Event *event,int type,bool allow_sales)
{
 inventory_item_click=-1;
 int _x=x,_y=y;
 int mouse_x=event->button.x,mouse_y=event->button.y;
 bool _sell=false,_equip=false,_equip_spell=false;
 if(event->type==SDL_MOUSEMOTION || event->type==SDL_MOUSEBUTTONDOWN)
    {
     inventory_item_selected=-1;
     for(int i=0;i<=NUMBER_OF_ITEMS_IDS;i++)
         {
          if(number_of_items_bought[i]!=0)
             {
              switch(type)
                 {
                  case 1:if(items_bought[i].Get_type()==10)
                            continue;
                         break;
                  case 2:if(items_bought[i].Get_type()!=10)
                            continue;
                         break;
                 }

              if(mouse_x>=_x && mouse_x<_x+80 && mouse_y>=_y && mouse_y<_y+60)
                 inventory_item_selected=i,_sell=false,_equip=false;

              switch(type)
                     {
                      case 1:if(mouse_x>=_x+40 && mouse_x<=_x+40+INVENTORY_EQUIP->w && mouse_y>=_y+2 && mouse_y<=_y+2+INVENTORY_EQUIP->h)
                                 _equip=true;
                             break;

                      case 2:if(mouse_x>=_x+40 && mouse_x<=_x+40+4*SHOP_inventory_spell_background->w && mouse_y>=_y+2 && mouse_y<=_y+2+INVENTORY_EQUIP->h && (event->type==SDL_MOUSEBUTTONDOWN))
                                {
                                 for(int spell_pos=0;spell_pos<4;spell_pos++)
                                     {
                                      if(spells[spell_pos].Get_id()==items_bought[i].Get_spell_id())
                                         {
                                          spells[spell_pos].Clear(true);
                                          spells[spell_pos].Set_id(0);
                                          spells[spell_pos].Load();
                                         }
                                     }
                                 for(int spell_pos=0;spell_pos<4;spell_pos++)
                                     {
                                      if(mouse_x-_x-40>=spell_pos*SHOP_inventory_spell_background->w && mouse_x-_x-40<(spell_pos+1)*SHOP_inventory_spell_background->w)
                                         {
                                          spells[spell_pos].Clear(true);
                                          spells[spell_pos].Set_id(items_bought[i].Get_spell_id());
                                          spells[spell_pos].Load();
                                          _equip_spell=true;
                                         }
                                     }
                                }
                             break;
                     }

              if(mouse_x>=_x+42 && mouse_x<=_x+42+INVENTORY_EQUIP->w && mouse_y>=_y+3+INVENTORY_EQUIP->h && mouse_y<=_y+3+INVENTORY_EQUIP->h+INVENTORY_EQUIP->h)
                 _sell=true;
              _x+=110;
              if(_x+110>PLAYER_INFO_LAST_POSX)
                 _x=x,_y+=60;
             }
         }
    }
 if(event->type==SDL_MOUSEBUTTONDOWN)
    inventory_item_click=inventory_item_selected;
 if(inventory_item_click!=-1)
    {
     if(_sell && allow_sales)
        Sell(inventory_item_click);
     if(_equip)
        Equip(inventory_item_click);
    }
 //Print_Character(PLAYER_INFO_POSX,0,_screen);
 //Print_Inventory(x,y,_screen,true,type,allow_sales);
 if(_sell)
    return inventory_item_click;
 if(_equip)
    return -inventory_item_click;
 if(_equip_spell)
    return 666013;
 return 0;
}

///Game

void Player::Set_hp(int _hp)
{
 hp=_hp;
 if(hp<0)
    hp=0;
 if(hp>basic_hp)
    hp=basic_hp;
 if(is_immortal && hp==0)
    hp=1;
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 char aux[TEXT_LENGTH_MAX]={NULL};
 itoa(hp,aux);
 SDL_FreeSurface(hp_image);
 hp_image=TTF_RenderText_Solid(font,aux,HP_COLOR);
 TTF_CloseFont(font);
}

void Player::Set_mana(int _mana)
{
 mana=_mana;
 if(mana<0)
    mana=0;
 if(mana>basic_mana)
    mana=basic_mana;
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Surface *_image=NULL;
 char aux[TEXT_LENGTH_MAX]={NULL};
 itoa(mana,aux);
 SDL_FreeSurface(mana_image);
 mana_image=TTF_RenderText_Solid(font,aux,MANA_COLOR);
 TTF_CloseFont(font);
}

void Player::Set_mental_health(int _mental_health)
{
 mental_health=_mental_health;
}

void Player::Set_map_position(int x,int y)
{
 map_positionX=x;
 map_positionY=y;
}

void Player::Set_skin_image_position(int x,int y)
{
 skin_image_position.x=x;
 skin_image_position.y=y;
 skin_image_position.h=skin_size_h;
 skin_image_position.w=skin_size_w;
}

void Player::Set_skin_image_position(SDL_Rect _skin_image_position)
{
 skin_image_position=_skin_image_position;
 skin_size_h=_skin_image_position.h;
 skin_size_w=_skin_image_position.w;
}

void Player::Reset_skin_image_position()
{
 skin_image_position=original_skin_image_position;
 skin_size_h=skin_image_position.h;
 skin_size_w=skin_image_position.w;
}

void Player::Set_velocityX(int _velocityX)
{
 velocityX=_velocityX;
}

void Player::Set_velocityY(int _velocityY)
{
 velocityY=_velocityY;
}

void Player::Set_movement_speed(int _movement_speed)
{
 movement_speed=_movement_speed;
}

void Player::Block()
{
 is_blocked=true;
}

void Player::Block_attack()
{
 can_attack=false;
}

void Player::Unblock()
{
 is_blocked=false;
}

void Player::Unblock_attack()
{
 can_attack=true;
}

int Player::Get_hp()
{
 return hp;
}

int Player::Get_mana()
{
 return mana;
}

int Player::Get_attack()
{
 return attack;
}

int Player::Get_defense()
{
 return defense;
}

int Player::Get_extra_money()
{
 return extra_money;
}

int Player::Get_spell_damage()
{
 return spell_damage;
}

int Player::Get_spell_resistance()
{
 return spell_resistance;
}

int Player::Get_movement_speed()
{
 return movement_speed;
}

int Player::Get_life_steal()
{
 return life_steal;
}

int Player::Get_map_positionX()
{
 return map_positionX;
}

int Player::Get_map_positionY()
{
 return map_positionY;
}

int Player::Get_velocityX()
{
 return velocityX;
}

int Player::Get_velocityY()
{
 return velocityY;
}

int Player::Get_skinW()
{
 return skin_image_position.w;
}

int Player::Get_skinH()
{
 return skin_image_position.h;
}

char *Player::Get_name()
{
 return name;
}

void Player::Set_skin(char *_skin_name)
{
 char where[TEXT_LENGTH_MAX]={NULL};
 strcpy(where,"shop/skins/");
 strcat(where,_skin_name);
 strcat(where,".bmp");
 skin=make_it_transparent(where);
}

void Player::Load_skin()
{
 if(equipped_items_ids[8]==0)
    skin=make_it_transparent("shop/skins/timy skin.bmp");
 else
    skin=equipped_items[8].Get_skin();
}

void Player::Update_skin_image_position()
{
 switch(velocityX)
        {
         case 1:{skin_image_position.x=0;break;};
         case -1:{skin_image_position.x=skin_image_position.h;break;};
         case 0:{break;};
        }
}

void Player::Print_name(int x,int y,SDL_Surface *_screen)
{
 //apply_surface(x,y,PLAYER_name_background,_screen);
 //apply_surface(x,y,PLAYER_CASE_front,_screen);
 x-=name_image->w/2;
 apply_surface(x+(x+200-x+1-name_image->w+10)/2,y+name_image->h/4,name_image,_screen);
}

void Player::Print_name(SDL_Surface *_screen)
{
 apply_surface(PLAYER_INFO_POSX,0,PLAYER_name_background,_screen);
 apply_surface(PLAYER_INFO_POSX,0,PLAYER_CASE_front,_screen);
 apply_surface(PLAYER_INFO_POSX+(PLAYER_INFO_LAST_POSX-PLAYER_INFO_POSX+1-name_image->w+10)/2,name_image->h/4,name_image,_screen);
}

void Player::Print_hp(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y,PLAYER_CASE_background,_screen);
 apply_surface(x,y,PLAYER_HP_background->w*hp/basic_hp,PLAYER_HP_background->h,PLAYER_HP_background,_screen);
 apply_surface(x,y,PLAYER_CASE_front,_screen);
 apply_surface(x,(y+hp_image->h)/2,hp_image,_screen);
}

void Player::Print_hp(SDL_Surface *_screen)
{
 apply_surface(PLAYER_INFO_POSX,40,PLAYER_CASE_background,_screen);
 apply_surface(PLAYER_INFO_POSX,40,PLAYER_HP_background->w*hp/basic_hp,PLAYER_HP_background->h,PLAYER_HP_background,_screen);
 apply_surface(PLAYER_INFO_POSX,40,PLAYER_CASE_front,_screen);
 apply_surface(PLAYER_INFO_POSX+(PLAYER_INFO_LAST_POSX-PLAYER_INFO_POSX+1-hp_image->w+10)/2,40+(40-hp_image->h)/2,hp_image,_screen);
}

void Player::Print_mana(int x,int y,SDL_Surface *_screen)
{
 apply_surface(x,y,PLAYER_CASE_background,_screen);
 apply_surface(x,y,PLAYER_MANA_background->w*mana/basic_mana,PLAYER_MANA_background->h,PLAYER_MANA_background,_screen);
 apply_surface(x,y,PLAYER_CASE_front,_screen);
 apply_surface(x,y,mana_image,_screen);
 apply_surface(x+mana_image->w+10,y,MANA,_screen);
}

void Player::Print_mana(SDL_Surface *_screen)
{
 apply_surface(PLAYER_INFO_POSX,80,PLAYER_CASE_background,_screen);
 apply_surface(PLAYER_INFO_POSX,80,PLAYER_MANA_background->w*mana/basic_mana,PLAYER_MANA_background->h,PLAYER_MANA_background,_screen);
 apply_surface(PLAYER_INFO_POSX,80,PLAYER_CASE_front,_screen);
 apply_surface(PLAYER_INFO_POSX+(PLAYER_INFO_LAST_POSX-PLAYER_INFO_POSX+1-mana_image->w+10)/2,80+(40-mana_image->h)/2,mana_image,_screen);
}

void Player::Print_skin(int x,int y,int mapX,int mapY,SDL_Surface *_screen)
{
 apply_surface(skin_image_position.x,skin_image_position.y,x+(map_positionX-mapX)*PIXELS_PER_INGAME_UNIT,y+(map_positionY-mapY)*PIXELS_PER_INGAME_UNIT,skin_image_position.w,skin_image_position.h,skin,_screen);
}

void Player::Print_skin_free(int x,int y,SDL_Surface *_screen)
{
 apply_surface(skin_image_position.x,skin_image_position.y,x,y,skin_image_position.w,skin_image_position.h,skin,_screen);
}

void Player::Equip_items()
{
 for(int i=0;i<9;i++)
     {
      attack+=equipped_items[i].Get_attack();
      defense+=equipped_items[i].Get_defense();
      extra_money+=equipped_items[i].Get_extra_money();
      spell_damage+=equipped_items[i].Get_spell_damage();
      spell_resistance+=equipped_items[i].Get_spell_resistance();
      movement_speed+=equipped_items[i].Get_movement_speed();
      if(equipped_items[i].Buff_Is_printable())
         printable_item_buffs_id.push_back(i);
     }
}

bool Player::Is_blocked()
{
 return is_blocked;
}

bool Player::Can_attack()
{
 return can_attack;
}

void Player::Move_X()
{
 map_positionX+=velocityX;
}

void Player::Move_Y()
{
 map_positionY+=velocityY;
}

void Player::Use_hp_potion()
{
 if(number_of_items_bought[15]>0)
    {
     Set_hp(hp+items_bought[15].Get_hp());
     number_of_items_bought[15]--;
     if(number_of_items_bought[15]==0)
        number_of_items--;
    }
}

void Player::Use_mana_potion()
{
 if(number_of_items_bought[16]>0)
    {
     Set_mana(mana+items_bought[16].Get_mana());
     number_of_items_bought[16]--;
     if(number_of_items_bought[16]==0)
        number_of_items--;
    }
}

bool Player::Is_immortal()
{
 return is_immortal;
}

void Player::Reset(int _map_positionX,int _map_positionY)
{
 map_positionX=_map_positionX;
 map_positionY=_map_positionY;
 Load_skin();
 Reset_skin_image_position();
 Set_mana(basic_mana);
 Set_hp(basic_hp);
 for(int i=0;!active_buffs.empty() && i<active_buffs.size();i++)
     {
      Remove_buff(&active_buffs[i]);
      active_buffs[i].Clear(false);
     }
 //active_buffs.clear();
 std::vector<Buff>().swap(active_buffs);
 std::vector<int>().swap(printable_item_buffs_id);
 for(int i=0;i<number_of_spells;i++)
     spells[i].Unblock();
 attack=basic_attack;
 defense=basic_defense;
 spell_damage=basic_spell_damage;
 spell_resistance=basic_spell_resistance;
 movement_speed=basic_movement_speed;
 life_steal=basic_life_steal;
 Equip_items();
}

///Game

///Buffs
void Player::Apply_buff(Buff *_buff)
{
 switch(_buff->Get_type())
        {
         ///TRANSMITTED
         case 1:_buff->Set_transmitted_buff_id(((rand()%100)<=_buff->Get_chance())?_buff->Get_transmissible_buff_id():0);
                break;
         ///HP
         case 2:if(_buff->Get_damage()<0)
                   hp+=(_buff->Get_damage())+_buff->Get_damage()*Get_spell_resistance()/100;
                else
                   hp+=_buff->Get_damage();
                Set_hp(std::min(basic_hp,hp));
                break;
         ///MANA
         case 3:mana+=_buff->Get_mana();
                Set_mana(std::min(basic_mana,mana));
                break;
         ///SELF BUFF
         case 4:if(_buff->Get_remaining_duration()<_buff->Get_duration())
                   break;
                attack+=_buff->Get_attack();
                defense+=_buff->Get_defense();
                spell_damage+=_buff->Get_spell_damage();
                spell_resistance+=_buff->Get_spell_resistance();
                movement_speed+=_buff->Get_movement_speed();
                life_steal+=_buff->Get_life_steal();
                break;
         ///SHAPESHIFT
         case 5:if(_buff->Get_remaining_duration()<_buff->Get_duration())
                   break;
                attack+=_buff->Get_attack();
                defense+=_buff->Get_defense();
                spell_damage+=_buff->Get_spell_damage();
                spell_resistance+=_buff->Get_spell_resistance();
                movement_speed+=_buff->Get_movement_speed();
                life_steal+=_buff->Get_life_steal();
                Set_skin_image_position(_buff->Get_skin_image_position());
                Set_skin(_buff->Get_skin_name());
                break;
         ///LAST BREATH
         case 6:if(_buff->Get_remaining_duration()<_buff->Get_duration())
                   break;
                is_immortal=true;
                break;
         default:break;
        }
 _buff->Decrease_remaining_duration();
}

void Player::Remove_buff(Buff *_buff)
{
 switch(_buff->Get_type())
        {
         case 4:attack-=_buff->Get_attack();
                defense-=_buff->Get_defense();
                spell_damage-=_buff->Get_spell_damage();
                spell_resistance-=_buff->Get_spell_resistance();
                movement_speed-=_buff->Get_movement_speed();
                life_steal-=_buff->Get_life_steal();
                break;
         case 5:attack-=_buff->Get_attack();
                defense-=_buff->Get_defense();
                spell_damage-=_buff->Get_spell_damage();
                spell_resistance-=_buff->Get_spell_resistance();
                movement_speed-=_buff->Get_movement_speed();
                life_steal-=_buff->Get_life_steal();
                Load_skin();
                Reset_skin_image_position();
                break;
         case 6:is_immortal=false;
                break;
         default:break;
        }
}

void Player::Apply_all_buffs()
{
 Apply_item_buffs();
 Apply_active_buffs();
}

void Player::Apply_new_active_buff()
{
 Apply_buff(&active_buffs[active_buffs.size()-1]);
}

void Player::Apply_active_buffs()
{
 for(std::vector<Buff>::iterator i=active_buffs.begin();i!=active_buffs.end();i)
     {
      Apply_buff(&(*i));
      if((*i).Is_done())
         {
          Buff x=*i;
          Remove_buff(&(*i));
          active_buffs.erase(i);
         }
      else
         i++;
     }
}

void Player::Apply_item_buffs()
{
 Buff aux;
 for(int i=0;i<9;i++)
     {
      Apply_buff(&(aux=equipped_items[i].Get_Buff()));
      equipped_items[i].Set_Buff(aux);
     }
}

void Player::Add_buff(Buff _buff)
{
 if(_buff.Get_id()==0)
    return;
 for(std::vector<Buff>::iterator i=active_buffs.begin();i!=active_buffs.end();i++)
     if(_buff.Get_id()==(*i).Get_id())
        {
         (*i).Reset();
         return;
        }
 active_buffs.push_back(_buff);
}

void Player::Get_transmitted_buffs(std::vector<Buff> *_buffs)
{
 Buff _copy,aux;
 for(int i=0;i<9;i++)
     {
      _copy=equipped_items[i].Get_Buff();
      if(_copy.Get_id()!=0 && _copy.Is_transmissible())
         {
          aux.Set_id(_copy.Get_transmitted_buff_id());
          if(aux.Get_id()!=0)
             {
              aux.Load();
              _buffs->push_back(aux);
             }
         }
     }
 for(std::vector<Buff>::iterator i=active_buffs.begin();i!=active_buffs.end();i++)
     {
      _copy=*i;
      if(_copy.Get_id()!=0 && _copy.Is_transmissible())
         {
          aux.Set_id(_copy.Get_transmitted_buff_id());
          if(aux.Get_id()!=0)
             {
              aux.Load();
              _buffs->push_back(aux);
             }
         }
     }
}

void Player::Print_buffs(int x,int y,SDL_Surface *_screen)
{
 Buff aux;
 for(std::vector<int>::iterator i=printable_item_buffs_id.begin();i!=printable_item_buffs_id.end();i++)
     {
      aux=equipped_items[*i].Get_Buff();
      if(aux.Get_id()!=0)
         {
          aux.Print_image(x,y,_screen);
          x+=40;
         }
     }
 for(std::vector<Buff>::iterator i=active_buffs.begin();i!=active_buffs.end();i++)
     {
      if((*i).Get_id()!=0)
         {
          (*i).Print_image(x,y,_screen);
          x+=40;
         }
     }
}

///Spells

bool Player::Pay_Spell(int spell_pos)
{
 if(spells[spell_pos].Get_id()==0)
    return false;
 bool rtn=spells[spell_pos].Pay(&mana,&hp,&mental_health);
 Set_mana(mana);
 Set_hp(hp);
 //Set_mental_health(mental_health);
 return rtn;
}

Spell Player::Get_Spell(int spell_pos)
{
 return spells[spell_pos];
}

int Player::Get_number_of_spells()
{
 return number_of_spells;
}

void Player::Block_Spell(int spell_pos)
{
 spells[spell_pos].Block();
}

void Player::Decrease_Spell_time_blocked(int spell_pos)
{
 spells[spell_pos].Decrease_time_blocked();
}

void Player::Decrease_all_Spells_time_blocked()
{
 for(int i=0;i<4;i++)
     Decrease_Spell_time_blocked(i);
}

bool Player::Spell_Is_blocked(int spell_pos)
{
 return spells[spell_pos].Is_blocked();
}

void Player::Print_spells(int x,int y,SDL_Surface *_screen)
{
 for(int spell_pos=0;spell_pos<4;spell_pos++)
     {
      apply_surface(x,y,PLAYER_SPELLS_background,_screen);
      if(spells[spell_pos].Get_id()!=0)
         {
          spells[spell_pos].Print_image(x,y,_screen);
          if(Spell_Is_blocked(spell_pos))
             apply_surface(x,y,PLAYER_SPELLS_not_ready,_screen);
          else
             if(!spells[spell_pos].Can_Pay(mana,hp,mental_health))
                apply_surface(x,y,PLAYER_SPELLS_no_mana,_screen);
         }
      apply_surface(x,y,PLAYER_SPELLS_front,_screen);
      x+=PLAYER_SPELLS_background->w+10;
     }
}

///Keys
void Player::Add_key(int _key_id)
{
 if(_key_id==0 || progress[_key_id]==true)
    return;
 keys[_key_id]=true;
 progress[_key_id]=true;
}

void Player::Add_keys(std::bitset<NUMBER_OF_MAX_KEYS> *_keys_ids)
{
 for(int i=0;i<_keys_ids->size();i++)
     {
      if((*_keys_ids)[i]==false)
         continue;
      if(progress[i]==true)
         continue;
      keys[i]=true;
      progress[i]=true;
     }
 //keys|=(*_keys_ids);
}

void Player::Remove_keys(std::bitset<NUMBER_OF_MAX_KEYS> *_keys_ids)
{

 for(int i=0;i<_keys_ids->size();i++)
     {
      keys[i]=((*_keys_ids)[i])?false:keys[i];
     }
}

void Player::Remove_key(int _key_id)
{
 if(_key_id==0)
    return;
 keys[_key_id]=false;
}

std::bitset<NUMBER_OF_MAX_KEYS> *Player::Get_keys()
{
 return &keys;
}

std::bitset<NUMBER_OF_MAX_KEYS> *Player::Get_progress()
{
 return &progress;
}

///Score
void Player::Increase_number_of_wins()
{
 number_of_wins++;
}

int Player::Get_number_of_wins()
{
 return number_of_wins;
}

 ///AI
void Player::Start_spell_timer(int _spell_id)
{
 spell_timer[_spell_id].start();
}

int Player::Get_spell_timer_ticks(int _spell_id)
{
 return spell_timer[_spell_id].get_ticks();
}

void Player::Start_attack_timer()
{
 attack_timer.start();
}

int Player::Get_attack_timer_ticks()
{
 return attack_timer.get_ticks();
}

void Player::Set_counter(int _counter)
{
 counter=_counter;
}

int Player::Get_counter()
{
 return counter;
}
