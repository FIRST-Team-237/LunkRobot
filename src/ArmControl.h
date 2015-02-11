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

const int c_HomeVertPos = 0;
const int c_HomeHorzPos = 0;
const int c_IdlePos = 0;
const int c_FullHorzPos = 0;
const int c_HalfHorzPos = 0;
const int c_FullVertPos = 0;
const int c_HalfVertPos = 0;
const int c_ZeroVertPos = 0;
const float c_VertMotorSpeed = 0.50;
const float c_HorzMotorSpeedOut = 0.50;
const float c_HorzMotorSpeedIn = 1.00;
const int c_TestVertPos = 0;
const int c_TestHorzPos = 0;
const int c_HorzPlay = 0;

class ArmControl {
public:
	typedef enum VState
	{
		VERTHOME,
		VERTNONE,
		VERTHALF,
		VERTFULL,
		VERTIDLE,
		VERTDONE
	} VertState;

	typedef enum HState
	{
		HORHOME,
		HORHALF,
		HORFULL,
		HORIDLE,
		HORDONE
	} HorState;

	VertState m_CurVertState;
	VertState m_PrevVertState;
	HorState m_CurHorState;
	HorState m_PrevHorState;
	//Encoder *m_pHorzEnc;

	//TalonSRX *m_pHorizontalMotorOne;
	CANTalon *m_pVerticalMotor;
	CANTalon *m_pHorizontalMotor;
	int m_CanCount;
	void init();


public:
	ArmControl(int horzontalTalonOne, int verticalCANTalon);
	virtual ~ArmControl();
	void TestControlVert(bool pressed , bool dir);
	void TestControlHorz(bool pressed , bool dir);
	void SetUpEncoders(UINT32 HorzEncA, UINT32 HorzEncB);
	void SetVertState(VertState State);
	void SetHorState(HorState State);
	void ExecuteStateAutonomous();
	void ResetTalons(int horzontalTalonOne, int verticalCANTalon);
	int GetEncVert();
	int GetEncHorz();
	VertState GetVertState();
	HorState GetHorState();

};

#endif /* ARMCONTROL_H_ */
