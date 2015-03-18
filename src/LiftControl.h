/*
 * LiftControl.h
 *
 *  Created on: Feb 12, 2015
 *      Author: Programming
 */

#ifndef SRC_LIFTCONTROL_H_
#define SRC_LIFTCONTROL_H_
#include "WPILib.h"

const int c_maxEncVal = 0;
const int c_minEncVal = 0;
const int c_midEncVal = 0;

const float c_elevSpeed = -0.30; // UP
const float c_negElevSpeed = 0.50; //DOWN
const float c_ditherSpeed = -0.8;
const int c_HalfWindowSize = 100;

const float c_suckSpeed = -1.00;
const float c_spitOutSpeed = 0.50;
const float c_grabSpeed = 0;
const float c_releaseSpeed = 0;

const int c_MaxStopLimit = 100000;
const int c_MinStopLimit = -100000;

const int c_deadZone = 10;
class LiftControl {
public:
	LiftControl(int elevOne, int elevTwo, int intake, int binGraber, int intakeR);
	virtual ~LiftControl();
	typedef enum State
	{
		ELEV_IDLE,
		ELEV_MOVING_UP,
		ELEV_MOVING_DOWN,
		ELEV_MOVING
	} ElevState;
	typedef enum GraberState
	{
		GRAB_MOVING,
		GRAB_IDLE,
	} GrabState;
	typedef enum IntkState
	{
		INTAKE_OUT,
		INTAKE_IN,
		INTAKE_IDLE,
	} IntakeState;
	typedef enum ELEVPOS
	{
		START,
		MAXPOINT,
		MIDPOINT,
		MINPOINT,
		FREE
	} ElevPosition;
	typedef enum Sides
	{
		INTAKE_LEFT,
		INTAKE_RIGHT,
	} IntakeSide;

	void SetElevPosition(ElevPosition state);
	void MoveUp(bool moving);
	void MoveDown(bool moving);
	void MoveElevator(float speed);
	void GrabTote(bool grabing);
	void ReleaseTote(bool release);
	void PullInTote(IntakeSide side, bool moving);
	void PushOutTote(IntakeSide side, bool moving);

	void SetLeftIntake(float speed);
	void SetRightIntake(float speed);

	//ElevPosition GetElevPosition();
	void HandleStates();
	int GetEncCount();
	void ResetEnc();
	void SetUpEncoder(int channelA, int channelB);
	void SetUpSolenoid(uint8_t moduleNumber, uint32_t channel);
	void ToggleSolenoid(bool set);
	void EnableDither();
	void DitherElev();
	void DisableDither();
	int GetDitherTarget();
	bool GetDitherEnabled();
	bool GetSolenoid();
private:
	// Motors for elevator
	Talon *m_pElevOne;
	Talon *m_pElevTwo;
	Talon *m_pIntake;
	Talon *m_pIntakeR;
	Talon *m_pBinGraber;
	// Encoder for Elevator
	Encoder *m_pElevEnc;
	ElevPosition m_CurrentPosition;
	ElevState m_CurrentState;
	GrabState m_GraberState;
	IntakeState m_IntakeState;
	bool m_EnableDither;
	int m_DitherTarget;
	Solenoid *m_pSolenoid;

};

#endif /* SRC_LIFTCONTROL_H_ */
