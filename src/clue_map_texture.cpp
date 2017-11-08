#include "clue_map_texture.h"
#include "script_interpreter.h"

void Clue_map_texture::Clear()
{
 id=type=0;
 interactive_map_texture.Clear();
}

void Clue_map_texture::Load()
{
 char filename[TEXT_LENGTH_MAX]={NULL},aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(filename,"maps/clue map textures/");
 itoa(id,aux);
 strcat(filename,aux);
 strcat(filename,".pwcmt");
 FILE *where=fopen(filename,"r");
 if(where==NULL)
    return;
 int interactive_map_texture_id;
 fscanf(where,"%d ",&interactive_map_texture_id);
 interactive_map_texture.Set_id(interactive_map_texture_id);
 interactive_map_texture.Load();
 fscanf(where,"%d ",&type);
 if(type!=0)
    {
     fgets(script_name,sizeof script_name,where);
     if(script_name[strlen(script_name)-1]=='\n')
        script_name[strlen(script_name)-1]=NULL;
    }
 int number_of_keys;
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_keys;i++)
     {
      int key_id;
      fscanf(where,"%d ",&key_id);
      keys_to_give[key_id]=true;
     }
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_keys;i++)
     {
      int key_id;
      fscanf(where,"%d ",&key_id);
      keys_to_take[key_id]=true;
     }
 fclose(where);
}

void Clue_map_texture::Load(int _id)
{
 Set_id(_id);
 Load();
}

void Clue_map_texture::Set_id(int _id)
{
 id=_id;
}

Interactive_map_texture *Clue_map_texture::Get_interactive_map_texture()
{
 return &interactive_map_texture;
}

std::bitset<NUMBER_OF_MAX_KEYS> *Clue_map_texture::Get_keys()
{
 return &keys_to_give;
}

std::bitset<NUMBER_OF_MAX_KEYS> *Clue_map_texture::Get_keys_to_take()
{
 return &keys_to_take;
}

int Clue_map_texture::Get_id()
{
 return id;
}

int Clue_map_texture::Get_type()
{
 return type;
}

char *Clue_map_texture::Get_script_name()
{
 if(type==0)
    return NULL;
 return script_name;
}

void Clue_map_texture::Start_action(Texture *_screen)
{
 if(type==0)
    return;
 Script_interpreter script_interpreter;
 script_interpreter.Start(_screen,script_name);
 SDL_Delay(100);
 SDL_PumpEvents();
}
