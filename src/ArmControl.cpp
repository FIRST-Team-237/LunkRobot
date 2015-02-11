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
	m_CurHorState = HORIDLE;
	m_CurVertState = VERTIDLE;
}
void ArmControl::ExecuteStateAutonomous()
{
	switch (m_CurHorState)
	{
	case HORHOME:
		m_pHorizontalMotor->SetPosition(c_HomeHorzPos);
		break;
	case HORHALF:
		m_pHorizontalMotor->SetPosition(c_HalfHorzPos);
		break;
	case HORFULL:
		m_pHorizontalMotor->SetPosition(c_FullHorzPos);
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
		m_pVerticalMotor->SetPosition(c_ZeroVertPos);
		break;
	case VERTHALF:
		m_pVerticalMotor->SetPosition(c_HalfVertPos);
		break;
	case VERTFULL:
		m_pVerticalMotor->SetPosition(c_FullVertPos);
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
			m_pVerticalMotor->Set(c_VertMotorSpeed,0);
		} else {
			m_pVerticalMotor->Set(0 , 0);
		}
	} else if (false == dir) {
		if (true == pressed){
			m_pVerticalMotor->Set(-c_VertMotorSpeed,0);
		} else {
			m_pVerticalMotor->Set(0 , 0);
		}
	}
}
void ArmControl::TestControlHorz(bool pressed, bool dir)
{
/*	if (true == dir){
			if (true == pressed){
				m_pHorizontalMotor->Set(c_HorzMotorSpeedOut,0);
			} else {
				m_pHorizontalMotor->Set(0 , 0);
			}
		} else if (false == dir) {
			if (true == pressed){
				m_pHorizontalMotor->Set(-c_HorzMotorSpeedIn,0);
			} else {
				m_pHorizontalMotor->Set(0 , 0);
			}
		}*/
	if (true == dir){
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
	}
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

}
