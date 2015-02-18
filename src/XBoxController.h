/*----------------------------------------------------------------------------*/
/* Copyright (c) Team 237 Black Magic. All Rights Reserved.					  */
/* Open Source Software - may be modified and shared by FRC teams.            */
/*----------------------------------------------------------------------------*/

#ifndef XBOXCONTROLLER_H_
#define XBOXCONTROLLER_H_

#include "Joystick.h"



class CXBoxController: public Joystick
{
public:
	explicit CXBoxController(uint32_t port);

	// Get Button Values
	bool GetA();
	bool GetB();
	bool GetX();
	bool GetY();
	bool GetStart();
	bool GetBack();
	bool GetLeftBumper();
	bool GetRightBumper();

	// Get Joystick Values
	float GetLeftX();
	float GetLeftY();
	float GetRightX();
	float GetRightY();
	float GetTriggers();
	float GetDPadX();
	float GetDPadY();
    
	// Get certain axes as buttons
	bool GetLeftTrigger();
	bool GetRightTrigger();
	bool GetDPadUp();
	bool GetDPadDown();
	bool GetDPadLeft();
	bool GetDPadRight();
	bool GetLeftYUpasButton();
	bool GetLeftYDownasButton();

};

#endif
