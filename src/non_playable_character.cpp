#include "non_playable_character.h"

void Non_Playable_Character::Clear()
{
 name[0]=NULL;
 map_positionX=0;
 map_positionY=0;
 type=0;
 script_name[0]=NULL;
 afterscript_name[0]=NULL;
 shop_name[0]=NULL;
 duel_mode_level_name[0]=NULL;
 puzzle_name[0]=NULL;
 std::vector<std::pair<int,int> >().swap(map_accessible_positions);
 chance_to_move=0;
 keys_to_give.reset();
 keys_to_take.reset();
 if(skin_image!=NULL)
    Destroy_Texture(skin_image);
 skin_image=NULL;
 skin_image_position.x=skin_image_position.y=0;
 skin_image_position.w=skin_image_position.h=PIXELS_PER_INGAME_UNIT;
 lose_posX=-1;
 lose_posY=-1;
}

void Non_Playable_Character::Set_name(char *_name)
{
 strcpy(name,_name);
}

int Non_Playable_Character::Set_map_positionX(int _map_positionX)
{
 map_positionX=_map_positionX;
}

int Non_Playable_Character::Set_map_positionY(int _map_positionY)
{
 map_positionY=_map_positionY;
}

int Non_Playable_Character::Get_chance_to_move()
{
 return chance_to_move;
}

bool Non_Playable_Character::Is_accessible(int x,int y)
{
 for(int i=0;i<map_accessible_positions.size();i++)
     if(map_accessible_positions[i].first==x && map_accessible_positions[i].second==y)
        return true;
 return false;
}

bool Non_Playable_Character::Is_static()
{
 return map_accessible_positions.size()==1;
}

bool Non_Playable_Character::Is_blocked()
{
 return is_blocked;
}

int Non_Playable_Character::Get_skinW()
{
 return skin_image_position.w;
}

int Non_Playable_Character::Get_skinH()
{
 return skin_image_position.h;
}

int Non_Playable_Character::Get_map_positionX()
{
 return map_positionX;
}

int Non_Playable_Character::Get_map_positionY()
{
 return map_positionY;
}

int Non_Playable_Character::Get_last_dir()
{
 return last_dir;
}

int Non_Playable_Character::Get_type()
{
 return type;
}

std::bitset<NUMBER_OF_MAX_KEYS> *Non_Playable_Character::Get_keys()
{
 return &keys_to_give;
}

std::bitset<NUMBER_OF_MAX_KEYS> *Non_Playable_Character::Get_keys_to_take()
{
 return &keys_to_take;
}

char *Non_Playable_Character::Get_script_name()
{
 return script_name;
}

char *Non_Playable_Character::Get_afterscript_name()
{
 return afterscript_name;
}

char *Non_Playable_Character::Get_shop_name()
{
 return shop_name;
}

char *Non_Playable_Character::Get_duel_mode_level_name()
{
 return duel_mode_level_name;
}

char *Non_Playable_Character::Get_puzzle_name()
{
 return puzzle_name;
}

int Non_Playable_Character::Get_range()
{
 return range;
}

int Non_Playable_Character::Get_lose_posX()
{
 return lose_posX;
}

int Non_Playable_Character::Get_lose_posY()
{
 return lose_posY;
}

void Non_Playable_Character::Update_skin(int dir)
{
 int dirx[]={1,0,-1,0};
 int diry[]={0,1,0,-1};
 if(dir==0)
    skin_image_position.x=0;
 if(dir==2)
    skin_image_position.x=skin_image_position.w;
 last_dir=dir;
}

void Non_Playable_Character::Block()
{
 is_blocked=true;
}

void Non_Playable_Character::Unblock()
{
 is_blocked=false;
}

