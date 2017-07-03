#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "library.h"

class Game_Controller
{
 private:
 SDL_GameController *id;
 //Dpad
 bool up,down,left,right;
 //Shoulders
 bool left_shoulder,right_shoulder;
 //Buttons
 bool A_button,B_button,X_button,Y_button;
 //Joystick
 int stickX,stickY;
 //Misc
 bool back_button,guide_button,start_button;

 public:
 Game_Controller();
 void Init(int _id);
 void Destroy();
 void Update_Events();
 SDL_GameController *Get_id();
 bool Pressed_Up();
 bool Pressed_Down();
 bool Pressed_Left();
 bool Pressed_Right();
 bool Pressed_LeftShoulder();
 bool Pressed_RightShoulder();
 bool Pressed_A_button();
 bool Pressed_B_button();
 bool Pressed_X_button();
 bool Pressed_Y_button();
 bool Get_Button_State(int index);
 bool Pressed_Back_button();
 bool Pressed_Guide_button();
 bool Pressed_Start_button();
 bool Pressed_Any_Button();
 int Get_StickX();
 int Get_StickY();
};

extern Game_Controller controller[3];

void Init_Controllers();
void Destroy_Controllers();
void Update_Controllers_Events();

#endif // CONTROLLER_H
