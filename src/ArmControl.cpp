/*
 * ArmControl.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Programming
 */

#include "ArmControl.h"

ArmControl::ArmControl(int horzontalTalonOne, int verticalCANTalon) {
	// TODO Auto-generated constructor stub
	m_pHorizontalMotor = new CANTalon(horzontalTalonOne);
	m_pVerticalMotor = new CANTalon(verticalCANTalon);
	m_pHorizontalMotor->SetSensorDirection(true);
	m_pVerticalMotor->SetSensorDirection(false);
	m_pVerticalMotor->ConfigFwdLimitSwitchNormallyOpen(true);
	m_pVerticalMotor->ConfigRevLimitSwitchNormallyOpen(true);
	//m_pVerticalMotor->ConfigSoftPositionLimits(1300, -2800);
	m_pHorizontalMotor->SetExpiration(2.0);
	m_pVerticalMotor->SetExpiration(2.0);
	m_pHorizontalMotor->SetSafetyEnabled(true);
	m_pVerticalMotor->SetSafetyEnabled(true);
	m_ArmState = ARM_IDLE;
	m_Index = 0;
}



void ArmControl::TestControlVert(VertDir dir)
{
	m_HorDir = HOR_IDLE;
	m_VertDir = dir;
	m_ArmState = ARM_MANUAL;
}


void ArmControl::TestControlHorz(HorDir dir)
{
	m_VertDir = VERT_IDLE;
	m_HorDir = dir;
	m_ArmState = ARM_MANUAL;
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


void ArmControl::ResetTalons(int horzontalTalonOne, int verticalCANTalon)
{
	m_pHorizontalMotor->SetPosition(0);
	m_pVerticalMotor->SetPosition(0);
}


void ArmControl::GrabBin()
{
	if (m_ArmState != ARM_IDLE)
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


	// Deal with manual arm moves
	if (m_ArmState == ARM_MANUAL)
	{
		switch (m_VertDir)
		{
		case VERT_IDLE:
		default:
			VertSpeed = 0.0;
			break;
		case VERT_UP:
			VertSpeed = c_VertMotorSpeedUp;
			break;
		case VERT_DN:
			VertSpeed = c_VertMotorSpeedDn;
			break;
		}

		switch (m_HorDir)
		{
		case HOR_IDLE:
		default:
			HorSpeed = 0.0;
			break;
		case HOR_OUT:
			HorSpeed = c_HorzMotorSpeedOut;
			break;
		case HOR_IN:
			HorSpeed = c_HorzMotorSpeedIn;
			break;
		}
		m_pHorizontalMotor->Set(HorSpeed, 0);
		m_pVerticalMotor->Set(VertSpeed, 0);
		return ARM_MANUAL;
	}


	// Handle the automatic bin grabbing
	switch (m_Index)
	{
		case 0:
		default:
			HorSpeed = VertSpeed = 0.0;
			break;
		case 1:
			// Move arm out to bin
			VertSpeed = c_VertMotorSpeedDn;
			HorSpeed = c_HorzMotorSpeedOut;
			if (VertPos > c_VertPos11)
				VertSpeed = 0.0;
			if (HorPos < c_HorPos11)
				m_Index++;
			break;
		case 2:
			if (HorPos <= c_HorPos13)
				VertSpeed = c_VertMotorSpeedUp;
			// Move arm out to bin
			HorSpeed = c_HorzMotorSpeedOut * 0.75;
			if (HorPos < c_HorPos1)
				m_Index++;
			break;
		case 3:
			// Move up to bin
			VertSpeed = c_VertMotorSpeedUp;
			HorSpeed = 0.0;//c_HorzMotorSpeedIn;
			if (VertPos < c_VertPos1)
				m_Index++;
			break;
		case 4:
			// Keep going up and back
			VertSpeed = c_VertMotorSpeedUp;
			HorSpeed = c_HorzMotorSpeedIn;
			if (VertPos < c_VertPos2)
				m_Index++;
			break;
		case 5:
			// Move down and continue back
			VertSpeed = 0.0; //c_VertMotorSpeedDn;
			HorSpeed = c_HorzMotorSpeedIn;
			if (HorPos > c_HorPos12)
				VertPos = c_VertMotorSpeedDn;
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
			{
				m_ArmState = ARM_FINISHING;
				m_Index++;
			}
			break;
		case 6:
			// Move down to drop bin
			VertSpeed = c_VertMotorSpeedDn;
			HorSpeed = 0.0;
			if (VertPos >= 100)
			{
				m_ArmState = ARM_FINISHING;
				m_Index++;
			}
			//if (VertPos > c_VertPos4)
			//{
			//	m_ArmState = ARM_FINISHING;
			//	m_Index++;
			//}
			break;
		case 7:
			// Move horizontal in
			VertSpeed = 0.0;
			HorSpeed = c_HorzMotorSpeedIn;
			if (HorPos > c_HorPos3)
				m_Index++;
			break;
		case 8:
			// Move up to 0
			VertSpeed = c_VertMotorSpeedUp;
			HorSpeed = 0.0;
			if (VertPos <= c_HomeVertPos)
			{
				m_Index = 0;
				m_ArmState = ARM_IDLE;
			}
			break;

	}

	m_pHorizontalMotor->Set(HorSpeed, 0);
	m_pVerticalMotor->Set(VertSpeed, 0);
	return m_ArmState;
}
