/*
 * Autonomous.h
 *
 *  Created on: Jan 25, 2015
 *      Author: Sean
 */

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_

#include "CTankDrive.h"
#include "WPILib.h"
#include "navX/IMU.h"
#include "ArmControl.h"

class CAutonomous
{
public:
	static void RunAuto();
	static void Setup(CTankDrive *pDrive, ArmControl *pArm);
	static void SetMode(int Mode);
	static int GetMode();

private:
	CAutonomous();
	~CAutonomous();

private:
	static CTankDrive *m_pDrive;
	static ArmControl *m_pArm;
	static IMU *m_pNavX;
	static int m_AutoMode;
};


#endif /* SRC_AUTONOMOUS_H_ */