void Non_Playable_Character::Load(std::bitset<NUMBER_OF_MAX_KEYS> *key,SDL_Rect player_pos)
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"NPC/");
 strcat(path,name);
 strcat(path,".pwnpc");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    return;
 int key_id=0;
 fscanf(where,"%d ",&key_id);
 if(key_id!=0 && (!(*key)[key_id]))
    {
     fclose(where);
     return;
    }
 int number_of_keys=0;
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_keys;i++)
     {
      int _key_id;
      fscanf(where,"%d ",&_key_id);
      keys_to_give[_key_id]=true;
     }
 number_of_keys=0;
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_keys;i++)
     {
      int _key_id;
      fscanf(where,"%d ",&_key_id);
      keys_to_take[_key_id]=true;
     }
 int number_of_map_accessible_positions=0;
 fscanf(where,"%d ",&number_of_map_accessible_positions);
 for(int i=0;i<number_of_map_accessible_positions;i++)
     {
      int x,y;
      fscanf(where,"%d %d ",&x,&y);
      map_accessible_positions.push_back(std::make_pair(x,y));
     }
 fscanf(where,"%d ",&chance_to_move);
 char skin_name[TEXT_LENGTH_MAX]={NULL};
 fgets(skin_name,sizeof skin_name,where);
 if(skin_name[strlen(skin_name)-1]=='\n')
    skin_name[strlen(skin_name)-1]=NULL;
 strcpy(path,"NPC/skins/");
 strcat(path,skin_name);
 strcat(path,".png");
 skin_image=Load_Transparent_Texture(path);
 int w,h;
 fscanf(where,"%d %d ",&w,&h);
 skin_image_position.w=w;
 skin_image_position.h=h;
 skin_image_position.x=skin_image_position.y=0;
 std::vector<int> available_positions;
 for(int i=0;i<number_of_map_accessible_positions;i++)
     {
      SDL_Rect NPC_position={map_accessible_positions[i].first,map_accessible_positions[i].second,Get_skinW()/PIXELS_PER_INGAME_UNIT,Get_skinH()/PIXELS_PER_INGAME_UNIT};
      if((NPC_position.x>player_pos.x+player_pos.w-1 || NPC_position.x+NPC_position.w-1<player_pos.x) || (NPC_position.y>player_pos.y+player_pos.h-1 || NPC_position.y+NPC_position.h-1<player_pos.y))
         available_positions.push_back(i);
     }
 #ifdef DEBUG
 if(available_positions.size()==0)
    {
     FILE *out=fopen("err/debug.txt","a");
     fprintf(out,"0 available positions %s x=%d,y=%d,w=%d,h=%d,player:x=%d,y=%d,w=%d,h=%d",name,0,0,Get_skinW()/PIXELS_PER_INGAME_UNIT,Get_skinH()/PIXELS_PER_INGAME_UNIT,
                                                                                           player_pos.x,player_pos.y,player_pos.w,player_pos.h);
     fclose(out);
    }
 #endif // DEBUG
 if(available_positions.size()==0)
    {
     fclose(where);
     return;
    }
 int random_position=rand()%available_positions.size();
 map_positionX=map_accessible_positions[available_positions[random_position]].first,map_positionY=map_accessible_positions[available_positions[random_position]].second;
 fscanf(where,"%d ",&type);
 fgets(script_name,sizeof script_name,where);
 if(script_name[strlen(script_name)-1]=='\n')
    script_name[strlen(script_name)-1]=NULL;
 lose_posX=-1;
 lose_posY=-1;
 switch(type)
        {
         case 2:fgets(shop_name,sizeof shop_name,where);
                if(shop_name[strlen(shop_name)-1]=='\n')
                   shop_name[strlen(shop_name)-1]=NULL;
                fgets(afterscript_name,sizeof afterscript_name,where);
                if(afterscript_name[strlen(afterscript_name)-1]=='\n')
                   afterscript_name[strlen(afterscript_name)-1]=NULL;
                break;
         case 3:fgets(duel_mode_level_name,sizeof duel_mode_level_name,where);
                if(duel_mode_level_name[strlen(duel_mode_level_name)-1]=='\n')
                   duel_mode_level_name[strlen(duel_mode_level_name)-1]=NULL;
                fscanf(where,"%d ",&range);
                fgets(afterscript_name,sizeof afterscript_name,where);
                if(afterscript_name[strlen(afterscript_name)-1]=='\n')
                   afterscript_name[strlen(afterscript_name)-1]=NULL;
                fscanf(where,"%d %d ",&lose_posX,&lose_posY);
                break;
         case 4:fgets(puzzle_name,sizeof puzzle_name,where);
                if(puzzle_name[strlen(puzzle_name)-1]=='\n')
                   puzzle_name[strlen(puzzle_name)-1]=NULL;
                fgets(afterscript_name,sizeof afterscript_name,where);
                if(afterscript_name[strlen(afterscript_name)-1]=='\n')
                   afterscript_name[strlen(afterscript_name)-1]=NULL;
                fscanf(where,"%d %d ",&lose_posX,&lose_posY);
                break;
         default:break;
        }
 if(type!=3)
    {
     range=0;
     if(!feof(where))
        fscanf(where,"%d ",&range);
    }
 fclose(where);
}

