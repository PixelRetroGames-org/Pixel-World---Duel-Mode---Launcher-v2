#include "texture.h"

Texture *last_frame;

Texture *Load_Texture(char *filename)
{
 SDL_Surface *image_surface=load_image(filename);
 if(image_surface==NULL)
    return NULL;
 SDL_LockMutex(RENDERER_MUTEX);
 SDL_Texture *image_texture=SDL_CreateTextureFromSurface(RENDERER,image_surface);
 if(image_texture==NULL)
    {
     FILE *where=fopen("err/logs.txt","a");
     fprintf(where,"%s while loading %s \n",SDL_GetError(),filename);
     fclose(where);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"SDL_CreateTextureFromSurface failed : ");
     strcat(message,SDL_GetError());
     strcat(message," while loading ");
     strcat(message,filename);
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL video module failure",message,NULL);
     exit(-1);
    }
 SDL_UnlockMutex(RENDERER_MUTEX);

 Texture *texture;
 texture=new Texture;
 texture->w=image_surface->w;
 texture->h=image_surface->h;
 texture->image=image_texture;

 SDL_FreeSurface(image_surface);
 return texture;
}

Texture *Load_Transparent_Texture(char *filename)
{
 SDL_Surface *image_surface=make_it_transparent(filename);
 if(image_surface==NULL)
    return NULL;
 SDL_LockMutex(RENDERER_MUTEX);
 SDL_Texture *image_texture=SDL_CreateTextureFromSurface(RENDERER,image_surface);
 if(image_texture==NULL)
    {
     FILE *where=fopen("err/logs.txt","a");
     fprintf(where,"%s while loading %s \n",SDL_GetError(),filename);
     fclose(where);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"SDL_CreateTextureFromSurface failed : ");
     strcat(message,SDL_GetError());
     strcat(message," while loading ");
     strcat(message,filename);
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL video module failure",message,NULL);
     exit(-1);
    }
 SDL_UnlockMutex(RENDERER_MUTEX);

 Texture *texture;
 texture=new Texture;
 texture->w=image_surface->w;
 texture->h=image_surface->h;
 texture->image=image_texture;

 SDL_FreeSurface(image_surface);
 return texture;
}

Texture *Create_TTF_Texture(TTF_Font *font,std::string text,SDL_Color color)
{
 SDL_Surface *image_surface=TTF_RenderText_Solid(font,text.c_str(),color);
 if(image_surface==NULL)
    return NULL;
 SDL_LockMutex(RENDERER_MUTEX);
 SDL_Texture *image_texture=SDL_CreateTextureFromSurface(RENDERER,image_surface);
 if(image_texture==NULL)
    {
     FILE *where=fopen("err/logs.txt","a");
     fprintf(where,"%s\n",SDL_GetError());
     fclose(where);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"SDL_CreateTextureFromSurface failed : ");
     strcat(message,SDL_GetError());
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL video module failure",message,NULL);
     exit(-1);
    }
 SDL_UnlockMutex(RENDERER_MUTEX);

 Texture *texture;
 texture=new Texture;
 texture->w=image_surface->w;
 texture->h=image_surface->h;
 texture->image=image_texture;

 SDL_FreeSurface(image_surface);
 return texture;
}

Texture *Create_Transparent_Texture(int w,int h)
{
 Texture *_texture=NULL;
 _texture=new Texture;
 SDL_LockMutex(RENDERER_MUTEX);
 _texture->image=SDL_CreateTexture(RENDERER,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);
 SDL_SetRenderTarget(RENDERER,_texture->image);
 SDL_SetRenderDrawBlendMode(RENDERER,SDL_BLENDMODE_BLEND);
 SDL_SetTextureBlendMode(_texture->image,SDL_BLENDMODE_BLEND);
 SDL_SetRenderDrawColor(RENDERER,0,0,0,0);
 SDL_RenderClear(RENDERER);
 SDL_SetRenderTarget(RENDERER,NULL);
 SDL_UnlockMutex(RENDERER_MUTEX);
 if(_texture->image==NULL)
    {
     FILE *where=fopen("err/logs.txt","a");
     fprintf(where,"%s\n",SDL_GetError());
     fclose(where);
     char message[TEXT_LENGTH_MAX];
     strcpy(message,"SDL_CreateTexture failed : ");
     strcat(message,SDL_GetError());
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL video module failure",message,NULL);
     exit(-1);
    }
 _texture->w=w;
 _texture->h=h;
 return _texture;
}

void Apply_Texture(int x,int y,Texture *source,Texture *destination)
{
 if(source==NULL || source->image==NULL)
    return;
 SDL_Rect *offset;
 offset=new SDL_Rect;
 offset->x=x;
 offset->y=y;
 offset->w=source->w;
 offset->h=source->h;

 SDL_LockMutex(RENDERER_MUTEX);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,destination->image);
    }
 SDL_RenderCopy(RENDERER,source->image,NULL,offset);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,NULL);
    }
 SDL_UnlockMutex(RENDERER_MUTEX);

 delete offset;
 if(destination==SCREEN)
    Apply_Texture(x,y,source,last_frame);
}

