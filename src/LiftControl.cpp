/*
 * LiftControl.cpp
 *
 *  Created on: Feb 12, 2015
 *      Author: Programming
 */

#include <LiftControl.h>

LiftControl::LiftControl(int TalonOne, int TalonTwo, int TalonThree) {
	// TODO Auto-generated constructor stub
	m_pElevOne = new Talon(TalonOne);
	m_pElevTwo = new Talon(TalonTwo);
	m_pElevThree = new Talon(TalonThree);;
}

LiftControl::~LiftControl() {
	// TODO Auto-generated destructor stub
}
void LiftControl::SetUpEncoder(int channelA, int channelB) {
	m_pElevEnc = new Encoder(channelA, channelB, true, Encoder::k4X);
}
void LiftControl::ResetEnc()
{
	m_pElevEnc->Reset();
}
int LiftControl::GetEncCount()
{
	return m_pElevEnc->GetRaw();
}
//set the position of elev
void LiftControl::SetElevPosition(ElevPosition state)
{
	if (m_CurrentState != ELEV_IDLE){

	} else {
		m_CurrentPosition = state;
	}
}

/*ElevPosition LiftControl::GetElevPosition()
{
	return m_CurrentState;
}*/

void LiftControl::MoveUp(bool moving)
{
	if (m_CurrentState == ELEV_MOVING){

	} else {
		if (moving == true){
			m_CurrentState = ELEV_MANUAL;
			m_pElevOne->SetSpeed(c_speedCim);
			m_pElevTwo->SetSpeed(c_speedRS775);
			m_pElevThree->SetSpeed(c_speedRS775);
		} else {
			m_pElevOne->SetSpeed(0);
			m_pElevTwo->SetSpeed(0);
			m_pElevThree->SetSpeed(0);
			m_CurrentState = ELEV_IDLE;
		}
	}

}

void LiftControl::MoveDown(bool moving)
{
	if (m_CurrentState == ELEV_MOVING){

	} else {
		if (moving == true){
			m_CurrentState = ELEV_MANUAL;
			m_pElevOne->SetSpeed(c_NegSpeedCim);
			m_pElevTwo->SetSpeed(c_NegSpeedRS775);
				m_pElevThree->SetSpeed(c_NegSpeedRS775);
		} else {
			m_pElevOne->SetSpeed(0);
			m_pElevTwo->SetSpeed(0);
			m_pElevThree->SetSpeed(0);
			m_CurrentState = ELEV_IDLE;
		}
	}
}

void LiftControl::HandleStates()
{
	if (m_CurrentState != ELEV_IDLE) {

	} else {
		m_CurrentState = ELEV_MOVING;

		switch(m_CurrentPosition)
		{
		case START:
			if (m_pElevEnc->GetRaw() > (c_minEncVal+c_deadZone)){
				m_pElevOne->SetSpeed(c_NegSpeedCim);
				m_pElevTwo->SetSpeed(c_NegSpeedRS775);
				m_pElevThree->SetSpeed(c_NegSpeedRS775);
			} else if (m_pElevEnc->GetRaw() < (c_minEncVal-c_deadZone)){
				m_pElevOne->SetSpeed(c_speedCim);
				m_pElevTwo->SetSpeed(c_speedRS775);
				m_pElevThree->SetSpeed(c_speedRS775);
			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);
				m_pElevThree->SetSpeed(0);
				m_CurrentState = ELEV_IDLE;
			}
			break;
		case MAXPOINT:
			if (m_pElevEnc->GetRaw() < (c_maxEncVal)){
				m_pElevOne->SetSpeed(c_speedCim);
				m_pElevTwo->SetSpeed(c_speedRS775);
				m_pElevThree->SetSpeed(c_speedRS775);
			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);
				m_pElevThree->SetSpeed(0);
				m_CurrentState = ELEV_IDLE;
			}
			break;
		case MIDPOINT:
			if (m_pElevEnc->GetRaw() > (c_midEncVal+c_deadZone)){
				m_pElevOne->SetSpeed(c_NegSpeedCim);
				m_pElevTwo->SetSpeed(c_NegSpeedRS775);
				m_pElevThree->SetSpeed(c_NegSpeedRS775);
			} else if (m_pElevEnc->GetRaw() < (c_midEncVal-c_deadZone)){
				m_pElevOne->SetSpeed(c_speedCim);
				m_pElevTwo->SetSpeed(c_speedRS775);
				m_pElevThree->SetSpeed(c_speedRS775);
			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);
				m_pElevThree->SetSpeed(0);
				m_CurrentState = ELEV_IDLE;
			}
			break;
		case MINPOINT:
			if (m_pElevEnc->GetRaw() > c_minEncVal){
				m_pElevOne->SetSpeed(c_NegSpeedCim);
				m_pElevTwo->SetSpeed(c_NegSpeedRS775);
				m_pElevThree->SetSpeed(c_NegSpeedRS775);
			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);
				m_pElevThree->SetSpeed(0);
				m_CurrentState = ELEV_IDLE;
			}
			break;
		case FREE:
			break;

		}
	}
}
