#ifndef SCRIPT_INTERPRETER_H
#define SCRIPT_INTERPRETER_H

#include "library.h"
#include "texture.h"
#include "load_images.h"
#include "settings.h"
#include "controller.h"

#include <algorithm>
#include <stack>
#include <cstring>

const int number_of_commands=12,LINE_HEIGHT=23;
extern const char *command_names[number_of_commands+1];

class Script_interpreter
{
 private:
 char script_name[TEXT_LENGTH_MAX]={NULL};
 Texture *screen=NULL,*background_image=SCRIPT_default_background_image,*buffer,*big_buffer;
 int bufferW=0;
 int text_pos_x=0,text_pos_y=0;
 Mix_Chunk *chunk=NULL;
 Mix_Music *music=NULL;
 std::stack<SDL_Color> text_color;
 int screen_posX,screen_posY;

 void Start_line_audio(Mix_Chunk *line_audio);
 void Pause_line_audio();
 void Unpause_line_audio();
 void Stop_line_audio();
 void Start_background_audio(Mix_Music *background_audio);
 void Pause_background_audio();
 void Unpause_background_audio();
 void Stop_background_audio();
 void Print_line(int &x,int y,char *_line,bool on_screen=false);
 void Print_image(int &x,int y,char *_name);
 void Set_background_image(char *_name);

 public:
 //Script_interpreter();
 ~Script_interpreter();
 void Clear();
 void Set_script_name(char *_script_name);
 void Set_screen(Texture *_screen);
 void Start(int X=0,int Y=0);
 void Start(Texture *_screen,char *_script_name,int X=0,int Y=0);
};

int Get_command_id(char *_command);

#endif // SCRIPT_INTERPRETER_H
