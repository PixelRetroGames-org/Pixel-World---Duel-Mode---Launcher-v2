#ifndef SCRIPT_INTERPRETATOR_H
#define SCRIPT_INTERPRETATOR_H

#include "library.h"
#include "load_images.h"
#include<algorithm>
#include<stack>

#ifndef TEXT_LENGTH_MAX
#define TEXT_LENGTH_MAX 100
#endif // TEXT_LENGTH_MAX

const int number_of_commands=11,LINE_HEIGHT=23;
extern const char *command_names[number_of_commands+1];

class Script_interpreter
{
 private:
 char script_name[TEXT_LENGTH_MAX]={NULL};
 SDL_Surface *screen=NULL,*background_image=SCRIPT_default_background_image,*buffer;
 int bufferW=0;
 int text_pos_x=0,text_pos_y=0;
 Mix_Chunk *chunk=NULL;
 Mix_Music *music=NULL;
 std::stack<SDL_Color> text_color;

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
 void Set_screen(SDL_Surface *_screen);
 void Start();
 void Start(SDL_Surface *_screen);
 void Start(SDL_Surface *_screen,char *_script_name);
};

int Get_command_id(char *_command);

#endif // SCRIPT_INTERPRETATOR_H
