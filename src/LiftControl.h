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

const int c_speedRS775 = 0;
const int c_speedCim = 0;
const int c_NegSpeedRS775 = 0;
const int c_NegSpeedCim = 0;

const int c_deadZone = 10;
class LiftControl {
public:
	LiftControl(int TalonOne, int TalonTwo, int TalonThree);
	virtual ~LiftControl();
	typedef enum State
	{
		ELEV_IDLE,
		ELEV_MOVING,
		ELEV_MANUAL
	} ElevState;
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
	//ElevPosition GetElevPosition();
	void HandleStates();
	int GetEncCount();
	void ResetEnc();
	void SetUpEncoder(int channelA, int channelB);
private:
	// Motors for elevator
	Talon *m_pElevOne;
	Talon *m_pElevTwo;
	Talon *m_pElevThree;
	// Encoder for Elevator
	Encoder *m_pElevEnc;
	ElevPosition m_CurrentPosition;
	ElevState m_CurrentState;
};

#endif /* SRC_LIFTCONTROL_H_ */
