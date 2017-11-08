#ifndef JOURNAL_ENTRY_H
#define JOURNAL_ENTRY_H

#include "library.h"
#include "texture.h"
#include "script_interpreter.h"

#include <bitset>

const int NUMBER_OF_PAGES_MAX=5;

extern const int PAGE_POSX,PAGE_POSY;

class Journal_Entry
{
 private:
 char title[TEXT_LENGTH_MAX],title_image_name[TEXT_LENGTH_MAX];
 Texture *title_image=NULL;
 int beginning_key=0,ending_key=0;
 int number_of_pages=0,current_page=0;
 char pages_script_names[NUMBER_OF_PAGES_MAX][TEXT_LENGTH_MAX];
 Texture *pages_images[NUMBER_OF_PAGES_MAX];

 public:
 void Clear();
 void Load();
 void Load(char *_title);
 void Set_title(char *_title);
 int Get_title_sizeW();
 int Get_title_sizeH();
 bool Handle_Events(SDL_Event *event);
 void Print_Title(int x,int y,Texture *_screen,bool click=false,bool hover=false);
 void Print_Page(Texture *_screen);
 bool Is_started(std::bitset<NUMBER_OF_MAX_KEYS> *progress);
 bool Is_finished(std::bitset<NUMBER_OF_MAX_KEYS> *progress);
 bool Is_in_progress(std::bitset<NUMBER_OF_MAX_KEYS> *progress);
};

#endif // JOURNAL_ENTRY_H
