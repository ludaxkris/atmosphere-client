#include "Controller.h"

Controller::Controller()
{
	//Load configuration file
	cManager = new ConfigManager();
	config = cManager->buildController();

	//Enables camera rotation
	b_camMoveEn = false;

	//Left stick position
	b_leftStickMove = false;

	f_leftThumbXPosition = 0.0f;
	f_leftThumbYPosition = 0.0f;

	f_leftThumbXRaw = 0.0f;
	f_leftThumbYRaw = 0.0f;

	//Right stick position
	b_rightStickMove = false;

	f_rightThumbXPosition = 0.0f;
	f_rightThumbYPosition = 0.0f;

	f_rightThumbXRaw = 0.0f;
	f_rightThumbYRaw = 0.0f;

	initButtonState();

	b_leftTrigger = false;
	b_rightTrigger = false;

	f_leftTriggerPosition = 0.0f;
	f_rightTriggerPosition = 0.0f;

	initVibrationStates();
}

void Controller::initButtonState()
{
	//buttons
	b_up = false;
	b_down = false;
	b_left = false;
	b_right = false;
	b_start = false;
	b_back = false;
	b_leftThumb = false;
	b_rightThumb = false;
	b_leftShoulder = false;
	b_rightShoulder = false;
	b_a = false;
	b_b = false;
	b_x = false;
	b_y = false;
}

void Controller::CheckControllerStatus()
{
	b_camMoveEn = false;
	b_leftStickMove = false;
	b_rightStickMove = false;
	initButtonState();			//Reset the button flags
	b_leftTrigger = false;
	b_rightTrigger = false;


	DWORD dwResult;         // Used to store if a controller is connected
    XINPUT_STATE state;     // Data type that stores all the current states
		                    // of a controller.

    ZeroMemory( &state, sizeof(XINPUT_STATE) ); // Zeros out the states of
                                                  // the controller.

    // Get all the current states of controller 1
    dwResult = XInputGetState( 0, &state );

	//check if the controller is connected
    if( dwResult == ERROR_SUCCESS )
    {
		checkLeftStick(state);
		checkRightStick(state);
		checkButtons(state);
		checkTriggers(state);

		//*************Controller Demo**********************
		/*
		if(moveLeftStick())
		{
			printf("LS: %f, %f \n",GetLeftThumbRaw().x, GetLeftThumbRaw().y);
		}
		if(pressUp()){printf("up\n");}
		if(pressDown()){printf("down\n");}
		if(pressLeft()){printf("left\n");}
		if(pressRight()){printf("right\n");}
		if(pressStart()){printf("start\n");}
		if(pressBack()){printf("back\n");}
		if(pressLeftThumb()){printf("leftThumb\n");}
		if(pressRightThumb()){printf("rightThumb\n");}
		if(pressLeftShoulder()){printf("leftShoulder\n");}
		if(pressRightShoulder()){printf("rightShoulder\n");}*/
		if(pressA())
		{
			printf("A\n");
			setVibState("V_L20_R50");
		}
		if(pressB())
		{
			printf("B\n");
			setVibState("V_OFF");
		}
		/*if(pressX()){printf("X\n");}
		if(pressY()){printf("Y\n");}

		if(pressLeftTrigger())
		{
			printf("TriggerL: %f\n",GetLeftTriggerPos());
		}
		if(pressRightTrigger())
		{
			printf("TriggerR: %f\n",GetRightTriggerPos());
		}
		*/
		//*************Controller Demo**********************
	}
	else
	{
		//Controller is disconnected.
		printf("Controller is disconnected.\n");
	}
}

/*-----------------------------------------------------------------
 *
 * CHECKERS
 *
 *----------------------------------------------------------------*/

