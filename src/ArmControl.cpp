/*
 * ArmControl.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Programming
 */

#include "ArmControl.h"

ArmControl::ArmControl(int horzontalTalonOne, int verticalCANTalon) {
	// TODO Auto-generated constructor stub
	//m_pHorizontalMotorOne = new TalonSRX(horzontalTalonOne);
	m_pHorizontalMotor = new CANTalon(horzontalTalonOne);
	m_pVerticalMotor = new CANTalon(verticalCANTalon);
	m_pHorizontalMotor->SetSensorDirection(true);
	m_pVerticalMotor->SetSensorDirection(false);
	m_CurHorState = HORIDLE;
	m_CurVertState = VERTIDLE;
	m_Index = 0;
}
void ArmControl::ExecuteStateAutonomous()
{
	switch (m_CurHorState)
	{
	case HORHOME:
		m_pHorizontalMotor->SetPosition(c_HomeHorzPos);
		break;
	case HORHALF:
		//m_pHorizontalMotor->SetPosition(c_HalfHorzPos);
		break;
	case HORFULL:
		//m_pHorizontalMotor->SetPosition(c_FullHorzPos);
		break;
	case HORIDLE:
		break;
	case HORDONE:
		break;
	}

	switch (m_CurVertState)
	{
	case VERTHOME:
		m_pVerticalMotor->SetPosition(c_HomeVertPos);
		break;
	case VERTNONE:
		//m_pVerticalMotor->SetPosition(c_ZeroVertPos);
		break;
	case VERTHALF:
		//m_pVerticalMotor->SetPosition(c_HalfVertPos);
		break;
	case VERTFULL:
		//m_pVerticalMotor->SetPosition(c_FullVertPos);
		break;
	case VERTIDLE:
		break;
	case VERTDONE:
		break;
	}
}


//void ArmControl::SetUpEncoders(UINT32 HorzEncA, UINT32 HorzEncB)
//{
//	m_pHorzEnc = new Encoder(HorzEncA , HorzEncB , true, Encoder::k4X);
//}

void ArmControl::TestControlVert(bool pressed , bool dir)
{
	if (true == dir){
		if (true == pressed){
			m_pVerticalMotor->Set(c_VertMotorSpeedDn,0);
		} else {
			m_pVerticalMotor->Set(0 , 0);
		}
	} else if (false == dir) {
		if (true == pressed){
			m_pVerticalMotor->Set(c_VertMotorSpeedUp,0);
		} else {
			m_pVerticalMotor->Set(0 , 0);
		}
	}
}
void ArmControl::TestControlHorz(bool pressed, bool dir)
{
	if (true == dir){
			if (true == pressed){
				m_pHorizontalMotor->Set(c_HorzMotorSpeedOut,0);
			} else {
				m_pHorizontalMotor->Set(0 , 0);
			}
		} else if (false == dir) {
			if (true == pressed){
				m_pHorizontalMotor->Set(c_HorzMotorSpeedIn,0);
			} else {
				m_pHorizontalMotor->Set(0 , 0);
			}
		}
	/*if (true == dir){
		if (true == pressed)
		{
			if ( -8000 < m_pHorizontalMotor->GetEncPosition()){
				m_pHorizontalMotor->Set(c_HorzMotorSpeedOut,0);
			} else {
				m_pHorizontalMotor->Set(0,0);
			}
		} else {
			//m_pHorizontalMotor->Set(0,0);
		}
	} else if (false == dir) {
		if (true == pressed){
			m_pHorizontalMotor->Set(-c_HorzMotorSpeedIn,0);
		} else {
			m_pHorizontalMotor->Set(0 , 0);
		}
	}*/
}
int ArmControl::GetEncVert()
{
	return m_pVerticalMotor->GetEncPosition();
}
int ArmControl::GetEncHorz()
{
	return m_pHorizontalMotor->GetEncPosition();
}
ArmControl::~ArmControl() {
	// TODO Auto-generated destructor stub
}

void ArmControl::SetVertState(VertState State)
{
	m_CurVertState = State;
}

void ArmControl::SetHorState(HorState State)
{
	m_CurHorState = State;
}

ArmControl::VertState ArmControl::GetVertState()
{
	return m_CurVertState;
}

ArmControl::HorState ArmControl::GetHorState()
{
	return m_CurHorState;
}
void ArmControl::ResetTalons(int horzontalTalonOne, int verticalCANTalon)
{
/*	m_pHorizontalMotor->~CANTalon();
	m_pVerticalMotor->~CANTalon();
	m_pHorizontalMotor = new CANTalon(horzontalTalonOne);
	m_pVerticalMotor = new CANTalon(verticalCANTalon);
	m_pHorizontalMotor->SetSensorDirection(true);
	m_CurHorState = HORIDLE;
	m_CurVertState = VERTIDLE;
*/
	m_pHorizontalMotor->SetPosition(0);
	m_pVerticalMotor->SetPosition(0);

}


void ArmControl::GrabBin()
{
	if (m_ArmState == ARM_MOVING)
		return;
	m_ArmState = ARM_MOVING;
	m_Index = 1;
}


ArmControl::ArmState ArmControl::HandleStates()
{
	float HorSpeed, VertSpeed;
	bool VertDone = false;
	bool HorDone = false;
	int HorPos = m_pHorizontalMotor->GetEncPosition();
	int VertPos = m_pVerticalMotor->GetEncPosition();

	switch (m_Index)
	{
		case 0:
		default:
			HorSpeed = VertSpeed = 0.0;
			break;
		case 1:
			// Move arm out to bin
			HorSpeed = c_HorzMotorSpeedOut;
			if (HorPos < c_HorPos1)
				m_Index++;
			break;
		case 2:
			// Move up to bin and start back
			VertSpeed = c_VertMotorSpeedUp;
			HorSpeed = c_HorzMotorSpeedIn;
			if (VertPos < c_VertPos1)
				m_Index++;
			break;
		case 3:
			// Keep going up and back
			VertSpeed = c_VertMotorSpeedUp;
			HorSpeed = c_HorzMotorSpeedIn;
			if (VertPos < c_VertPos2)
				m_Index++;
			break;
		case 4:
			// Move down and continue back
			VertSpeed = c_VertMotorSpeedDn;
			HorSpeed = c_HorzMotorSpeedIn;
			if (VertPos > c_VertPos3)
			{
				VertSpeed = 0.0;
				VertDone = true;
			}
			if (HorPos > c_HorPos2)
			{
				HorSpeed = 0.0;
				HorDone = true;
			}
			if (HorDone == true && VertDone == true)
				m_Index++;
			break;
		case 5:
			// Move down to drop bin
			VertSpeed = c_VertMotorSpeedDn;
			HorSpeed = 0.0;
			if (VertPos > c_VertPos4)
				m_Index++;
			break;
		case 6:
			// Move horizontal in
			VertSpeed = 0.0;
			HorSpeed = c_HorzMotorSpeedIn;
			if (HorPos > c_HorPos3)
			{
				m_ArmState = ARM_IDLE;
				m_Index = 0;
			}
			break;
	}

	m_pHorizontalMotor->Set(HorSpeed, 0);
	m_pVerticalMotor->Set(VertSpeed, 0);
	return m_ArmState;
}
