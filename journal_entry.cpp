#include "journal_entry.h"

void Journal_Entry::Clear()
{
 title[0]=title_image_name[0]=NULL;
 SDL_FreeSurface(title_image);
 title_image=NULL;
 beginning_key=ending_key=0;
 for(int i=0;i<number_of_pages;i++)
     pages_script_names[i][0]=NULL;
 number_of_pages=current_page=0;
}

void Journal_Entry::Load()
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"journal/journal entries/");
 strcat(path,title);
 strcat(path,".pwje");
 FILE *where=fopen(path,"r");
 fgets(title_image_name,sizeof title_image_name,where);
 if(title_image_name[strlen(title_image_name)-1]=='\n')
    title_image_name[strlen(title_image_name)-1]=NULL;
 fscanf(where,"%d %d ",&beginning_key,&ending_key);
 fscanf(where,"%d ",&number_of_pages);
 for(int i=0;i<number_of_pages;i++)
     {
      fgets(pages_script_names[i],sizeof pages_script_names[i],where);
      if(pages_script_names[i][strlen(pages_script_names[i])-1]=='\n')
         pages_script_names[i][strlen(pages_script_names[i])-1]=NULL;
     }
 fclose(where);
 strcpy(path,"journal/journal entries/images/");
 strcat(path,title_image_name);
 strcat(path,".bmp");
 SDL_Surface *image=load_image(path);
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",20);
 SDL_Color color={0,0,0};
 SDL_Surface *text_image=TTF_RenderText_Solid(font,title,color);
 title_image=load_image("images/journal/title_background_empty.bmp");
 apply_surface(5,5,text_image,title_image);
 apply_surface(5+text_image->w,5,image,title_image);
 TTF_CloseFont(font);
 SDL_FreeSurface(image);
 SDL_FreeSurface(text_image);
 current_page=0;
}

void Journal_Entry::Load(char *_title)
{
 Set_title(_title);
 Load();
}

void Journal_Entry::Set_title(char *_title)
{
 strcpy(title,_title);
}

void Journal_Entry::Handle_Events(SDL_Event *event)
{
 if(event->type==SDL_KEYDOWN)
    {
     if(event->key.keysym.sym==SDLK_LEFT || event->key.keysym.sym==SDLK_a)
        {
         current_page--;
         if(current_page<0)
            current_page=0;
         return;
        }
     if(event->key.keysym.sym==SDLK_RIGHT || event->key.keysym.sym==SDLK_d)
        {
         current_page++;
         if(current_page>=number_of_pages)
            current_page=number_of_pages-1;
         return;
        }
    }
}

void Journal_Entry::Print_Title(int x,int y,SDL_Surface *_screen,bool click,bool hover)
{
 if(!hover && !click)
    apply_surface(x,y,JOURNAL_ENTRY_BACKGROUND,_screen);
 if(hover && !click)
    apply_surface(x,y,JOURNAL_ENTRY_HOVER_BACKGROUND,_screen);
 if(click)
    apply_surface(x,y,JOURNAL_ENTRY_CLICK_BACKGROUND,_screen);
 apply_surface(x,y,title_image,_screen);
}

void Journal_Entry::Print_Page(int x,int y,SDL_Surface *_screen)
{
 Script_interpreter script_interpreter;
 script_interpreter.Start(_screen,pages_script_names[current_page],x,y);
}

bool Journal_Entry::Is_started(std::bitset<NUMBER_OF_MAX_KEYS> *progress)
{
 return ((*progress)[beginning_key]==true);
}

bool Journal_Entry::Is_finished(std::bitset<NUMBER_OF_MAX_KEYS> *progress)
{
 return ((*progress)[ending_key]==true);
}

bool Journal_Entry::Is_in_progress(std::bitset<NUMBER_OF_MAX_KEYS> *progress)
{
 return (Is_started(progress) && !Is_finished(progress));
}
