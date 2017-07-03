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
 Destroy_Texture(name_image);
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
 name_image=Create_TTF_Texture(font,name,color);
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

Timer controller_timer;
const int CONTROLLER_DELAY=100;

void Journal::Handle_Events(SDL_Event *event)
{
 redraw=false;
 if(event->type==SDL_KEYDOWN || controller[1].Pressed_Any_Button() || controller[2].Pressed_Any_Button())
    {
     if((event->key.keysym.scancode==SDL_SCANCODE_UP || event->key.keysym.scancode==SDL_SCANCODE_W) || (controller_timer.get_ticks()>CONTROLLER_DELAY && (controller[1].Pressed_Up() || controller[2].Pressed_Up())))
        {
         controller_timer.start();
         if(current_entry-1>=0)
            {
             current_entry--;
             if(current_entry<0)
                current_entry=0;
             while(!journal_entries[current_entry].Is_in_progress(progress) && current_entry>0)
                   current_entry--;
             redraw=true;
            }
        }
     if((event->key.keysym.scancode==SDL_SCANCODE_DOWN || event->key.keysym.scancode==SDL_SCANCODE_S) || (controller_timer.get_ticks()>CONTROLLER_DELAY && (controller[1].Pressed_Down() || controller[2].Pressed_Down())))
        {
         if(current_entry+1<=number_of_entries)
            {
             controller_timer.start();
             int cpy=current_entry;
             current_entry++;
             if(current_entry>=number_of_entries)
                current_entry=number_of_entries-1;
             while(!journal_entries[current_entry].Is_in_progress(progress) && current_entry<number_of_entries-1)
                   current_entry++;
             if(!journal_entries[current_entry].Is_in_progress(progress))
                current_entry=cpy;
             redraw=true;
            }
        }
    }
 if(event->type==SDL_MOUSEMOTION || event->type==SDL_MOUSEBUTTONDOWN)
    {
     int _x=20,_y=(RESOLUTION_H-50*number_of_entries)/2,mouse_x=0,mouse_y=0;
     mouse_x=event->button.x,mouse_y=event->button.y;
     hover_entry=-1;
     for(int i=0;i<number_of_entries;i++)
         {
          if(!journal_entries[i].Is_in_progress(progress))
             continue;
          if(mouse_x>=_x && mouse_x<=_x+journal_entries[i].Get_title_sizeW() && mouse_y>=_y && mouse_y<=_y+journal_entries[i].Get_title_sizeH())
             hover_entry=i;
          _y+=journal_entries[i].Get_title_sizeH();
         }
     redraw=true;
    }
 if(event->type==SDL_MOUSEBUTTONDOWN && hover_entry!=-1)
    {
     current_entry=hover_entry;
     redraw=true;
    }
 if(redraw)
    return;
 redraw=journal_entries[current_entry].Handle_Events(event);
}

void Journal::Print(Texture *_screen)
{
 Apply_Texture(0,0,LEVEL_background_image,_screen);
 if(current_entry!=-1)
    journal_entries[current_entry].Print_Page(_screen);
 Apply_Texture((RESOLUTION_W-name_image->w)/2,5,name_image,_screen);
 int X=20,Y=(RESOLUTION_H-50*number_of_entries)/2;
 for(int i=0;i<number_of_entries;i++)
     {
      if(!journal_entries[i].Is_in_progress(progress))
         continue;
      journal_entries[i].Print_Title(X,Y,_screen,i==current_entry,i==hover_entry);
      Y+=journal_entries[i].Get_title_sizeH();
     }
}

const int FRAMES_PER_SECOND=27;

void Journal::Start(Texture *_screen)
{
 SDL_Event event;
 SDL_Delay(100);
 while(SDL_PollEvent(&event));
 bool quit=false;
 Print(_screen);
 Flip_Buffers(_screen);
 Timer fps;
 controller_timer.start();
 while(!quit)
       {
        fps.start();
        if(SDL_PollEvent(&event) && !quit)
           {
            Update_Controllers_Events();
            if((event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE) || (event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_J) ||
               (controller[1].Pressed_Guide_button() || controller[1].Pressed_Start_button() || controller[2].Pressed_Guide_button() || controller[2].Pressed_Start_button()))
               quit=true;
            Handle_Events(&event);
            if(redraw)
               {
                Print(_screen);
                Flip_Buffers(_screen);
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

void Journal::Start(char *_name,std::bitset<NUMBER_OF_MAX_KEYS> *_progress,Texture *_screen)
{
 SDL_Thread *_loading_image=NULL;
 _loading_image=SDL_CreateThread(Loading_image,"Journal Loading",NULL);
 Load(_name);
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 int thread_return_value=0;
 SDL_WaitThread(_loading_image,&thread_return_value);
 Flip_Buffers(_screen);
 progress=_progress;
 Start(_screen);
 _loading_image=SDL_CreateThread(Loading_image,"Journal Clear Loading",NULL);
 Clear();
 SDL_LockMutex(loading_image_mutex);
 Loading_image_quit=true;
 SDL_UnlockMutex(loading_image_mutex);
 thread_return_value=0;
 SDL_WaitThread(_loading_image,&thread_return_value);
 Flip_Buffers(_screen);
}

void Journal::Start(std::bitset<NUMBER_OF_MAX_KEYS> *_progress,Texture *_screen)
{
 progress=_progress;
 Start(_screen);
}

Journal journal;

void Load_Journal()
{
 journal.Load("Journal");
}

void Clear_Journal()
{
 journal.Clear();
}
