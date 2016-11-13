#include "non_playable_character.h"

void Non_Playable_Character::Clear()
{
 name[0]=NULL;
 map_positionX=0;
 map_positionY=0;
 type=0;
 script_name[0]=NULL;
 shop_name[0]=NULL;
 std::vector<std::pair<int,int> >().swap(map_accessible_positions);
 chance_to_move=0;
 if(skin_image!=NULL)
    SDL_FreeSurface(skin_image);
 skin_image=NULL;
 skin_image_position.x=skin_image_position.y=0;
 skin_image_position.w=skin_image_position.h=40;
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

char *Non_Playable_Character::Get_script_name()
{
 return script_name;
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

void Non_Playable_Character::Load(std::bitset<NUMBER_OF_MAX_KEYS> *key)
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
 int number_of_map_accessible_positions=0;
 fscanf(where,"%d ",&number_of_map_accessible_positions);
 for(int i=0;i<number_of_map_accessible_positions;i++)
     {
      int x,y;
      fscanf(where,"%d %d ",&x,&y);
      map_accessible_positions.push_back(std::make_pair(x,y));
     }
 map_positionX=map_accessible_positions[0].first;
 map_positionY=map_accessible_positions[0].second;
 fscanf(where,"%d ",&chance_to_move);
 char skin_name[TEXT_LENGTH_MAX]={NULL};
 fgets(skin_name,sizeof skin_name,where);
 if(skin_name[strlen(skin_name)-1]=='\n')
    skin_name[strlen(skin_name)-1]=NULL;
 strcpy(path,"NPC/skins/");
 strcat(path,skin_name);
 strcat(path,".bmp");
 skin_image=make_it_transparent(path);
 int w,h;
 fscanf(where,"%d %d ",&w,&h);
 skin_image_position.w=w;
 skin_image_position.h=h;
 skin_image_position.x=skin_image_position.y=0;
 fscanf(where,"%d ",&type);
 fgets(script_name,sizeof script_name,where);
 if(script_name[strlen(script_name)-1]=='\n')
    script_name[strlen(script_name)-1]=NULL;
 fclose(where);
}

void Non_Playable_Character::Load(char *_name,std::bitset<NUMBER_OF_MAX_KEYS> *key)
{
 Set_name(_name);
 Load(key);
}

//void Non_Playable_Character::Interact(char *player_name)

void Non_Playable_Character::Print_skin(int x,int y,int mapX,int mapY,SDL_Surface *_screen)
{
 apply_surface(skin_image_position.x,skin_image_position.y,x+(map_positionX-mapX)*40,y+(map_positionY-mapY)*40,skin_image_position.w,skin_image_position.h,skin_image,_screen);
}
