/*
 * Autonomous.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Sean
 */

#include "Autonomous.h"

CTankDrive *CAutonomous::m_pDrive = NULL;
ArmControl *CAutonomous::m_pArm = NULL;
LiftControl *CAutonomous::m_pLift = NULL;
IMU *CAutonomous::m_pNavX = NULL;
int CAutonomous::m_AutoMode = 0;
int CAutonomous::Index = 0;

const double DriveSpeed = 0.98;
const float m_adjustDegree = 0.0;

void CAutonomous::Setup(CTankDrive *pDrive, ArmControl *pArm, LiftControl *pLift)
{
	m_pLift = pLift;
	m_pDrive = pDrive;
	m_pArm = pArm;
}


void CAutonomous::SetMode(int Mode)
{
	m_AutoMode = Mode;
}

int CAutonomous::GetMode()
{
	return m_AutoMode;
}

void CAutonomous::RunAuto()
{
	static double LSpeed = 0.0;
	static double RSpeed = 0.0;
	double Error;
	float Yaw = 0.0;
	int LeftPos, RightPos;
	int Distance;
	ArmControl::ArmState GrabState;

	GrabState = m_pArm->HandleStates();
	//m_pLift->MoveAuto();

	switch (Index)
	{
	case 0:
	case 2:
	case 4:
	case 6:
		LSpeed = RSpeed = 0.0;
		if (GrabState == ArmControl::ARM_IDLE || GrabState == ArmControl::ARM_FINISHING)
			Index++;
		else
			m_pDrive->ResetEncoders();
		break;
	case 1:
	case 3:
	case 5:
		if (Index == 3)
			Distance = 7200; //8250;
		else if (Index == 5)
			Distance = 4600;
		else
			Distance = 4800; //5250;
		// Get info from sensors
		Yaw = m_pDrive->GetNavXYaw() + m_adjustDegree;
		m_pDrive->GetPositions(&LeftPos, &RightPos);

		// Have we gone the required distance?
		if (LeftPos >= Distance || RightPos >= Distance)
		{
			LSpeed = RSpeed = 0.0;
			Index++;
			m_pArm->GrabBin();
		}
		// Correct for yaw because Joe designed a shitty drive system
		else
		{
			LSpeed = RSpeed = DriveSpeed;
			Error = (double)Yaw / 20;
			LSpeed -= Error/2;
			RSpeed += Error/2;
		}
		break;
	case 7:
		// Turn 90 degrees
		if(m_pDrive->GetNavXYaw() > -90)
		{
			// set back to 0.70 and -0.70
			LSpeed = -0.00;
			RSpeed = 0.00;
		}
		else
		{
			LSpeed = RSpeed = 0.0;
			m_pDrive->ResetEncoders();
			Index++;
		}
		break;
	case 8:
		// Get info from sensors
		Yaw = m_pDrive->GetNavXYaw();
		m_pDrive->GetPositions(&LeftPos, &RightPos);

		// Have we gone the required distance?
		if (LeftPos + RightPos >= 20000)
		{
			LSpeed = RSpeed = 0.0;
			Index++;
		}
		// Correct for yaw because Joe's drive system is still shitty
		else
		{
			LSpeed = RSpeed = 0.0 - DriveSpeed;
			Error = (double)(Yaw + 90) / 20;
			LSpeed = 0;
			RSpeed = 0;
		}
		break;
	case 9:
	default:
		LSpeed = RSpeed = 0.0;
		break;
	}

	if (LSpeed == 0)
		LSpeed = 0.02;
	m_pDrive->SetMotorSpeeds(LSpeed-0.02, RSpeed);

	//SmartDashboard::PutNumber( "Left", LeftPos);
	//SmartDashboard::PutNumber( "Right", RightPos);
	//SmartDashboard::PutNumber("IMU_Yaw", Yaw);
}

