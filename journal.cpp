#include "journal.h"

void Journal::Clear()
{
 for(int i=0;i<number_of_entries;i++)
     {
      journal_entries[i].Clear();
      journal_entries_names[i][0]=NULL;
     }
 number_of_entries=current_entry=0;
 name[0]=NULL;
 SDL_FreeSurface(name_image);
 name_image=NULL;
}

void Journal::Load()
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"journal/");
 strcat(path,name);
 strcat(path,".pwj");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d ",&number_of_entries);
 for(int i=0;i<number_of_entries;i++)
     {
      fgets(journal_entries_names[i],sizeof journal_entries_names[i],where);
      if(journal_entries_names[i][strlen(journal_entries_names[i])-1]=='\n')
         journal_entries_names[i][strlen(journal_entries_names[i])-1]=NULL;
      journal_entries[i].Load(journal_entries_names[i]);
     }
 fclose(where);
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",20);
 SDL_Color color={0,0,0};
 name_image=TTF_RenderText_Solid(font,name,color);
 TTF_CloseFont(font);
}

void Journal::Load(char *_name)
{
 Set_name(_name);
 Load();
}

void Journal::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Journal::Handle_Events(SDL_Event *event)
{
 if(event->type==SDL_KEYDOWN)
    {
     if(event->key.keysym.sym==SDLK_UP || event->key.keysym.sym==SDLK_w)
        {
         current_entry--;
         if(current_entry<0)
            current_entry=0;
         return;
        }
     if(event->key.keysym.sym==SDLK_DOWN || event->key.keysym.sym==SDLK_s)
        {
         current_entry++;
         if(current_entry>=number_of_entries)
            current_entry=number_of_entries;
         return;
        }
    }
}

void Journal::Print(SDL_Surface *_screen)
{
 apply_surface(0,0,LEVEL_background_image,_screen);
 apply_surface((RESOLUTION_X-name_image->w)/2,5,name_image,_screen);
 int X=0,Y=6;
 for(int i=0;i<number_of_entries;i++)
     {
      journal_entries[i].Print_Title(X,Y,_screen,i==current_entry,false);
      if(i==current_entry)
         journal_entries[i].Print_Page(100,6,_screen);
     }
}

void Journal::Start(SDL_Surface *_screen)
{
 SDL_Event event;
 SDL_PollEvent(&event);
 bool quit=false;
 while(!quit)
       {
        if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE)
           quit=true;
        Handle_Events(&event);
        Print(_screen);
       }
}

void Journal::Start(char *_name,SDL_Surface *_screen)
{
 Load(_name);
 Start(_screen);
}