void Non_Playable_Character::Load(char *_name,std::bitset<NUMBER_OF_MAX_KEYS> *key,SDL_Rect player_pos)
{
 Set_name(_name);
 Load(key,player_pos);
}

void Non_Playable_Character::Print_skin(int x,int y,int mapX,int mapY,int mapW,int mapH,Texture *_screen)
{
 if(type==0)
    return;
 if((Get_map_positionX()-(mapX>0?mapX:0)>=mapW/PIXELS_PER_INGAME_UNIT) ||
    (Get_map_positionY()-(mapY>0?mapY:0)>=mapH/PIXELS_PER_INGAME_UNIT) ||
    (Get_map_positionX()-(mapX>0?mapX:0)+Get_skinW()/PIXELS_PER_INGAME_UNIT-1<0) ||
    (Get_map_positionY()-(mapY>0?mapY:0)+Get_skinH()/PIXELS_PER_INGAME_UNIT-1<0))
    return;
 SDL_Rect _skin_image_position=skin_image_position;
 if(Get_map_positionY()-mapY+Get_skinH()/PIXELS_PER_INGAME_UNIT-1>=mapH/PIXELS_PER_INGAME_UNIT && Get_map_positionY()-mapY<mapH/PIXELS_PER_INGAME_UNIT)
    _skin_image_position.h=(mapH/PIXELS_PER_INGAME_UNIT-(Get_map_positionY()-mapY))*PIXELS_PER_INGAME_UNIT;
 if(Get_map_positionY()-mapY<0 && Get_map_positionY()-mapY+Get_skinH()/PIXELS_PER_INGAME_UNIT-1>=0)
    _skin_image_position.h=(Get_skinH()/PIXELS_PER_INGAME_UNIT-(-Get_map_positionY()+mapY))*PIXELS_PER_INGAME_UNIT,_skin_image_position.y=(0-(Get_map_positionY()-mapY))*PIXELS_PER_INGAME_UNIT;
 if(Get_map_positionX()-mapX+Get_skinW()/PIXELS_PER_INGAME_UNIT-1>=mapW/PIXELS_PER_INGAME_UNIT && Get_map_positionX()-mapX<mapW/PIXELS_PER_INGAME_UNIT)
    _skin_image_position.w=(mapW/PIXELS_PER_INGAME_UNIT-(Get_map_positionX()-mapX))*PIXELS_PER_INGAME_UNIT;
 if(Get_map_positionX()-mapX<0 && Get_map_positionX()-mapX+Get_skinW()/PIXELS_PER_INGAME_UNIT-1>=0)
    _skin_image_position.w=(Get_skinW()/PIXELS_PER_INGAME_UNIT-(-Get_map_positionX()+mapX))*PIXELS_PER_INGAME_UNIT,_skin_image_position.x+=(Get_skinW()/PIXELS_PER_INGAME_UNIT+(Get_map_positionX()-mapX))*PIXELS_PER_INGAME_UNIT;
 Apply_Texture(_skin_image_position.x,_skin_image_position.y,-skin_image_position.x+_skin_image_position.x+x+(map_positionX-mapX)*PIXELS_PER_INGAME_UNIT,-skin_image_position.y+_skin_image_position.y+y+(map_positionY-mapY)*PIXELS_PER_INGAME_UNIT,_skin_image_position.w,_skin_image_position.h,skin_image,_screen);
}
