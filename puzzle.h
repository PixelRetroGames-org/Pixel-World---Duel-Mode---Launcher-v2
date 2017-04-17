#ifndef PUZZLE_H
#define PUZZLE_H

#include "library.h"
#include "settings.h"
#include "script_interpreter.h"

#include <bitset>

const int NUMBER_OF_ANSWERS_MAX=10;

class Puzzle
{
 private:
 int type=0,number_of_answers=0;
 char name[TEXT_LENGTH_MAX];
 char title[TEXT_LENGTH_MAX],text[TEXT_LENGTH_MAX],text_typed[TEXT_LENGTH_MAX],answers[NUMBER_OF_ANSWERS_MAX][TEXT_LENGTH_MAX];
 SDL_Surface* title_image=NULL,*text_image=NULL,*text_typed_image=NULL,*answer_image=NULL;
 SDL_Surface* background_image=NULL;

 public:
 Puzzle();
 ~Puzzle();
 void Clear(bool _delete=false);
 void Set_name(char* _name);
 void Load();
 bool Start(SDL_Surface* _screen);
 bool Validate_Riddle();
 void Print_Verdict(bool verdict,SDL_Surface* _screen);
 bool Start_Riddle(SDL_Surface* _screen);
};

int Get_Key(char ch);

#endif // PUZZLE_H
