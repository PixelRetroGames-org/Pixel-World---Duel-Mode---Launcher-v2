#include "puzzle.h"

Puzzle::Puzzle()
{
 type=number_of_answers=0;
 title[0]=text[0]=text_typed[0]=name[0]=NULL;
 for(int i=0;i<NUMBER_OF_ANSWERS_MAX;i++)
     answers[i][0]=NULL;
 title_image=text_image=text_typed_image=answer_image=NULL;
 background_image=NULL;
}

Puzzle::~Puzzle()
{
 Clear(true);
}

void Puzzle::Clear(bool _delete)
{
 type=number_of_answers=0;
 title[0]=text[0]=text_typed[0]=NULL;
 for(int i=0;i<NUMBER_OF_ANSWERS_MAX;i++)
     answers[i][0]=NULL;
 if(_delete)
    {
     SDL_FreeSurface(title_image);
     SDL_FreeSurface(text_image);
     SDL_FreeSurface(text_typed_image);
     SDL_FreeSurface(answer_image);
     SDL_FreeSurface(background_image);
    }
 title_image=text_image=text_typed_image=answer_image=NULL;
 background_image=NULL;
}

void Puzzle::Set_name(char *_name)
{
 strcpy(name,_name);
}

void Puzzle::Load()
{
 char path[TEXT_LENGTH_MAX]={NULL};
 char aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(path,"puzzles/");
 strcat(path,name);
 strcat(path,".pwp");
 FILE *where=fopen(path,"r");
 fscanf(where,"%d ",&type);
 fgets(title,sizeof title,where);
 if(title[strlen(title)-1]=='\n')
    title[strlen(title)-1]=NULL;
 int number_of_keys=0;
 fscanf(where,"%d ",&number_of_keys);
 for(int i=0;i<number_of_keys;i++)
     {
      int key_id;
      fscanf(where,"%d ",&key_id);
      keys[key_id]=true;
     }
 switch(type)
        {
         case 1:
                fgets(text,sizeof text,where);
                if(text[strlen(text)-1]=='\n')
                   text[strlen(text)-1]=NULL;
                fscanf(where,"%d ",&number_of_answers);
                for(int i=0;i<number_of_answers;i++)
                    {
                     fgets(answers[i],sizeof answers[i],where);
                     if(answers[i][strlen(answers[i])-1]=='\n')
                        answers[i][strlen(answers[i])-1]=NULL;
                    }
                break;
        }
 fgets(aux,sizeof aux,where);
 if(aux[strlen(aux)-1]=='\n')
    aux[strlen(aux)-1]=NULL;
 strcpy(path,"puzzles/images/");
 strcat(path,aux);
 strcat(path,".bmp");
 background_image=load_image(path);
 fclose(where);
}

bool Puzzle::Start(SDL_Surface *_screen)
{
 switch(type)
        {
         ///Riddle
         case 1: return Start_Riddle(_screen);
        }
}

bool Puzzle::Validate_Riddle()
{
 for(int i=0;i<number_of_answers;i++)
     if(strcmp(answers[i],text_typed)==0)
        return true;
 return false;
}

const int PUZZLE_TEXT_LENGTH_MAX=20,BACKSPACE_POS=2*('z'-'a')+2,COOLDOWN_TIME=100;

int Get_Key(char ch)
{
 if(ch>='a' && ch<='z')
    return ch-'a'+1;
 if(ch>='A' && ch<='Z')
    return ch-'A'+'z'-'a'+2;
 if(ch==' ')
    return 2*('z'-'a')+1;
 return 0;
}

bool Puzzle::Start_Riddle(SDL_Surface *_screen)
{
 bool quit=false,text_typed_modified=false;
 SDL_Event event;
 SDL_PollEvent(&event);
 char ch[2]={NULL,NULL};
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Color answer_color={102,153,0},title_color={169,57,255},text_color={243,238,120};
 SDL_Surface *text_typed_background_image=make_it_transparent("images/puzzle/riddle_type_background.bmp");
 title_image=TTF_RenderText_Solid(font,title,title_color);
 text_image=TTF_RenderText_Solid(font,text,text_color);
 apply_surface(0,0,background_image,_screen);
 apply_surface((RESOLUTION_X-text_typed_background_image->w)/2,title_image->h+10+(RESOLUTION_Y-text_typed_background_image->h)/2,text_typed_background_image,_screen);
 apply_surface((RESOLUTION_X-title_image->w)/2,0,title_image,_screen);
 apply_surface((RESOLUTION_X-text_image->w)/2,title_image->h+12,text_image,_screen);
 SDL_Flip(_screen);
 SDL_EnableUNICODE(SDL_ENABLE);
 SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
 int length=1;
 int keys_cooldown[2*('z'-'a')+3];
 for(int i=0;i<2*('z'-'a')+3;i++)
     keys_cooldown[i]=0;
 while(!quit)
       {
        if(event.key.type==SDL_KEYDOWN)
           {
            text_typed_modified=false;
            if(event.key.keysym.sym==SDLK_ESCAPE)
               quit=true;
            if(event.key.keysym.sym==SDLK_BACKSPACE && length!=0 && keys_cooldown[BACKSPACE_POS]==0)
               text_typed[length-1]=NULL,text_typed_modified=true,length--,keys_cooldown[BACKSPACE_POS]=COOLDOWN_TIME;
            if(((event.key.keysym.unicode>=(Uint16)'a' && event.key.keysym.unicode<=(Uint16)'z') ||
               (event.key.keysym.unicode>=(Uint16)'A' && event.key.keysym.unicode<=(Uint16)'Z') ||
               (event.key.keysym.unicode==(Uint16)' ')) && length<PUZZLE_TEXT_LENGTH_MAX && keys_cooldown[Get_Key((char)event.key.keysym.unicode)]==0)
               {
                ch[0]=(char)event.key.keysym.unicode;
                ch[1]=NULL;
                strcat(text_typed,ch);
                text_typed_modified=true;
                length++;
                keys_cooldown[Get_Key((char)event.key.keysym.unicode)]=COOLDOWN_TIME;
               }
            if(event.key.keysym.sym==SDLK_RETURN)
               if(Validate_Riddle())
                  return true;
            if(text_typed_modified)
               {
                SDL_FreeSurface(text_typed_image);
                text_typed_image=NULL;
                apply_surface((RESOLUTION_X-text_typed_background_image->w)/2,title_image->h+10+(RESOLUTION_Y-text_typed_background_image->h)/2,text_typed_background_image,_screen);
                if(length!=0)
                   {
                    text_typed_image=TTF_RenderText_Solid(font,text_typed,answer_color);
                    apply_surface((RESOLUTION_X-text_typed_image->w)/2,title_image->h+12+(RESOLUTION_Y-text_typed_image->h)/2,text_typed_image,_screen);
                   }
                SDL_Flip(_screen);
               }
            SDL_Delay(50);
           }
        for(int i=0;i<2*('z'-'a')+3;i++)
            {
             keys_cooldown[i]-=10;
             if(i==BACKSPACE_POS)
                keys_cooldown[i]-=20;
             if(keys_cooldown[i]<=0)
                keys_cooldown[i]=0;
            }
        SDL_PollEvent(&event);
       }
 TTF_CloseFont(font);
 SDL_FreeSurface(text_typed_background_image);
 SDL_FreeSurface(text_typed_image);
 SDL_FreeSurface(title_image);
 SDL_FreeSurface(text_image);
 SDL_FreeSurface(background_image);
 SDL_EnableUNICODE(SDL_DISABLE);
 SDL_EnableKeyRepeat(0,0);
}

