/*
 * Autonomous.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Sean
 */

#include "Autonomous.h"

CTankDrive *CAutonomous::m_pDrive = NULL;
IMU *CAutonomous::m_pNavX = NULL;
const double DriveSpeed = 0.70;


void CAutonomous::Setup(CTankDrive *pDrive, IMU *pNavX)
{
	m_pDrive = pDrive;
	m_pNavX = pNavX;
}


void CAutonomous::RunAuto()
{
	static int Index = 0;
	static double LSpeed;
	static double RSpeed;
	double Error;
	float Yaw;
	int LeftPos, RightPos;
	static int DelteMeLater = 0;

	switch (Index)
	{
	case 0:
	case 2:
	case 4:
	case 6:
		LSpeed = RSpeed = 0.0;
		///
		// GRAB TOTE HERE
		if (++DelteMeLater > 200)
			Index++;
		//
		///
		m_pDrive->ResetEncoders();
		break;
	case 1:
	case 3:
	case 5:
		// Get info from sensors
		Yaw = m_pNavX->GetYaw();
		m_pDrive->GetPositions(&LeftPos, &RightPos);

		// Have we gone the required distance?
		if (LeftPos + RightPos >= 10000)
		{
			LSpeed = RSpeed = 0.0;
			Index++;
			DelteMeLater = 0;
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
		if(m_pNavX->GetYaw() > -90)
		{
			LSpeed = -0.70;
			RSpeed = 0.70;
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
		Yaw = m_pNavX->GetYaw();
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
			LSpeed -= Error;
			RSpeed += Error;
		}
		break;
	case 9:
	default:
		LSpeed = RSpeed = 0.0;
		break;
	}

	m_pDrive->SetMotorSpeeds(LSpeed, RSpeed);

	SmartDashboard::PutNumber( "Left", LeftPos);
	SmartDashboard::PutNumber( "Right", RightPos);
	SmartDashboard::PutNumber("IMU_Yaw", Yaw);

	// Wait for a motor update time
	Wait(0.005);
}

