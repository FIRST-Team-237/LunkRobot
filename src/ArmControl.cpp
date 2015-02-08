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

}
void ArmControl::ExecuteStateAutonomous()
{
	switch (m_CurrentState)
	{
	case HOME:
		m_pHorizontalMotor->SetPosition(c_HomeHorzPos);
		m_pVerticalMotor->SetPosition(c_HomeVertPos);
		break;
	case HORZHALF:
		m_pHorizontalMotor->SetPosition(c_HalfHorzPos);
		break;
	case HORZFULL:
		m_pHorizontalMotor->SetPosition(c_FullHorzPos);
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
	case IDLE:
		break;
	case DONE:
		break;
	}
}
void ArmControl::SetUpEncoders(UINT32 HorzEncA, UINT32 HorzEncB)
{
	m_pHorzEnc = new Encoder(HorzEncA , HorzEncB , true, Encoder::k4X);
}
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
	if (true == dir){
			if (true == pressed){
				m_pHorizontalMotor->Set(c_HorzMotorSpeed,0);
			} else {
				m_pHorizontalMotor->Set(0 , 0);
			}
		} else if (false == dir) {
			if (true == pressed){
				m_pHorizontalMotor->Set(-c_HorzMotorSpeed,0);
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

