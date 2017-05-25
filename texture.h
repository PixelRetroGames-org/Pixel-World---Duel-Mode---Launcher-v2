#ifndef TEXTURE_H
#define TEXTURE_H

#include "library.h"

class Texture
{
 public:
 int w=0,h=0;
 GLuint id=NULL;
};

extern Texture SCREEN_TEXTURE;

Texture* Load_Texture(char* name);
Texture* Load_Texture(SDL_Surface* first);
Texture* Load_transparent_Texture(char* name);
Texture* Load_TTF_Texture(TTF_Font *font,char *text,SDL_Color color);

void Apply_Texture(float x,float y,Texture* source,Texture* destination);
void Apply_Texture(float x,float y,float w,float h,Texture* source,Texture* destination);
void Apply_Texture(float xImage,float yImage,float xScreen,float yScreen,float w,float h,Texture* source,Texture* destination);

void Free_Texture(Texture* _texture);

#endif //TEXTURE_H
