#ifndef Controller_h
#define Controller_h
#include "dxstdafx.h"
#include <XInput.h>
#pragma comment (lib, "XInput.lib")
#include <map>
#include "./../ConfigManager/ConfigManager.h"

class Controller
{
	struct strCmp 
		{
			bool operator()( const char* s1, const char* s2 ) const 
			{
			return strcmp( s1, s2 ) < 0;
			}
		};
	public:
		Controller();					//Constructor
		void initButtonState();			//Reset the button flags
		void CheckControllerStatus();	//controller status call

	//CHECKERS
		void checkLeftStick(XINPUT_STATE state);
		void checkRightStick(XINPUT_STATE state);
		void checkButtons(XINPUT_STATE state);
		void checkTriggers(XINPUT_STATE state);

	//ACCESSORS
		//Camera

		bool CamCanMove();

		//Sticks		
		bool moveLeftStick();
		bool moveRightStick();

		D3DXVECTOR2 GetLeftThumbPos();	
		D3DXVECTOR2 GetRightThumbPos();

		D3DXVECTOR2 GetLeftThumbRaw();	
		D3DXVECTOR2 GetRightThumbRaw();

		float GetLeftStickMagnitude();	
		float GetRightStickMagnitude();

		float GetLeftStickRadian();
		float GetRightStickRadian();

		//Button

		bool pressUp();	
		bool pressDown();
		bool pressLeft();
		bool pressRight();
		bool pressStart();
		bool pressBack();
		bool pressLeftThumb();
		bool pressRightThumb();
		bool pressLeftShoulder();
		bool pressRightShoulder();
		bool pressA();
		bool pressB();
		bool pressX();
		bool pressY();

		//Trigger

		bool pressLeftTrigger();
		bool pressRightTrigger();

		float GetLeftTriggerPos();
		float GetRightTriggerPos();

	//VIBRATION
		
		void initVibrationStates();
		void insertVibState(char * c_stateName, int i_leftMSpeed, int i_rightMSpeed);
		void setVibState(char * c_stateName);

	//Config Manager
		ConfigManager *cManager;
		ControllerConfig *config;

	private:
		//Enables camera rotation
		bool b_camMoveEn;

		//Left stick
		bool b_leftStickMove;

		float f_leftThumbXPosition;
		float f_leftThumbYPosition;

		float f_leftThumbXRaw;
		float f_leftThumbYRaw;

		//Right stick
		bool b_rightStickMove;

		float f_rightThumbXPosition;
		float f_rightThumbYPosition;

		float f_rightThumbXRaw;
		float f_rightThumbYRaw;

		//buttons
		bool b_up;
		bool b_down;
		bool b_left;
		bool b_right;
		bool b_start;
		bool b_back;
		bool b_leftThumb;
		bool b_rightThumb;
		bool b_leftShoulder;
		bool b_rightShoulder;
		bool b_a;
		bool b_b;
		bool b_x;
		bool b_y;

		//triggers
		bool b_leftTrigger;
		bool b_rightTrigger;

		float f_leftTriggerPosition;
		float f_rightTriggerPosition;

		//vibration
		std::map<char *, XINPUT_VIBRATION *, strCmp> VibStates;


};

#endif