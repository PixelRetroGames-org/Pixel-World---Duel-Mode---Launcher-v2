#include "script_interpreter.h"
#include<cstring>

const char *command_names[number_of_commands+1]={"set","line","color","la","ba","sleep","wla","we","bckimg","img","page"};
const SDL_Color default_text_color={255,255,255};

Script_interpreter::Script_interpreter()
{
 text_color.push(default_text_color);
}

void Script_interpreter::Start_line_audio(Mix_Chunk *line_audio)
{
 Mix_PlayChannel(2,line_audio,0);
}

void Script_interpreter::Pause_line_audio()
{
 if(!Mix_Paused(2))
    Mix_Pause(2);
}

void Script_interpreter::Unpause_line_audio()
{
 if(Mix_Paused(2))
    Mix_Resume(2);
}

void Script_interpreter::Stop_line_audio()
{
 Mix_HaltChannel(2);
}

void Script_interpreter::Start_background_audio(Mix_Music *background_audio)
{
 if(Mix_PlayingMusic()==0)
    Mix_PlayMusic(background_audio,-1);
}

void Script_interpreter::Pause_background_audio()
{
 if(Mix_PlayingMusic()==1)
    Mix_PauseMusic();
}

void Script_interpreter::Unpause_background_audio()
{
 if(Mix_PlayingMusic()==0)
    Mix_ResumeMusic();
}

void Script_interpreter::Stop_background_audio()
{
 Mix_HaltMusic();
}

void Script_interpreter::Print_line(int &x,int y,char *_line,bool on_screen)
{
 SDL_Surface *image=NULL;
 TTF_Font *font=TTF_OpenFont("fonts/pixel3.ttf",50);
 image=TTF_RenderText_Solid(font,_line,text_color.top());
 if(image!=NULL)
    {
     apply_surface(x,y,image,buffer);
     x+=image->w;
     bufferW+=image->w;
    }
 if(on_screen)
    {
     apply_surface((screen->w-bufferW)/2,y,buffer,screen);
     bufferW=0;
     SDL_FreeSurface(buffer);
     buffer=make_it_transparent("script/images/empty.bmp");
     SDL_Flip(screen);
     SDL_FreeSurface(image);
     TTF_CloseFont(font);
     return;
    }
 SDL_Flip(screen);
 SDL_FreeSurface(image);
 TTF_CloseFont(font);
}

void Script_interpreter::Print_image(int &x,int y,char *_name)
{
 char aux[TEXT_LENGHT_MAX]={NULL};
 strcpy(aux,"script/images/");
 strcat(aux,_name);
 strcat(aux,".bmp");
 SDL_Surface *image=make_it_transparent(aux);
 apply_surface((screen->w-image->w)/2,y,image,screen);
 SDL_Flip(screen);
 SDL_FreeSurface(image);
}

void Script_interpreter::Set_background_image(char *_name)
{
 char aux[TEXT_LENGHT_MAX]={NULL};
 strcpy(aux,"script/images/");
 strcat(aux,_name);
 strcat(aux,".bmp");
 background_image=load_image(aux);
}

void Script_interpreter::Set_script_name(char *_script_name)
{
 strcpy(script_name,_script_name);
}

void Script_interpreter::Set_screen(SDL_Surface *_screen)
{
 screen=_screen;
}

