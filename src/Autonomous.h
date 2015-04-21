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
#include "LiftControl.h"

class CAutonomous
{
public:
	static void RunAuto();
	static void RunAutoFwd();
	static void RunAutoRev();
	static void Setup(CTankDrive *pDrive, ArmControl *pArm, LiftControl *pLift);
	static void SetMode(int Mode);
	static int GetMode();
	static void SetDir(int Dir);
	static int GetDir();
	static int Index;
private:
	CAutonomous();
	~CAutonomous();

private:
	static CTankDrive *m_pDrive;
	static ArmControl *m_pArm;
	static LiftControl *m_pLift;
	static IMU *m_pNavX;
	static int m_AutoMode;
	static int m_AutoDir;
};


#endif /* SRC_AUTONOMOUS_H_ */
