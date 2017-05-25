#include "Texture.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
    const int amask = 0x000000ff;
#else
    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
    const int amask = 0xff000000;

#endif

Texture SCREEN_TEXTURE;

Texture* Load_Texture(char* name)
{
 Texture* _texture=new Texture;
 SDL_Surface* first=load_image(name);
 return Load_Texture(first);
}

Texture* Load_Texture(SDL_Surface* first)
{
 Texture* _texture=new Texture;
 SDL_Surface* img=SDL_CreateRGBSurface(0, first->w, first->h, 32, bmask, gmask, rmask, amask);
 SDL_BlitSurface(first,0,img,0);
 SDL_FreeSurface(first);
 glGenTextures(1,&_texture->id);
 glBindTexture(GL_TEXTURE_2D,_texture->id);
 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
 int colors=img->format->BytesPerPixel,texture_format;
 if(colors==4)
    {
     if(img->format->Rmask==amask)
        texture_format=GL_RGBA;
     else
        texture_format=GL_BGRA;
    }
 else
    {
     if(img->format->Rmask==amask)
        texture_format=GL_RGB;
     else
        texture_format=GL_BGR;
    }
 glTexImage2D(GL_TEXTURE_2D,0,colors,img->w,img->h,0,texture_format,GL_UNSIGNED_BYTE,img->pixels);
 _texture->w=img->w;
 _texture->h=img->h;
 SDL_FreeSurface(img);
 return _texture;
}

Texture* Load_transparent_Texture(char* name)
{
 SDL_Surface *img=make_it_transparent(name);
 return Load_Texture(img);
}

Texture* Load_TTF_Texture(TTF_Font *font,char *text,SDL_Color color)
{
 SDL_Surface *surface=TTF_RenderText_Solid(font,text,color);
 surface=optimize_image(surface);
 Texture *texture=Load_Texture(surface);
 return texture;
}

void Apply_Texture(float x,float y,Texture* source)
{
 glBindTexture(GL_TEXTURE_2D,source->id);
 glBegin(GL_QUADS);
 glTexCoord2f(0,0); glVertex2f(x,y); //Upper-left corner
 glTexCoord2f(1,0); glVertex2f(x+source->w,y); //Upper-right corner
 glTexCoord2f(1,1); glVertex2f(x+source->w,y+source->h); //Down-right corner
 glTexCoord2f(0,1); glVertex2f(x,y+source->h); //Down-left corner
 glEnd(); //End drawing
}

void Apply_Texture(float x,float y,float w,float h,Texture* source)
{
 glBindTexture(GL_TEXTURE_2D,source->id);
 glBegin(GL_QUADS);
 glTexCoord2d(0,0); glVertex2f(x,y); //Upper-left corner
 glTexCoord2d(w/source->w,0); glVertex2f(x+w,y); //Upper-right corner
 glTexCoord2d(w/source->w,h/source->h); glVertex2f(x+w,y+h); //Down-right corner
 glTexCoord2d(0,h/source->h); glVertex2f(x,y+h); //Down-left corner
 glEnd(); //End drawing
}

void Apply_Texture(float xImage,float yImage,float xScreen,float yScreen,float w,float h,Texture* source)
{
 glBindTexture(GL_TEXTURE_2D,source->id);
 glBegin(GL_QUADS);
 glTexCoord2d(xImage/source->w,yImage/source->h); glVertex2f(xScreen,yScreen); //Upper-left corner
 glTexCoord2d(xImage/source->w+w/source->w,yImage/source->h); glVertex2f(xScreen+w,yScreen); //Upper-right corner
 glTexCoord2d(xImage/source->w+w/source->w,yImage/source->h+h/source->h); glVertex2f(xScreen+w,yScreen+h); //Down-right corner
 glTexCoord2d(xImage/source->w,yImage/source->h+h/source->h); glVertex2f(xScreen,yScreen+h); //Down-left corner
 glEnd(); //End drawing
}

void Free_Texture(Texture* _texture)
{
 glDeleteTextures(1,&_texture->id);
 delete _texture;
}
