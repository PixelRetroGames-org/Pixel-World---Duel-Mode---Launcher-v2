#include "library.h"
//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

Uint8 *keystates=SDL_GetKeyState(NULL);

const int PLAYER_INFO_POSX=760,PLAYER_INFO_LAST_POSX=1130;

const int MAP_POSX=0,MAP_POSY=0,MAP_LAST_POSX=760,MAP_LAST_POSY=1130;

void Set_icon(char *filename)
{
 Uint32 colorkey;
 SDL_Surface *_icon=NULL;
 _icon=SDL_LoadBMP(filename);
 colorkey=SDL_MapRGB(_icon->format,255,0,255);
 SDL_SetColorKey(_icon, SDL_SRCCOLORKEY, colorkey);
 SDL_WM_SetIcon(_icon,NULL);
}

void Make_Fullscreen(SDL_Surface *_screen,bool *fullscreen)
{
 if(fullscreen)
    _screen=SDL_SetVideoMode(0,0,32,SDL_SWSURFACE);
 else
    _screen=SDL_SetVideoMode(0,0,32,SDL_FULLSCREEN);
 bool x=*fullscreen;
 x=!(x);
 (*fullscreen)=x;
}

SDL_Surface *load_image(std::string filename)
{
 SDL_Surface *loadedImage=NULL;
 SDL_Surface *optimizedImage=NULL;
 loadedImage=SDL_LoadBMP(filename.c_str());
 if(loadedImage!=NULL)
    {
     optimizedImage=SDL_DisplayFormat(loadedImage);
     SDL_FreeSurface(loadedImage);
    }
 return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
 SDL_Rect *offset;
 offset=new SDL_Rect;
 offset->x=x;
 offset->y=y;
 SDL_BlitSurface(source,NULL,destination,offset);
 delete offset;
 #ifdef DEBUG
 SDL_Flip(destination);
 #endif // DEBUG
}

void apply_surface( int x, int y,int w,int h, SDL_Surface* source, SDL_Surface* destination )
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
 #ifdef DEBUG
 SDL_Flip(destination);
 #endif // DEBUG
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
 #ifdef DEBUG
 SDL_Flip(destination);
 #endif // DEBUG
}

SDL_Surface *make_it_transparent( char *filename )
{
 //The image that's loaded
 SDL_Surface *loadedImage=NULL;
 //The optimized image that will be used
 SDL_Surface *optimizedImage=NULL;
 //Load the image
 loadedImage=SDL_LoadBMP(filename);
 //If the image loaded
 if(loadedImage!=NULL)
    {
	//Create an optimized image
	optimizedImage=SDL_DisplayFormat(loadedImage);
	//Free the old image
	SDL_FreeSurface(loadedImage);
	//If the image was optimized just fine
	if(optimizedImage!=NULL)
        {
	    //Map the color key
	    Uint32 colorkey=SDL_MapRGB(optimizedImage->format,0xFF,0x0,0xE1);
	    //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
	    SDL_SetColorKey(optimizedImage,SDL_SRCCOLORKEY,colorkey);
        }
    }
 //Return the optimized image
 return optimizedImage;
}

void make_it_transparent( SDL_Surface *image )
{
 if(image!=NULL)
    {
     //Map the color key
     Uint32 colorkey=SDL_MapRGB(image->format,0xFF,0x0,0xE1);
     SDL_SetColorKey(image,SDL_SRCCOLORKEY,colorkey);
    }
}

Timer::Timer()
{
 //Initialize the variables
 startTicks = 0;
 pausedTicks = 0;
 paused = false;
 started = false;
}
void Timer::start()
{
 //Start the timer
 started = true;
 //Unpause the timer
 paused = false;
 //Get the current clock time
 startTicks = SDL_GetTicks();
}
void Timer::stop()
{
 //Stop the timer
 started = false;
 //Unpause the timer
 paused = false;
}
void Timer::pause()
{
 //If the timer is running and isn't already paused
 if((started==true) && (paused==false))
    {
     //Pause the timer
     paused = true;
     //Calculate the paused ticks
     pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void Timer::unpause()
{
 //If the timer is paused
 if(paused==true)
    {
     //Unpause the timer
     paused=false;
     //Reset the starting ticks
     startTicks = SDL_GetTicks() - pausedTicks;
     //Reset the paused ticks
     pausedTicks = 0;
    }
}
int Timer::get_ticks()
{
 //If the timer is running
 if(started==true)
    {
     //If the timer is paused
     if(paused==true)
        {
         //Return the number of ticks when the timer was paused
         return pausedTicks;
        }
     else
        {
         //Return the current time minus the start time
         return SDL_GetTicks() - startTicks;
        }
    }
 //If the timer isn't running
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
 int i, sign;
 if ((sign = n) < 0)  /* record sign */
     n = -n;          /* make n positive */
 i = 0;
 do {       /* generate digits in reverse order */
     s[i++] = n % 10 + '0';   /* get next digit */
 } while ((n /= 10) > 0);     /* delete it */
 if (sign < 0)
     s[i++] = '-';
 s[i] = '\0';
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
 int i, j;
 char c;

 for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
     c = s[i];
     s[i] = s[j];
     s[j] = c;
 }
}
