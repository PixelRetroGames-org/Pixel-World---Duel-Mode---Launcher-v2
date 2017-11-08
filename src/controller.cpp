#include "controller.h"

Game_Controller controller[3];

void Init_Controllers()
{
 int number_of_controllers=SDL_NumJoysticks();
 if(number_of_controllers<1)
    return;
 for(int i=0;i<2 && i<number_of_controllers;i++)
     {
      controller[i+1].Init(i);
     }
}

void Destroy_Controllers()
{
 int number_of_controllers=SDL_NumJoysticks();
 if(number_of_controllers<1)
    return;
 for(int i=0;i<2 && i<number_of_controllers;i++)
     {
      controller[i+1].Destroy();
     }
}

void Update_Controllers_Events()
{
 SDL_JoystickUpdate();
 for(int i=0;i<2;i++)
     {
      controller[i+1].Update_Events();
     }
}

Game_Controller::Game_Controller()
{
 //Dpad
 up=down=left=right=false;
 //Shoulders
 left_shoulder=right_shoulder=false;
 //Buttons
 A_button=B_button=X_button=Y_button=false;
 //Joystick
 stickX=stickY=0;
}

void Game_Controller::Init(int index)
{
 id=SDL_GameControllerOpen(index);
}

void Game_Controller::Destroy()
{
 SDL_GameControllerClose(id);
 id=NULL;
}

void Game_Controller::Update_Events()
{
 if(id!=NULL && SDL_GameControllerGetAttached(id))
    {
     //Dpad
     up=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_DPAD_UP);
     down=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_DPAD_DOWN);
     left=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_DPAD_LEFT);
     right=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
     //Shoulders
     left_shoulder=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
     right_shoulder=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
     //Buttons
     A_button=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_A);
     B_button=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_B);
     X_button=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_X);
     Y_button=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_Y);
     //Joystick
     stickX=SDL_GameControllerGetAxis(id,SDL_CONTROLLER_AXIS_LEFTX);
     stickY=SDL_GameControllerGetAxis(id,SDL_CONTROLLER_AXIS_LEFTY);
     //Misc
     start_button=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_BACK);
     guide_button=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_GUIDE);
     back_button=SDL_GameControllerGetButton(id,SDL_CONTROLLER_BUTTON_START);
    }
}

SDL_GameController *Game_Controller::Get_id()
{
 return id;
}

bool Game_Controller::Pressed_Up()
{
 return up;
}

bool Game_Controller::Pressed_Down()
{
 return down;
}

bool Game_Controller::Pressed_Left()
{
 return left;
}

bool Game_Controller::Pressed_Right()
{
 return right;
}

bool Game_Controller::Pressed_LeftShoulder()
{
 return left_shoulder;
}

bool Game_Controller::Pressed_RightShoulder()
{
 return right_shoulder;
}

bool Game_Controller::Pressed_A_button()
{
 return A_button;
}

bool Game_Controller::Pressed_B_button()
{
 return B_button;
}

bool Game_Controller::Pressed_X_button()
{
 return X_button;
}

bool Game_Controller::Pressed_Y_button()
{
 return Y_button;
}

bool Game_Controller::Get_Button_State(int index)
{
 bool rtn;
 switch(index)
        {
         case 1: rtn=A_button; break;
         case 2: rtn=B_button; break;
         case 3: rtn=X_button; break;
         case 4: rtn=Y_button; break;
         default: rtn=false;
        }
 return rtn;
}

bool Game_Controller::Pressed_Back_button()
{
 return back_button;
}

bool Game_Controller::Pressed_Guide_button()
{
 return guide_button;
}

bool Game_Controller::Pressed_Start_button()
{
 return start_button;
}

bool Game_Controller::Pressed_Any_Button()
{
 bool rtn;
 //Dpad
 rtn=up || down || left || right;
 //Shoulders
 rtn=rtn || left_shoulder || right_shoulder;
 //Buttons
 rtn=rtn || A_button || B_button || X_button || Y_button;
 //Misc
 rtn=rtn || back_button || guide_button || start_button;
 return rtn;
}

int Game_Controller::Get_StickX()
{
 return stickX;
}

int Game_Controller::Get_StickY()
{
 return stickY;
}
