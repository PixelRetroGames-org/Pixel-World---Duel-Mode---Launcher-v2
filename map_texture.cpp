#include "map_texture.h"

Texture::Texture()
{
 image_position.h=40;
 image_position.w=40;
}

void Texture::Clear(bool _delete)
{
 if(image!=NULL && _delete)
    {
     SDL_FreeSurface(image);
     image=NULL;
    }
 id=0;
 buff.Clear(_delete);
}

void Texture::Set_id(int _id)
{
 id=_id;
}

void Texture::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Texture::Set_type(int _type)
{
 type=_type;
}

int Texture::Get_id()
{
 return id;
}

int Texture::Get_type()
{
 return type;
}

bool Texture::Get_print_before_player()
{
 return print_before_player;
}

bool Texture::Is_obstacle()
{
 return obstacle;
}

bool Texture::Is_light()
{
 return light;
}

bool Texture::Is_animation()
{
 return animation;
}

Buff Texture::Get_Buff()
{
 return buff;
}

int Texture::Get_power()
{
 return power;
}

int Texture::Get_number_of_frames()
{
 return number_of_frames;
}

int Texture::Get_frame_change_delay()
{
 return frame_change_delay;
}

void Texture::Load()
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
 fscanf(where,"%d %d ",&power,&frame_change_delay);
 buff.Set_id(buff_id);
 buff.Load();
 strcpy(path,"maps/textures/images/");
 strcat(path,name);
 strcat(path,".bmp");
 image=make_it_transparent(path);
 ///
 fclose(where);
}

void Texture::Print_image(int x,int y,SDL_Surface *_screen,int frame)
{
 if(image==NULL)
    return;
 image_position.x=frame*40;
 image_position.y=0;
 image_position.h=40;
 image_position.w=40;
 apply_surface(image_position.x,image_position.y,x,y,image_position.h,image_position.w,image,_screen);
}