void Apply_Texture(int x,int y,int w,int h,Texture *source,Texture *destination)
{
 if(source==NULL || source->image==NULL)
    return;
 SDL_Rect *offset,*enlarge;
 offset=new SDL_Rect;
 enlarge=new SDL_Rect;
 offset->x=x;
 offset->y=y;
 offset->w=w;
 offset->h=h;
 enlarge->x=enlarge->y=0;
 enlarge->w=w;
 enlarge->h=h;

 SDL_LockMutex(RENDERER_MUTEX);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,destination->image);
    }
 SDL_RenderCopy(RENDERER,source->image,enlarge,offset);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,NULL);
    }
 SDL_UnlockMutex(RENDERER_MUTEX);

 delete offset;
 delete enlarge;
 if(destination==SCREEN)
    Apply_Texture(x,y,w,h,source,last_frame);
}

void Apply_Texture(int xImage,int yImage,int xScreen,int yScreen,int w,int h,Texture *source,Texture *destination)
{
 if(source==NULL || source->image==NULL)
    return;
 if(xImage<0)
    {
     xScreen-=xImage;
     xImage=0;
    }
 if(yImage<0)
    {
     yScreen-=yImage;
     yImage=0;
    }
 SDL_Rect *offset,*enlarge;
 offset=new SDL_Rect;
 enlarge=new SDL_Rect;
 offset->x=xScreen;
 offset->y=yScreen;
 offset->w=w;
 offset->h=h;
 enlarge->x=xImage;
 enlarge->y=yImage;
 enlarge->w=w;
 enlarge->h=h;

 if(destination==NULL)
    return;

 SDL_LockMutex(RENDERER_MUTEX);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,destination->image);
    }
 SDL_RenderCopy(RENDERER,source->image,enlarge,offset);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,NULL);
    }
 SDL_UnlockMutex(RENDERER_MUTEX);

 delete offset;
 delete enlarge;
 if(destination==SCREEN)
    Apply_Texture(xImage,yImage,xScreen,yScreen,w,h,source,last_frame);
}

void Apply_Stretched_Texture(int x,int y,int w,int h,Texture *source,Texture *destination)
{
 if(source==NULL || source->image==NULL)
    return;
 SDL_Rect *offset,*enlarge;
 offset=new SDL_Rect;
 enlarge=new SDL_Rect;
 offset->x=x;
 offset->y=y;
 offset->w=w;
 offset->h=h;
 enlarge->x=enlarge->y=0;
 enlarge->w=w;
 enlarge->h=h;

 SDL_LockMutex(RENDERER_MUTEX);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,destination->image);
    }
 SDL_RenderCopy(RENDERER,source->image,enlarge,offset);
 if(destination->image!=NULL)
    {
     SDL_SetRenderTarget(RENDERER,NULL);
    }
 SDL_UnlockMutex(RENDERER_MUTEX);

 delete offset;
 delete enlarge;
 if(destination==SCREEN)
    Apply_Texture(x,y,w,h,source,last_frame);
}

void Set_Texture_Blend_Mode(Texture *_texture,SDL_BlendMode blend_mode)
{
 if(_texture==NULL || _texture->image==NULL)
    return;
 SDL_LockMutex(RENDERER_MUTEX);
 SDL_SetTextureBlendMode(_texture->image,blend_mode);
 SDL_UnlockMutex(RENDERER_MUTEX);
}

void Set_Texture_Alpha(Texture *_texture,Uint8 alpha)
{
 if(_texture==NULL || _texture->image==NULL)
    return;
 SDL_BlendMode blendmode;
 SDL_GetTextureBlendMode(_texture->image,&blendmode);
 if(blendmode!=SDL_BLENDMODE_BLEND)
    Set_Texture_Blend_Mode(_texture,SDL_BLENDMODE_BLEND);
 SDL_LockMutex(RENDERER_MUTEX);
 SDL_SetTextureAlphaMod(_texture->image,alpha);
 SDL_UnlockMutex(RENDERER_MUTEX);
}

void Destroy_Texture(Texture *_texture)
{
 if(_texture==NULL)
    return;
 if(_texture->image!=NULL)
    {
     SDL_LockMutex(RENDERER_MUTEX);
     SDL_DestroyTexture(_texture->image);
     SDL_UnlockMutex(RENDERER_MUTEX);
     _texture->image=NULL;
    }
 delete _texture;
 _texture=NULL;
}

Texture *SCREEN;

void Flip_Buffers(Texture *screen)
{
 if(screen!=SCREEN)
    return;
 SDL_LockMutex(RENDERER_MUTEX);
 SDL_RenderPresent(RENDERER);
 SDL_UnlockMutex(RENDERER_MUTEX);
}
