#include "journal_entry.h"

void Journal_Entry::Clear()
{
 title[0]=title_image_name[0]=NULL;
 Destroy_Texture(title_image);
 title_image=NULL;
 beginning_key=ending_key=0;
 for(int i=0;i<number_of_pages;i++)
     {
      pages_script_names[i][0]=NULL;
      Destroy_Texture(pages_images[current_page]);
      pages_images[current_page]=NULL;
     }
 number_of_pages=current_page=0;
}

const int PAGE_POSX=150,PAGE_POSY=100;

void Journal_Entry::Load()
{
 char path[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"journal/journal entries/");
 strcat(path,title);
 strcat(path,".pwje");
 FILE *where=fopen(path,"r");
 if(where==NULL)
    return;
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
 for(int i=0;i<number_of_pages;i++)
     {
      pages_images[i]=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
      Script_interpreter script_interpreter;
      script_interpreter.Start(pages_images[i],pages_script_names[i],PAGE_POSX,PAGE_POSY);
     }
 strcpy(path,"journal/journal entries/images/");
 strcat(path,title_image_name);
 strcat(path,".png");
 Texture *image=Load_Transparent_Texture(path);
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",20);
 SDL_Color color={52,124,191};
 Texture *text_image=Create_TTF_Texture(font,title,color);
 Texture *aux=Load_Transparent_Texture("images/journal/title_background_empty.png");
 title_image=Create_Transparent_Texture(aux->w,aux->h);
 Apply_Texture(0,0,aux,text_image);
 Destroy_Texture(aux);
 Apply_Texture(5,5,text_image,title_image);
 Apply_Texture(5+text_image->w,5,image,title_image);
 TTF_CloseFont(font);
 Destroy_Texture(image);
 Destroy_Texture(text_image);
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

int Journal_Entry::Get_title_sizeW()
{
 return title_image->w;
}

int Journal_Entry::Get_title_sizeH()
{
 return title_image->h;
}

bool Journal_Entry::Handle_Events(SDL_Event *event)
{
 if(event->type==SDL_KEYDOWN)
    {
     if(event->key.keysym.scancode==SDL_SCANCODE_LEFT || event->key.keysym.scancode==SDL_SCANCODE_A)
        {
         current_page--;
         if(current_page<0)
            current_page=0;
         return true;
        }
     if(event->key.keysym.scancode==SDL_SCANCODE_RIGHT || event->key.keysym.scancode==SDL_SCANCODE_D)
        {
         current_page++;
         if(current_page>=number_of_pages)
            current_page=number_of_pages-1;
         return true;
        }
    }
 return false;
}

void Journal_Entry::Print_Title(int x,int y,Texture *_screen,bool click,bool hover)
{
 if(!hover && !click)
    Apply_Texture(x,y,JOURNAL_ENTRY_BACKGROUND,_screen);
 if(hover && !click)
    Apply_Texture(x,y,JOURNAL_ENTRY_HOVER_BACKGROUND,_screen);
 if(click)
    Apply_Texture(x,y,JOURNAL_ENTRY_CLICK_BACKGROUND,_screen);
 Apply_Texture(x,y,title_image,_screen);
}

void Journal_Entry::Print_Page(Texture *_screen)
{
 Apply_Texture(0,0,pages_images[current_page],_screen);
}

bool Journal_Entry::Is_started(std::bitset<NUMBER_OF_MAX_KEYS> *progress)
{
 if(beginning_key==-1)
    return true;
 return ((*progress)[beginning_key]==true);
}

bool Journal_Entry::Is_finished(std::bitset<NUMBER_OF_MAX_KEYS> *progress)
{
 if(ending_key==-1)
    return false;
 return ((*progress)[ending_key]==true);
}

bool Journal_Entry::Is_in_progress(std::bitset<NUMBER_OF_MAX_KEYS> *progress)
{
 return (Is_started(progress) && !Is_finished(progress));
}
