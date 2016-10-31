#include "puzzle.h"

Puzzle::Puzzle()
{
 id=0,type=0;
 title[0]=text[0]=text_typed[0]=answer[0]=NULL;
 title_image=text_image=text_typed_image=answer_image=NULL;
 background_image=NULL;
}

Puzzle::~Puzzle()
{
 Clear(true);
}

void Puzzle::Clear(bool _delete=false)
{
 id=type=0;
 title[0]=text[0]=text_typed[0]=answer[0]=NULL;
 if(_delete)
    {
     SDL_FreeSurface(title_image);
     SDL_FreeSurface(text_image);
     SDL_FreeSurface(text_typed_image);
     SDL_FreeSurface(answer_image);
     SDL_FreeSurface(background_image)
    }
 title_image=text_image=text_typed_image=answer_image=NULL;
 background_image=NULL;
}

void Puzzle::Set_id(int _id)
{
 id=_id;
}

int Puzzle::Get_id()
{
 return id;
}

void Puzzle::Load()
{
 if(id==0)
    return;
 char path[TEXT_LENGHT_MAX]={NULL};
 char aux[TEXT_LENGHT_MAX]={NULL};
 itoa(id,aux);
 strcpy(path,"puzzles/");
 strcat(path,aux);
 strcat(path,".pwp");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d ",&type);
 fgets(title,sizeof title,where);
 if(title[strlen(title)-1]=='\n')
    title[strlen(title)-1]=NULL;
 fgets(text,sizeof text,where);
 if(text[strlen(text)-1]=='\n')
    text[strlen(text)-1]=NULL;
 fgets(answer,sizeof answer,where);
 if(answer[strlen(answer)-1]=='\n')
    answer[strlen(answer)-1]=NULL;
 fgets(aux,sizeof aux,where);
 if(aux[strlen(aux)-1]=='\n')
    aux[strlen(aux)-1]=NULL;
 strcpy(path,"puzzles/images/");
 strcat(path,aux);
 strcat(path,".bmp");
 background_image=load_image(path);
 fclose(where);
}

bool Puzzle::Start(SDL_Surface *_screen);
void Puzzle::Print(SDL_Surface *_screen);
void Puzzle::Handle_Events(SDL_Event);