void Controller::checkLeftStick(XINPUT_STATE state)
{
	/*----------------
	 * Note:
	 * Thumb min: -32768
	 * Thumb max:  32767
	 *----------------*/

	//RAW controller data
	float f_leftThumbXValue = 0.0f;
	float f_leftThumbYValue = 0.0f;

	f_leftThumbXRaw = state.Gamepad.sThumbLX;
	f_leftThumbYRaw = state.Gamepad.sThumbLY;

	//Check if the controller is within the dead zone.
	//This prevents motion without user input.
	if( (state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
         state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
         state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
    {    
		state.Gamepad.sThumbLX = 0;
        state.Gamepad.sThumbLY = 0;
    }
	
	//Obtain X position (-1 to 1)
	f_leftThumbXValue = state.Gamepad.sThumbLX;
	if(f_leftThumbXValue)
	{
		f_leftThumbXPosition = (f_leftThumbXValue / 32768.0f);
		b_camMoveEn = true;
		b_leftStickMove = true;
	}

	//Obtain Y position (-1 to 1)
	f_leftThumbYValue = state.Gamepad.sThumbLY;
	if(f_leftThumbYValue)
	{
		f_leftThumbYPosition = (f_leftThumbYValue / 32768.0f);
		b_camMoveEn = true;
		b_leftStickMove = true;
	}
}

void Controller::checkRightStick(XINPUT_STATE state)
{
	/*----------------
	 * Note:
	 * Thumb min: -32768
	 * Thumb max:  32767
	 *----------------*/

	//RAW controller data
	float f_rightThumbXValue = 0.0f;
	float f_rightThumbYValue = 0.0f;
	
	f_rightThumbXRaw = state.Gamepad.sThumbRX;
	f_rightThumbYRaw = state.Gamepad.sThumbRY;


	//Check if the controller is within the dead zone.
	//This prevents motion without user input.
	if( (state.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
         state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
         state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
    {
		state.Gamepad.sThumbRX = 0;
		state.Gamepad.sThumbRY = 0;
    }

	//Obtain X position (-1 to 1)
	f_rightThumbXValue = state.Gamepad.sThumbRX;
	if(f_rightThumbXValue)
	{
		f_rightThumbXPosition = (f_rightThumbXValue / 32768.0f);
		b_rightStickMove = true;
	}

	//Obtain Y position (-1 to 1)
	f_rightThumbYValue = state.Gamepad.sThumbRY;
	if(f_rightThumbYValue)
	{
		f_rightThumbYPosition = (f_rightThumbYValue / 32768.0f);
		b_rightStickMove = true;
	}
}

void Controller::checkButtons(XINPUT_STATE state)
{
	if(state.Gamepad.wButtons)
	{
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			b_up = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			b_down = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			b_left = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			b_right = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
			b_start = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			b_back = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			b_leftThumb = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
			b_rightThumb = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			b_leftShoulder = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			b_rightShoulder = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			b_a = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			b_b = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
			b_x = true;
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			b_y = true;
	}
}

void Controller::checkTriggers(XINPUT_STATE state)
{
	if(state.Gamepad.bLeftTrigger && 
	   state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		b_leftTrigger = true;
		f_leftTriggerPosition = state.Gamepad.bLeftTrigger/255.0f;
	}

	if(state.Gamepad.bRightTrigger && 
	   state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		b_rightTrigger = true;
		f_rightTriggerPosition = state.Gamepad.bRightTrigger/255.0f;
	}
}
/*-----------------------------------------------------------------
 *
 * ACCESSORS
 *
 *----------------------------------------------------------------*/

 //GENERAL

bool Controller::CamCanMove(){return b_leftStickMove;}

 //THUMBSTICK

bool Controller::moveLeftStick(){return b_leftStickMove;}
bool Controller::moveRightStick(){return b_rightStickMove;}

D3DXVECTOR2 Controller::GetLeftThumbPos(){
	return D3DXVECTOR2(f_leftThumbXPosition, f_leftThumbYPosition);
}


D3DXVECTOR2 Controller::GetRightThumbPos(){
	return D3DXVECTOR2(f_rightThumbXPosition, f_rightThumbYPosition);
}

D3DXVECTOR2 Controller::GetLeftThumbRaw()
{return D3DXVECTOR2(f_leftThumbXRaw, f_leftThumbYRaw);}
D3DXVECTOR2 Controller::GetRightThumbRaw()
{return D3DXVECTOR2(f_rightThumbXRaw, f_rightThumbYRaw);}

float Controller::GetLeftStickMagnitude()
{
	D3DXVECTOR2 v_stick (f_leftThumbXPosition, f_leftThumbYPosition);
	return D3DXVec2Length(&v_stick);
}


float Controller::GetRightStickMagnitude()
{
	D3DXVECTOR2 v_stick (f_rightThumbXPosition, f_rightThumbYPosition);
	return D3DXVec2Length(&v_stick);
}


float Controller::GetLeftStickRadian()
{
	float x = f_leftThumbXPosition;
	float y = f_leftThumbYPosition;
	float radian = atanf(y/x);
	if(y >= 0 && x >= 0)
		radian += 3*D3DX_PI/2;
	else if(y >= 0 && x <= 0)
		radian += D3DX_PI/2;
	else if(y <= 0 && x <= 0)
		radian += D3DX_PI/2;
	else
		radian += 3*D3DX_PI/2;
	return radian;
}

float Controller::GetRightStickRadian()
{
	float x = f_rightThumbXPosition;
	float y = f_rightThumbYPosition;
	float radian = atanf(y/x);
	if(y >= 0 && x >= 0)
		radian += 3*D3DX_PI/2;
	else if(y >= 0 && x <= 0)
		radian += D3DX_PI/2;
	else if(y <= 0 && x <= 0)
		radian += D3DX_PI/2;
	else
		radian += 3*D3DX_PI/2;
	return radian;
}

 //BUTTON

bool Controller::pressUp(){return b_up;}
bool Controller::pressDown(){return b_down;}
bool Controller::pressLeft(){return b_left;}
bool Controller::pressRight(){return b_right;}
bool Controller::pressStart(){return b_start;}
bool Controller::pressBack(){return b_back;}
bool Controller::pressLeftThumb(){return b_leftThumb;}
bool Controller::pressRightThumb(){return b_rightThumb;}
bool Controller::pressLeftShoulder(){return b_leftShoulder;}
bool Controller::pressRightShoulder(){return b_rightShoulder;}
bool Controller::pressA(){return b_a;}
bool Controller::pressB(){return b_b;}
bool Controller::pressX(){return b_x;}
bool Controller::pressY(){return b_y;}

 //TRIGGER

bool Controller::pressLeftTrigger(){return b_leftTrigger;}
bool Controller::pressRightTrigger(){return b_rightTrigger;}
float Controller::GetLeftTriggerPos(){return f_leftTriggerPosition;}
float Controller::GetRightTriggerPos(){return f_rightTriggerPosition;}

/*-----------------------------------------------------------------
 *
 * VIBRATION
 *
 *----------------------------------------------------------------*/

void Controller::initVibrationStates()
{
	for (int i=0; i<3; i++)
	{
		insertVibState(config->stateNames.at(i), config->stateMin.at(i), config->stateMax.at(i));
	}
}

void Controller::insertVibState(char * c_stateName, int i_leftMSpeed, int i_rightMSpeed)
{
	if(i_leftMSpeed > 100){i_leftMSpeed = 100;}
	if(i_leftMSpeed < 0){i_leftMSpeed = 0;}
	if(i_rightMSpeed > 100){i_rightMSpeed = 100;}
	if(i_rightMSpeed < 0){i_rightMSpeed = 0;}

	XINPUT_VIBRATION * vibState = new XINPUT_VIBRATION();
	ZeroMemory( vibState, sizeof(XINPUT_VIBRATION) );
    vibState->wLeftMotorSpeed = (WORD)((((float)i_leftMSpeed)/100.0f)*65535.0f);      // use any value between 0-65535
    vibState->wRightMotorSpeed =(WORD)((((float)i_rightMSpeed)/100.0f)*65535.0f); // use any value between 0-65535
 
	VibStates[c_stateName] = vibState;
}

void Controller::setVibState(char * c_stateName)
{
	if(VibStates.find(c_stateName) != VibStates.end())
	{
		XInputSetState( 0, VibStates[c_stateName]);
	}
}