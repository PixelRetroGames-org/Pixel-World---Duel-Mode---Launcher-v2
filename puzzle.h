#ifndef PUZZLE_H
#define PUZZLE_H

#include "library.h"

class Puzzle
{
 private:
 int id=0,type=0;
 char title[TEXT_LENGHT_MAX],text[TEXT_LENGHT_MAX],text_typed[TEXT_LENGHT_MAX],answer[TEXT_LENGHT_MAX];
 SDL_Surface *title_image=NULL,*text_image=NULL,*text_typed_image=NULL,answer_image=NULL;
 SDL_Surface *background_image=NULL;

 public:
 Puzzle();
 ~Puzzle();
 void Clear(bool _delete=false);
 void Set_id(int _id);
 int Get_id();
 void Load();
 bool Start(SDL_Surface *_screen);
 void Print(SDL_Surface *_screen);
 void Handle_Events(SDL_Event);
};

#endif // PUZZLE_H
