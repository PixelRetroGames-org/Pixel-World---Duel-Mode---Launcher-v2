#ifndef JOURNAL_H
#define JOURNAL_H

#include "library.h"
#include "journal_entry.h"
#include "settings.h"
#include "load_audio_effects.h"
#include "load_images.h"
#include <vector>

const int NUMBER_OF_MAX_JOURNAL_ENTRIES=10;

class Journal
{
 public:
 int number_of_entries=0,current_entry=0;
 char journal_entries_names[NUMBER_OF_MAX_JOURNAL_ENTRIES][TEXT_LENGTH_MAX];
 Journal_Entry journal_entries[NUMBER_OF_MAX_JOURNAL_ENTRIES];
 char name[TEXT_LENGTH_MAX];
 SDL_Surface *name_image=NULL;

 private:
 void Clear();
 void Load();
 void Load(char *_name);
 void Set_name(char *_name);
 void Handle_Events(SDL_Event *event);
 void Print(SDL_Surface *_screen);
 void Start(SDL_Surface *_screen);
 void Start(char *_name,SDL_Surface *_screen);
};

#endif // JOURNAL_H
