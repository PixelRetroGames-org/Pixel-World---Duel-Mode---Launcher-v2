#ifndef LIBRARY_H
#define LIBRARY_H

///DEBUG
//#define DEBUG
///DEBUG

///AUDIO
#define AUDIO
///

//SDL
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>

extern const Uint8 *keystates;
extern const int PLAYER_INFO_POSX,PLAYER_INFO_LAST_POSX;
extern const int MAP_POSX,MAP_POSY,MAP_LAST_POSX,MAP_LAST_POSY;
extern const int PIXELS_PER_INGAME_UNIT;

///RENDERER
extern SDL_Window *WINDOW;
extern SDL_Renderer *RENDERER;
extern SDL_mutex *RENDERER_MUTEX;
extern SDL_Surface *SCREEN_SURFACE;

void Open_Window_and_Renderer(int RESOLUTION_W,int RESOLUTION_H,int DISPLAY_MODE);
void Close_Window_and_Renderer();
///

#define NUMBER_OF_MAX_KEYS 100
#define TEXT_LENGTH_MAX 101

void Copy_files(char *source,char *destination);

void atoi(int &n, char s[]);
void itoa(int n, char s[]);
void reverse(char s[]);
void Set_icon(char *filename,SDL_Window *_window);
void Make_Fullscreen(SDL_Window *_window,bool *fullscreen);
SDL_Surface *load_image(std::string filename);
void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination );
void apply_surface( int x, int y,int w,int h, SDL_Surface *source, SDL_Surface *destination );
void apply_surface(int xImage,int yImage,int xScreen,int yScreen,int w,int h,SDL_Surface *source,SDL_Surface *destination);
SDL_Surface *make_it_transparent( char *filename );

class Timer
{
 private:
 //The clock time when the timer started
 int startTicks;
 //The ticks stored when the timer was paused
 int pausedTicks;
 //The timer status
 bool paused;
 bool started;
 public:
 Timer();
 void start();
 void stop();
 void pause();
 void unpause();
 int get_ticks();
 bool is_started();
 bool is_paused();
};

#endif // LIBRARY_H
