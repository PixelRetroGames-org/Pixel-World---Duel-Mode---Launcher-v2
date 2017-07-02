#include "map_texture.h"

Map_Texture::Map_Texture()
{
 image_position.h=PIXELS_PER_INGAME_UNIT;
 image_position.w=PIXELS_PER_INGAME_UNIT;
}

void Map_Texture::Clear(bool _delete)
{
 if(image!=NULL && _delete)
    {
     Destroy_Texture(image);
     image=NULL;
    }
 id=0;
 buff.Clear(_delete);
 frame_change_delay=0;
 is_trigger=false;
 is_passer=false;
}

void Map_Texture::Set_id(int _id)
{
 id=_id;
}

void Map_Texture::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Map_Texture::Set_type(int _type)
{
 type=_type;
}

int Map_Texture::Get_id()
{
 return id;
}

int Map_Texture::Get_type()
{
 return type;
}

bool Map_Texture::Get_print_before_player()
{
 return print_before_player;
}

bool Map_Texture::Is_obstacle()
{
 return obstacle;
}

bool Map_Texture::Is_light()
{
 return light;
}

bool Map_Texture::Is_animation()
{
 return animation;
}

bool Map_Texture::Is_trigger()
{
 return is_trigger;
}

bool Map_Texture::Is_passer()
{
 return is_passer;
}

Buff Map_Texture::Get_Buff()
{
 return buff;
}

int Map_Texture::Get_power()
{
 return power;
}

int Map_Texture::Get_number_of_frames()
{
 return number_of_frames;
}

int Map_Texture::Get_frame_change_delay()
{
 return frame_change_delay;
}

void Map_Texture::Load()
{
 if(id==0)
    return;
 char path[TEXT_LENGTH_MAX]={NULL};
 char aux[TEXT_LENGTH_MAX]={NULL};
 itoa(id,aux);
 strcpy(path,"maps/textures/");
 strcat(path,aux);
 strcat(path,".pwt");
 FILE *where=fopen(path,"r");
 int buff_id,_print_before_player;
 fscanf(where,"%d ",&number_of_frames);
 fscanf(where,"%d ",&type);
 fscanf(where,"%d ",&buff_id);
 fscanf(where,"%d ",&_print_before_player);
 fscanf(where,"%s ",name);
 int _obstacle,_light,_animation;
 fscanf(where,"%d ",&_obstacle);
 fscanf(where,"%d ",&_light);
 fscanf(where,"%d ",&_animation);
 obstacle=(bool)_obstacle,light=(bool)_light,animation=(bool)_animation,print_before_player=(int)_print_before_player;
 fscanf(where,"%d ",&power);
 if(!feof(where))
    fscanf(where,"%d ",&frame_change_delay);
 if(!feof(where))
    fscanf(where,"%d ",&is_trigger);
 if(!feof(where))
    fscanf(where,"%d ",&is_passer);
 buff.Set_id(buff_id);
 buff.Load();
 strcpy(path,"maps/textures/images/");
 strcat(path,name);
 strcat(path,".png");
 image=Load_Transparent_Texture(path);
 fclose(where);
}

void Map_Texture::Print_image(int x,int y,Texture *_screen,int frame)
{
 if(image==NULL)
    return;
 image_position.x=frame*PIXELS_PER_INGAME_UNIT;
 image_position.y=0;
 image_position.h=PIXELS_PER_INGAME_UNIT;
 image_position.w=PIXELS_PER_INGAME_UNIT;
 Apply_Texture(image_position.x,image_position.y,x,y,image_position.h,image_position.w,image,_screen);
}
