/*
 * ArmControl.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Programming
 */

#ifndef ARMCONTROL_H_
#define ARMCONTROL_H_
#include "WPILib.h"

#define UINT32 uint32_t
#define INT32 int32_t


const int c_HomeHorzPos = 0;
const int c_HorPos1 = -6500;
const int c_HorPos2 = -500;//-2000;
const int c_HorPos3 = -500;
const int c_HorPos11 = -5000;
const int c_HorPos12 = -5000;
const int c_HorPos13 = -6500;

const int c_HomeVertPos = 350;
const int c_VertPos1 = -2000;
const int c_VertPos2 = -2700;
const int c_VertPos3 = -2000;
const int c_VertPos4 = 200;
const int c_VertPos11 = 500;

const float c_VertMotorSpeedUp = -1.00;
const float c_VertMotorSpeedDn = 1.00;
const float c_HorzMotorSpeedOut = 1.00;
const float c_HorzMotorSpeedIn = -0.75;

const int c_TestVertPos = 0;
const int c_TestHorzPos = 0;
const int c_HorzPlay = 0;


class ArmControl {
public:
	typedef enum State
	{
		ARM_IDLE,
		ARM_MOVING,
		ARM_FINISHING,
		ARM_MANUAL
	} ArmState;

	typedef enum VERTDIR
	{
		VERT_IDLE,
		VERT_UP,
		VERT_DN
	} VertDir;

	typedef enum HORDIR
	{
		HOR_IDLE,
		HOR_OUT,
		HOR_IN
	} HorDir;

	ArmState m_ArmState;
	VertDir m_VertDir;
	HorDir m_HorDir;
	CANTalon *m_pVerticalMotor;
	CANTalon *m_pHorizontalMotor;
	int m_CanCount;
	void init();


public:
	ArmControl(int horzontalTalonOne, int verticalCANTalon);
	virtual ~ArmControl();
	void TestControlVert(VertDir dir);
	void TestControlHorz(HorDir dir);
	void SetUpEncoders(UINT32 HorzEncA, UINT32 HorzEncB);
	void ResetTalons(int horzontalTalonOne, int verticalCANTalon);
	int GetEncVert();
	int GetEncHorz();
	void GrabBin();
	ArmState HandleStates();
	void StopRake();

private:
	int m_Index;
};

#endif /* ARMCONTROL_H_ */
