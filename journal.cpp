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
 redraw=true;
}

void Journal::Load()
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"journal/");
 strcat(path,name);
 strcat(path,".pwj");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    return;
 fscanf(where,"%d ",&number_of_entries);
 for(int i=0;i<number_of_entries;i++)
     {
      fgets(journal_entries_names[i],sizeof journal_entries_names[i],where);
      if(journal_entries_names[i][strlen(journal_entries_names[i])-1]=='\n')
         journal_entries_names[i][strlen(journal_entries_names[i])-1]=NULL;
      journal_entries[i].Load(journal_entries_names[i]);
     }
 fclose(where);
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",40);
 SDL_Color color={52,124,191};
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
 redraw=false;
 if(event->type==SDL_KEYDOWN)
    {
     if(event->key.keysym.sym==SDLK_UP || event->key.keysym.sym==SDLK_w)
        {
         current_entry--;
         if(current_entry<0)
            current_entry=0;
         redraw=true;
         return;
        }
     if(event->key.keysym.sym==SDLK_DOWN || event->key.keysym.sym==SDLK_s)
        {
         current_entry++;
         if(current_entry>=number_of_entries)
            current_entry=number_of_entries-1;
         redraw=true;
         return;
        }
    }
 redraw=journal_entries[current_entry].Handle_Events(event);
}

void Journal::Print(SDL_Surface *_screen)
{
 apply_surface(0,0,LEVEL_background_image,_screen);
 journal_entries[current_entry].Print_Page(_screen);
 apply_surface((RESOLUTION_X-name_image->w)/2,5,name_image,_screen);
 int X=20,Y=(RESOLUTION_Y-50*number_of_entries)/2;
 for(int i=0;i<number_of_entries;i++)
     {
      if(!journal_entries[i].Is_in_progress(progress))
         continue;
      journal_entries[i].Print_Title(X,Y,_screen,i==current_entry,false);
      Y+=50;
     }
}

const int FRAMES_PER_SECOND=27;

void Journal::Start(SDL_Surface *_screen)
{
 SDL_Event event;
 SDL_Delay(100);
 while(SDL_PollEvent(&event));
 bool quit=false;
 Print(_screen);
 SDL_Flip(_screen);
 Timer fps;
 while(!quit)
       {
        fps.start();
        if(SDL_PollEvent(&event) && !quit)
           {
            if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE)
               quit=true;
            Handle_Events(&event);
            if(redraw)
               {
                Print(_screen);
                SDL_Flip(_screen);
               }
           }
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND)
           {
            SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
           }
       }
 SDL_Delay(100);
 while(SDL_PollEvent(&event));
}

void Journal::Start(char *_name,std::bitset<NUMBER_OF_MAX_KEYS> *_progress,SDL_Surface *_screen)
{
 SDL_Thread *_loading_image=NULL;
 _loading_image=SDL_CreateThread(Loading_image,NULL);
 Load(_name);
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 int thread_return_value=0;
 SDL_WaitThread(_loading_image,&thread_return_value);
 SDL_Flip(static_screen);
 progress=_progress;
 Start(_screen);
 _loading_image=SDL_CreateThread(Loading_image,NULL);
 Clear();
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 thread_return_value=0;
 SDL_WaitThread(_loading_image,&thread_return_value);
 SDL_Flip(static_screen);
}

void Journal::Start(std::bitset<NUMBER_OF_MAX_KEYS> *_progress,SDL_Surface *_screen)
{
 progress=_progress;
 Start(_screen);
}

Journal journal;

void Load_Journal()
{
 journal.Load("Journal");
}
