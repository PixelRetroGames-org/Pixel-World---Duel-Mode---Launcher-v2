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
     Destroy_Texture(title_image);
     Destroy_Texture(text_image);
     Destroy_Texture(text_typed_image);
     Destroy_Texture(answer_image);
     Destroy_Texture(background_image);
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
 if(where==NULL)
    return;
 fscanf(where,"%d ",&type);
 fgets(title,sizeof title,where);
 if(title[strlen(title)-1]=='\n')
    title[strlen(title)-1]=NULL;
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
                     int length=strlen(answers[i])-1;
                     if(answers[i][length]=='\n')
                        answers[i][length]=NULL,length--;
                    }
                break;
        }
 fgets(aux,sizeof aux,where);
 if(aux[strlen(aux)-1]=='\n')
    aux[strlen(aux)-1]=NULL;
 strcpy(path,"puzzles/images/");
 strcat(path,aux);
 strcat(path,".png");
 background_image=Load_Texture(path);
 fclose(where);
}

bool Puzzle::Start(Texture *_screen)
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

void Puzzle::Print_Verdict(bool verdict,Texture *_screen)
{
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",50);
 SDL_Color right_color={0,205,0},wrong_color={207,0,0};
 Texture *right_message=Create_TTF_Texture(font,"Right!",right_color);
 Texture *wrong_message=Create_TTF_Texture(font,"Wrong!",wrong_color);
 TTF_CloseFont(font);
 Apply_Texture(0,0,background_image,_screen);
 if(verdict==false)
    Apply_Texture((RESOLUTION_W-wrong_message->w)/2,300,wrong_message,_screen);
 else
    Apply_Texture((RESOLUTION_W-right_message->w)/2,300,right_message,_screen);
 Flip_Buffers(_screen);
 SDL_Delay(300);
 Destroy_Texture(right_message);
 Destroy_Texture(wrong_message);
}

const int PUZZLE_TEXT_LENGTH_MAX=45;

bool Puzzle::Start_Riddle(Texture *_screen)
{
 bool quit=false,text_typed_modified=false;
 SDL_Event event;
 SDL_PollEvent(&event);
 char ch[2]={NULL,NULL};
 TTF_Font *font=TTF_OpenFont("fonts/pixel.ttf",30);
 SDL_Color answer_color={102,153,0},title_color={169,57,255},text_color={243,238,120};
 Texture *text_typed_background_image=Load_Transparent_Texture("images/puzzle/riddle_type_background.png");
 title_image=Create_TTF_Texture(font,title,title_color);
 text_image=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
 Script_interpreter script_interpreter;
 script_interpreter.Start(text_image,text,0,0);
 Apply_Texture(0,0,background_image,_screen);
 Apply_Texture(0,0,text_image,_screen);
 Apply_Texture((RESOLUTION_W-text_typed_background_image->w)/2,title_image->h+10+(RESOLUTION_H-text_typed_background_image->h)/2,text_typed_background_image,_screen);
 Apply_Texture((RESOLUTION_W-title_image->w)/2,0,title_image,_screen);
 Flip_Buffers(_screen);
 SDL_StartTextInput();
 int length=0;
 bool full=false;
 while(!quit)
       {
        if(SDL_PollEvent(&event))
           {
            if(event.key.type==SDL_KEYDOWN)
               {
                text_typed_modified=false;
                if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE)
                   quit=true;
                if(event.key.keysym.scancode==SDL_SCANCODE_BACKSPACE && length!=0)
                   text_typed[length-1]=NULL,text_typed_modified=true,length--;
                if(event.key.keysym.scancode==SDL_SCANCODE_RETURN)
                   {
                    if(Validate_Riddle())
                       {
                        Print_Verdict(true,_screen);
                        TTF_CloseFont(font);
                        Destroy_Texture(text_typed_background_image);
                        SDL_StopTextInput();
                        return true;
                       }
                    Print_Verdict(false,_screen);
                    TTF_CloseFont(font);
                    Destroy_Texture(text_typed_background_image);
                    SDL_StopTextInput();
                    return false;
                   }
               }
            else
               {
                if(event.type==SDL_TEXTINPUT)
                   {
                    if(length<PUZZLE_TEXT_LENGTH_MAX && !full)
                        {
                         strcat(text_typed,event.text.text);
                         text_typed_modified=true;
                         int n=strlen(event.text.text);
                         length+=n;
                        }
                   }
               }
           }
        if(text_typed_modified)
           {
            Destroy_Texture(text_typed_image);
            text_typed_image=NULL;
            Apply_Texture(0,0,background_image,_screen);
            Apply_Texture(0,0,text_image,_screen);
            Apply_Texture((RESOLUTION_W-text_typed_background_image->w)/2,title_image->h+10+(RESOLUTION_H-text_typed_background_image->h)/2,text_typed_background_image,_screen);
            Apply_Texture((RESOLUTION_W-title_image->w)/2,0,title_image,_screen);
            if(length!=0)
               {
                text_typed_image=Create_TTF_Texture(font,text_typed,answer_color);
                full=(text_typed_image->w>=text_typed_background_image->w-40);
                Apply_Texture((RESOLUTION_W-text_typed_image->w)/2,title_image->h+12+(RESOLUTION_H-text_typed_image->h)/2,text_typed_image,_screen);
               }
            Flip_Buffers(_screen);
           }
       }
 TTF_CloseFont(font);
 Destroy_Texture(text_typed_background_image);
 SDL_StopTextInput();
 return false;
}

