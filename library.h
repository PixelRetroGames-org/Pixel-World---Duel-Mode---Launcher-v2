#ifndef LIBRARY_H
#define LIBRARY_H

//SDL
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );

#endif // LIBRARY_H