void Script_interpreter::Start()
{
 char path[TEXT_LENGHT_MAX]={NULL};
 //system("cd \\script");
 strcpy(path,"script\\");
 strcat(path,script_name);
 strcat(path,".pwst");
 //FILE *out=fopen(script_name,"w");
 //fclose(out);
 std::FILE *in=std::fopen(path,"r");
 //system("cd ..");
 bool quit=false,line=false;
 char ch=NULL,command[TEXT_LENGHT_MAX]={NULL},script_line[TEXT_LENGHT_MAX]={NULL};
 int x=text_pos_x,y=text_pos_y;
 SDL_Event event;
 buffer=make_it_transparent("script/images/empty.bmp");
 bufferW=0;
 while(SDL_PollEvent(&event));
 while(!quit)
       {
        ch=fgetc(in);
        if(ch=='<')
           {
            fscanf(in," %s ",command);
            if(command[0]=='/')
               {
                switch(Get_command_id(command+1))
                       {
                        case 1: Print_line(x,y,script_line,true),memset(script_line,0,sizeof script_line),line=false; break;
                        case 2: Print_line(x,y,script_line),memset(script_line,0,sizeof script_line),text_color.pop(); break;
                        case 3: Stop_line_audio(); Mix_FreeChunk(chunk); break;
                        case 4: Stop_background_audio(); Mix_FreeMusic(music);break;
                        case 10:apply_surface(text_pos_x,text_pos_y,background_image,screen);
                                x=text_pos_x,y=text_pos_y;line=false;
                                bufferW=0;
                                SDL_FreeSurface(buffer);
                                buffer=make_it_transparent("script/images/empty.bmp");
                                break;
                       }
               }
            else
               {
                SDL_Color aux;
                int r,g,b,_x,_y;
                char audio[TEXT_LENGHT_MAX]={"script/audio/"},aux1[TEXT_LENGHT_MAX]={NULL};
                int aux2;
                switch(Get_command_id(command))
                       {
                        case 1: //x=text_pos_x,y=text_pos_y;
                                line=true;
                                bufferW=0;
                                SDL_FreeSurface(buffer);
                                buffer=make_it_transparent("script/images/empty.bmp");
                                break;
                        case 2: fscanf(in," %d %d %d ",&r,&g,&b);
                                aux.r=r;aux.g=g;aux.b=b;
                                Print_line(x,y,script_line);
                                memset(script_line,0,sizeof script_line);
                                text_color.push(aux);
                                break;
                        case 3:
                                fscanf(in," %s ",aux1);
                                strcat(audio,aux1);
                                chunk=Mix_LoadWAV(audio);
                                Start_line_audio(chunk);
                                break;
                        case 4:
                                fscanf(in," %s ",aux1);
                                strcat(audio,aux1);
                                music=Mix_LoadMUS(audio);
                                Start_background_audio(music); break;
                        case 5:
                                fscanf(in," %d ",&aux2);
                                SDL_Delay(aux2);
                                break;
                        case 6: while(Mix_Playing(2))
                                      SDL_Delay(10);
                                break;
                        case 7: while(SDL_PollEvent(&event));
                                aux2=1;
                                SDL_PollEvent(&event);
                                while(event.type!=SDL_KEYDOWN && event.type!=SDL_MOUSEBUTTONDOWN)
                                      {
                                       SDL_PollEvent(&event);
                                      }
                                break;
                        case 8: fscanf(in,"%s ",aux1);
                                Set_background_image(aux1);
                                break;
                        case 9: fscanf(in,"%d %d %s ",&x,&y,aux1);
                                Print_image(x,y,aux1);
                                break;
                        case 10:apply_surface(text_pos_x,text_pos_y,background_image,screen);
                                x=text_pos_x,y=text_pos_y;line=false;
                                bufferW=0;
                                SDL_FreeSurface(buffer);
                                buffer=make_it_transparent("script/images/empty.bmp");
                                break;
                       }
               }
            ch=fgetc(in);
           }
        else
           {
            if(line)
               {
                if(ch=='\n')
                   Print_line(x,y,script_line,true),x=text_pos_x,y+=LINE_HEIGHT;
                else
                   script_line[strlen(script_line)]=ch;
               }
           }
        quit=feof(in);
       }
 fclose(in);
}

void Script_interpreter::Start(SDL_Surface *_screen,char *_script_name)
{
 Set_screen(_screen);
 Set_script_name(_script_name);
 Start();
}

int Get_command_id(char *_command)
{
 for(int id=0;id<number_of_commands;id++)
     if(strcmp(_command,command_names[id])==0)
        return id;
 return -1;
}
