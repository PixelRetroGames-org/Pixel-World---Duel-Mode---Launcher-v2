#ifndef LIBRARY_H
#define LIBRARY_H

///DEBUG
//#define DEBUG
///DEBUG

///AUDIO
#define AUDIO
///

//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
//CSTRING
#include<cstring>
//STRING
#include<string>

extern Uint8 *keystates;
extern const int PLAYER_INFO_POSX,PLAYER_INFO_LAST_POSX;
extern const int MAP_POSX,MAP_POSY,MAP_LAST_POSX,MAP_LAST_POSY;

#define NUMBER_OF_MAX_KEYS 20
#define TEXT_LENGTH_MAX 101

void atoi(int &n, char s[]);
void itoa(int n, char s[]);
void reverse(char s[]);
void Set_icon(char *filename);
void Make_Fullscreen(SDL_Surface *_screen,bool *fullscreen);
SDL_Surface *load_image(std::string filename);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void apply_surface( int x, int y,int w,int h, SDL_Surface* source, SDL_Surface* destination );
void apply_surface(int xImage,int yImage,int xScreen,int yScreen,int w,int h,SDL_Surface *source,SDL_Surface *destination);
SDL_Surface *make_it_transparent( char *filename );
void make_it_transparent(SDL_Surface *image);
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
