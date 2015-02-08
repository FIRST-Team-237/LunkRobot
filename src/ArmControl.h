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
const float c_VertMotorSpeed = 0.75;
const float c_HorzMotorSpeed = 0.90;
const int c_TestVertPos = 0;
const int c_HorzPlay = 0;

class ArmControl {
private:
	enum ArmState
	{
		HOME,
		HORZHALF,
		HORZFULL,
		VERTNONE,
		VERTHALF,
		VERTFULL,
		IDLE,
		DONE
	};
	ArmState m_CurrentState;
	ArmState m_PrevState;
	Encoder *m_pHorzEnc;

	//TalonSRX *m_pHorizontalMotorOne;
	CANTalon *m_pVerticalMotor;
	CANTalon *m_pHorizontalMotor;
	int m_CanCount;
	void init();
	void ExecuteStateAutonomous();

public:
	ArmControl(int horzontalTalonOne, int verticalCANTalon);
	virtual ~ArmControl();
	void TestControlVert(bool pressed , bool dir);
	void TestControlHorz(bool pressed , bool dir);
	void SetUpEncoders(UINT32 HorzEncA, UINT32 HorzEncB);
	int GetEncVert();
	int GetEncHorz();
};

#endif /* ARMCONTROL_H_ */
