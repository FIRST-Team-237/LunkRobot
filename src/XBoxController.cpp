#include "XBoxController.h"





CXBoxController::CXBoxController(uint32_t port)
: Joystick(port)
{
}


/////////////
// Buttons //
/////////////
bool CXBoxController::GetA()
{
	return GetRawButton(1);
}


bool CXBoxController::GetB()
{
	return GetRawButton(2);
}


bool CXBoxController::GetX()
{
	return GetRawButton(3);
}


bool CXBoxController::GetY()
{
	return GetRawButton(4);
}


bool CXBoxController::GetStart()
{
	return GetRawButton(8);
}


bool CXBoxController::GetBack()
{
	return GetRawButton(7);
}


bool CXBoxController::GetLeftBumper()
{
	return GetRawButton(5);
}


bool CXBoxController::GetRightBumper()
{
	return GetRawButton(6);
}


/////////////////////
// Axes as buttons //
/////////////////////
bool CXBoxController::GetLeftTrigger()
{
	if (GetRawAxis(2) > 0.5f)
		return true;
	else
		return false;
}


bool CXBoxController::GetRightTrigger()
{
	if (GetRawAxis(3) > 0.5f)
		return true;
	else
		return false;
}


bool CXBoxController::GetDPadUp()
{
	if (GetRawAxis(7) > 0.5f)
		return true;
	else
		return false;
}


bool CXBoxController::GetDPadDown()
{
	if (GetRawAxis(7) < -0.5f)
		return true;
	else
		return false;
}


bool CXBoxController::GetDPadLeft()
{
	if (GetRawAxis(6) < -0.5f)
		return true;
	else
		return false;
}


bool CXBoxController::GetDPadRight()
{
	if (GetRawAxis(6) > 0.5f)
		return true;
	else
		return false;
}

bool CXBoxController::GetLeftYUpasButton()
{
	if (GetRawAxis(1) < -0.5f)
		return true;
	else
		return false;
}


bool CXBoxController::GetLeftYDownasButton()
{
	if (GetRawAxis(1) > 0.5f)
			return true;
		else
			return false;
}



//////////
// Axes //
//////////
float CXBoxController::GetLeftX()
{
	return GetRawAxis(1);
}


float CXBoxController::GetLeftY()
{
	return GetRawAxis(1);
}


float CXBoxController::GetRightX()
{
	return GetRawAxis(4);
}


float CXBoxController::GetRightY()
{
	return GetRawAxis(5);
}


float CXBoxController::GetTriggers()
{
	return GetRawAxis(3);
}


float CXBoxController::GetDPadX()
{
	return GetRawAxis(6);
}


float CXBoxController::GetDPadY()
{
	return GetRawAxis(7);
}




