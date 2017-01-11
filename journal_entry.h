#ifndef JOURNAL_ENTRY_H
#define JOURNAL_ENTRY_H

#include "library.h"
#include "script_interpreter.h"
#include <bitset>

const int NUMBER_OF_PAGES_MAX=5;

extern const int PAGE_POSX,PAGE_POSY;

class Journal_Entry
{
 private:
 char title[TEXT_LENGTH_MAX],title_image_name[TEXT_LENGTH_MAX];
 SDL_Surface *title_image=NULL;
 int beginning_key=0,ending_key=0;
 int number_of_pages=0,current_page=0;
 char pages_script_names[NUMBER_OF_PAGES_MAX][TEXT_LENGTH_MAX];
 SDL_Surface *pages_images[NUMBER_OF_PAGES_MAX];

 public:
 void Clear();
 void Load();
 void Load(char *_title);
 void Set_title(char *_title);
 bool Handle_Events(SDL_Event *event);
 void Print_Title(int x,int y,SDL_Surface *_screen,bool click=false,bool hover=false);
 void Print_Page(SDL_Surface *_screen);
 bool Is_started(std::bitset<NUMBER_OF_MAX_KEYS> *progress);
 bool Is_finished(std::bitset<NUMBER_OF_MAX_KEYS> *progress);
 bool Is_in_progress(std::bitset<NUMBER_OF_MAX_KEYS> *progress);
};

#endif // JOURNAL_ENTRY_H
