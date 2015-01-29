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


class CAutonomous
{
public:
	static void RunAuto();
	static void Setup(CTankDrive *pDrive, IMU *pNavX);

private:
	CAutonomous();
	~CAutonomous();

private:
	static CTankDrive *m_pDrive;
	static IMU *m_pNavX;
};


#endif /* SRC_AUTONOMOUS_H_ */
