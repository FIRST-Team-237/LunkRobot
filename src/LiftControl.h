/*
 * LiftControl.h
 *
 *  Created on: Feb 12, 2015
 *      Author: Programming
 */

#ifndef SRC_LIFTCONTROL_H_
#define SRC_LIFTCONTROL_H_
#import "WPILib.h"

const int c_maxEncVal = 0;
const int c_minEncVal = 0;
const int c_midEncVal = 0;

const float c_elevSpeed = -1.0;
const float c_negElevSpeed = 1.0;

const float c_suckSpeed = -1.00;
const float c_spitOutSpeed = 1.00;
const float c_grabSpeed = 0;
const float c_releaseSpeed = 0;

const int c_deadZone = 10;
class LiftControl {
public:
	LiftControl(int elevOne, int elevTwo, int intake, int binGraber);
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
	void SetElevPosition(ElevPosition state);
	void MoveUp(bool moving);
	void MoveDown(bool moving);
	void GrabTote(bool grabing);
	void ReleaseTote(bool release);
	void PullInTote(bool moving);
	void PushOutTote(bool moving);
	//ElevPosition GetElevPosition();
	void HandleStates();
	int GetEncCount();
	void ResetEnc();
	void SetUpEncoder(int channelA, int channelB);
private:
	// Motors for elevator
	Talon *m_pElevOne;
	Talon *m_pElevTwo;
	Talon *m_pIntake;
	Talon *m_pBinGraber;
	// Encoder for Elevator
	Encoder *m_pElevEnc;
	ElevPosition m_CurrentPosition;
	ElevState m_CurrentState;
	GrabState m_GraberState;
	IntakeState m_IntakeState;
};

#endif /* SRC_LIFTCONTROL_H_ */
