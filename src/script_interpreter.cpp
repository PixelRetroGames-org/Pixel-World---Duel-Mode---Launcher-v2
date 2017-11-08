#include "script_interpreter.h"

const char *command_names[number_of_commands+1]={"set","line","color","la","ba","sleep","wla","we","bckimg","img","page","flip"};
const SDL_Color default_text_color={255,255,255};

//Script_interpreter::Script_interpreter();

Script_interpreter::~Script_interpreter()
{
 while(!text_color.empty())
       text_color.pop();
 buffer=NULL;
}

void Script_interpreter::Clear()
{
 while(!text_color.empty())
       text_color.pop();
 Destroy_Texture(buffer);
 buffer=NULL;
 if(background_image!=SCRIPT_default_background_image)
    {
     Destroy_Texture(background_image);
     background_image=NULL;
    }
 Destroy_Texture(big_buffer);
 big_buffer=NULL;
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
 Texture *image=NULL;
 TTF_Font *font=TTF_OpenFont("fonts/pixel3.ttf",50);
 image=Create_TTF_Texture(font,_line,text_color.top());
 if(image!=NULL)
    {
     Apply_Texture(x,y,image,buffer);
     x+=image->w;
     bufferW+=image->w;
    }
 if(on_screen)
    {
     Apply_Texture((RESOLUTION_W-bufferW)/2,y,buffer,big_buffer);
     bufferW=0;
     Destroy_Texture(buffer);
     buffer=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
     Destroy_Texture(image);
     TTF_CloseFont(font);
     return;
    }
 Destroy_Texture(image);
 TTF_CloseFont(font);
}

void Script_interpreter::Print_image(int &x,int y,char *_name)
{
 char aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(aux,"script/images/");
 strcat(aux,_name);
 strcat(aux,".png");
 Texture *image=Load_Transparent_Texture(aux);
 Apply_Texture((RESOLUTION_W-image->w)/2,y,image,big_buffer);
 Destroy_Texture(image);
}

void Script_interpreter::Set_background_image(char *_name)
{
 char aux[TEXT_LENGTH_MAX]={NULL};
 strcpy(aux,"script/images/");
 strcat(aux,_name);
 strcat(aux,".png");
 background_image=Load_Transparent_Texture(aux);
 Apply_Texture(0,0,background_image,big_buffer);
}

void Script_interpreter::Set_script_name(char *_script_name)
{
 strcpy(script_name,_script_name);
}

void Script_interpreter::Set_screen(Texture *_screen)
{
 screen=_screen;
}

void Script_interpreter::Start(int X,int Y)
{
 screen_posX=X;
 screen_posY=Y;
 char path[TEXT_LENGTH_MAX]={NULL};
 //system("cd \\script");
 strcpy(path,"script/");
 strcat(path,script_name);
 strcat(path,".pwst");
 //FILE *out=fopen(script_name,"w");
 //fclose(out);
 FILE *in=fopen(path,"r");
 if(in==NULL)
    {
     char s[TEXT_LENGTH_MAX];
     strcpy(s,"Cannot read from \"");
     strcat(s,path);
     strcat(s,"\": ");
     strcat(s,strerror(errno));
     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Script interpreter error",s,WINDOW);
    }
 //system("cd ..");
 bool quit=false,line=false;
 char ch=NULL,command[TEXT_LENGTH_MAX]={NULL},script_line[TEXT_LENGTH_MAX]={NULL};
 int x=text_pos_x,y=text_pos_y;
 SDL_Event event;
 buffer=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
 big_buffer=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
 bufferW=0;
 Apply_Texture(0,0,big_buffer,screen);
 Flip_Buffers(screen);
 SDL_Delay(100);
 while(SDL_PollEvent(&event))
       {
        Update_Controllers_Events();
       }
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
                        case 1: Print_line(x,y,script_line,true),memset(script_line,0,sizeof script_line),line=false; x=text_pos_x,y+=LINE_HEIGHT; break;
                        case 2: Print_line(x,y,script_line),memset(script_line,0,sizeof script_line);
                                if(text_color.size()>1)
                                   text_color.pop();
                                break;
                        case 3: Stop_line_audio(); Mix_FreeChunk(chunk); break;
                        case 4: Stop_background_audio(); Mix_FreeMusic(music);break;
                        case 10:Apply_Texture(0,0,background_image,big_buffer);
                                x=text_pos_x,y=text_pos_y;line=false;
                                bufferW=0;
                                Destroy_Texture(buffer);
                                buffer=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
                                break;
                       }
               }
            else
               {
                SDL_Color aux;
                int r,g,b,_x,_y;
                char audio[TEXT_LENGTH_MAX]={NULL},aux1[TEXT_LENGTH_MAX]={NULL};
                int aux2;
                switch(Get_command_id(command))
                       {
                        case 1: //x=text_pos_x,y=text_pos_y;
                                line=true;
                                bufferW=0;
                                Destroy_Texture(buffer);
                                buffer=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
                                break;
                        case 2: fscanf(in," %d %d %d ",&r,&g,&b);
                                aux.r=r;aux.g=g;aux.b=b;
                                Print_line(x,y,script_line);
                                memset(script_line,0,sizeof script_line);
                                text_color.push(aux);
                                break;
                        case 3:
                                fgets(aux1,sizeof aux1,in);
                                if(aux1[strlen(aux1)-1]=='\n')
                                   aux1[strlen(aux1)-1]=NULL;
                                strcat(audio,aux1);
                                chunk=Mix_LoadWAV(audio);
                                Start_line_audio(chunk);
                                break;
                        case 4:
                                fgets(aux1,sizeof aux1,in);
                                if(aux1[strlen(aux1)-1]=='\n')
                                   aux1[strlen(aux1)-1]=NULL;
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
                        case 7: SDL_Delay(200);
                                while(SDL_PollEvent(&event))
                                      {
                                       Update_Controllers_Events();
                                      }
                                aux2=1;
                                SDL_PollEvent(&event);
                                Update_Controllers_Events();
                                while((event.type!=SDL_KEYDOWN && event.type!=SDL_MOUSEBUTTONDOWN) && !controller[1].Pressed_Any_Button() && !controller[2].Pressed_Any_Button())
                                      {
                                       SDL_PollEvent(&event);
                                       Update_Controllers_Events();
                                       SDL_Delay(25);
                                      }
                                break;
                        case 8: fscanf(in,"%s ",aux1);
                                Set_background_image(aux1);
                                Apply_Texture(0,0,background_image,screen);
                                Flip_Buffers(screen);
                                break;
                        case 9: fscanf(in,"%d %d %s ",&x,&y,aux1);
                                Print_image(x,y,aux1);
                                break;
                        case 10:Apply_Texture(0,0,background_image,big_buffer);
                                x=text_pos_x,y=text_pos_y;line=false;
                                bufferW=0;
                                Destroy_Texture(buffer);
                                buffer=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
                                break;
                        case 11:Apply_Texture(X/2,0,big_buffer,screen);
                                Flip_Buffers(screen);
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
 Clear();
}

void Script_interpreter::Start(Texture *_screen,char *_script_name,int X,int Y)
{
 text_color.push(default_text_color);
 Set_screen(_screen);
 Set_script_name(_script_name);
 Start(X,Y);
}

int Get_command_id(char *_command)
{
 for(int id=0;id<number_of_commands;id++)
     if(strcmp(_command,command_names[id])==0)
        return id;
 return -1;
}
