#include "library.h"

const Uint8 *keystates=SDL_GetKeyboardState(NULL);

const int PLAYER_INFO_POSX=760,PLAYER_INFO_LAST_POSX=1130;
const int MAP_POSX=0,MAP_POSY=0,MAP_LAST_POSX=760,MAP_LAST_POSY=1130;
const int PIXELS_PER_INGAME_UNIT=40;

///RENDERER
SDL_Window *WINDOW;
SDL_Renderer *RENDERER;
SDL_mutex *RENDERER_MUTEX;
SDL_Surface *SCREEN_SURFACE;

void Open_Window_and_Renderer(int RESOLUTION_W,int RESOLUTION_H,int DISPLAY_MODE)
{
 WINDOW=SDL_CreateWindow("Pixel World",0,0,RESOLUTION_W,RESOLUTION_H,DISPLAY_MODE);
 SDL_SetWindowPosition(WINDOW,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
 Set_icon("images/icon.png",WINDOW);
 RENDERER=SDL_CreateRenderer(WINDOW,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
 RENDERER_MUTEX=SDL_CreateMutex();
 SCREEN_SURFACE=SDL_GetWindowSurface(WINDOW);
}

void Close_Window_and_Renderer()
{
 SDL_DestroyWindow(WINDOW);
 WINDOW=NULL;
 SDL_DestroyRenderer(RENDERER);
 RENDERER=NULL;
 SDL_DestroyMutex(RENDERER_MUTEX);
 RENDERER_MUTEX=NULL;
}
///

void Set_icon(char *filename,SDL_Window *_window)
{
 Uint32 colorkey;
 SDL_Surface *_icon=NULL;
 _icon=IMG_Load(filename);
 if(_icon==NULL)
    return;
 colorkey=SDL_MapRGB(_icon->format,255,0,255);
 SDL_SetColorKey(_icon, SDL_TRUE, colorkey);
 SDL_SetWindowIcon(_window,_icon);
}

void Make_Fullscreen(SDL_Window *_window,bool *fullscreen)
{
 if(fullscreen)
    {
     int a=0;
     SDL_SetWindowFullscreen(_window,SDL_WINDOW_FULLSCREEN);
    }
 else
    {
     int b=0;
     SDL_SetWindowFullscreen(_window,0);
    }
 bool x=*fullscreen;
 x=!(x);
 (*fullscreen)=x;
}

SDL_Surface *load_image(std::string filename)
{
 SDL_Surface *loadedImage=NULL;
 loadedImage=IMG_Load(filename.c_str());
 if(loadedImage==NULL)
    return NULL;
 SDL_Surface *optimizedImage=SDL_ConvertSurface(loadedImage,SCREEN_SURFACE->format,NULL);
 SDL_FreeSurface(loadedImage);
 return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination )
{
 SDL_Rect *offset;
 offset=new SDL_Rect;
 offset->x=x;
 offset->y=y;
 SDL_BlitSurface(source,NULL,destination,offset);
 delete offset;
}

void apply_surface( int x, int y,int w,int h, SDL_Surface *source, SDL_Surface *destination )
{
 SDL_Rect *offset,*enlarge;
 offset=new SDL_Rect;
 enlarge=new SDL_Rect;
 offset->x=x;
 offset->y=y;
 enlarge->x=enlarge->y=0;
 enlarge->w=w;
 enlarge->h=h;
 SDL_BlitSurface(source,enlarge,destination,offset);
 delete offset;
 delete enlarge;
}

void apply_surface(int xImage,int yImage,int xScreen,int yScreen,int w,int h,SDL_Surface *source,SDL_Surface *destination)
{
 SDL_Rect *offset,*enlarge;
 offset=new SDL_Rect;
 enlarge=new SDL_Rect;
 offset->x=xScreen;
 offset->y=yScreen;
 enlarge->x=xImage;
 enlarge->y=yImage;
 enlarge->w=w;
 enlarge->h=h;
 int rtn=SDL_BlitSurface(source,enlarge,destination,offset);
 delete offset;
 delete enlarge;
}

SDL_Surface *make_it_transparent( char *filename )
{
 SDL_Surface *loadedImage=NULL;
 loadedImage=IMG_Load(filename);
 if(loadedImage!=NULL)
    {
     Uint32 colorkey=SDL_MapRGB(loadedImage->format,0xFF,0x0,0xE1);
     Uint8 r,g,b;
     SDL_GetRGB(colorkey,loadedImage->format,&r,&g,&b);
     if(r==0xFF && g==0x0 && b==0xE1)
        SDL_SetColorKey(loadedImage, SDL_TRUE, colorkey);
    }
 return loadedImage;
}

Timer::Timer()
{
 startTicks = 0;
 pausedTicks = 0;
 paused = false;
 started = false;
}
void Timer::start()
{
 started = true;
 paused = false;
 startTicks = SDL_GetTicks();
}
void Timer::stop()
{
 started = false;
 paused = false;
}
void Timer::pause()
{
 if((started==true) && (paused==false))
    {
     paused = true;
     pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void Timer::unpause()
{
 if(paused==true)
    {
     paused=false;
     startTicks = SDL_GetTicks() - pausedTicks;
     pausedTicks = 0;
    }
}
int Timer::get_ticks()
{
 if(started==true)
    {
     if(paused==true)
        {
         return pausedTicks;
        }
     else
        {
         return SDL_GetTicks() - startTicks;
        }
    }
 return 0;
}
bool Timer::is_started()
{
 return started;
}
bool Timer::is_paused()
{
 return paused;
}

void itoa(int n, char s[])
{
 int i,sign;
 if((sign=n)<0)
     n=-n;
 i=0;
 do
   {
    s[i++]=n%10+'0';
   }
 while((n/=10)>0);
 if(sign<0)
    s[i++]='-';
 s[i]='\0';
 reverse(s);
}

void atoi(int &n,char s[])
{
 n=0;
 int x=strlen(s);
 for(int i=0;i<x;i++)
     {
      n=n*10;
      n+=(int)(s[i]-'0');
     }
}

void reverse(char s[])
{
 int i,j;
 char c;
 for(i=0,j=strlen(s)-1;i<j;i++,j--)
     {
      c=s[i];
      s[i]=s[j];
      s[j]=c;
     }
}

void Copy_files(char *source,char *destination)
{
 FILE *source_file,*destination_file;
 source_file=fopen(source,"r");
 destination_file=fopen(destination,"w");
 char buffer[TEXT_LENGTH_MAX];
 while(!feof(source_file))
       {
        fgets(buffer,sizeof buffer,source_file);
        fputs(buffer,destination_file);
       }
 fclose(source_file);
 fclose(destination_file);
}
