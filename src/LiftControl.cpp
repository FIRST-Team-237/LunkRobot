/*
 * LiftControl.cpp
 *
 *  Created on: Feb 12, 2015
 *      Author: Programming
 */

#include <LiftControl.h>

LiftControl::LiftControl(int elevOne, int elevTwo, int intake, int binGraber) {
	// TODO Auto-generated constructor stub
	m_pElevOne = new Talon(elevOne);
	m_pElevTwo = new Talon(elevTwo);
	m_pIntake = new Talon(intake);
	m_pBinGraber = new Talon(binGraber);
	m_IntakeState = INTAKE_IDLE;
	m_GraberState = GRAB_IDLE;
	m_CurrentState = ELEV_IDLE;
	m_EnableDither = false;
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

	if (m_CurrentState == ELEV_IDLE || m_CurrentState == ELEV_MOVING_UP ){
		if (moving == true && m_pElevEnc->Get() <  c_MaxStopLimit){
				DisableDither();
				m_CurrentState = ELEV_MOVING_UP;
				m_pElevOne->SetSpeed(c_elevSpeed);
				m_pElevTwo->SetSpeed(c_elevSpeed);

			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);
				m_CurrentState = ELEV_IDLE;
			}
	} else {

	}

}

void LiftControl::MoveDown(bool moving)
{

	if (m_CurrentState == ELEV_IDLE || m_CurrentState == ELEV_MOVING_DOWN){
		if (moving == true && m_pElevEnc->Get() >  c_MinStopLimit ){
			DisableDither();
			m_CurrentState = ELEV_MOVING_DOWN;
			m_pElevOne->SetSpeed(c_negElevSpeed);
			m_pElevTwo->SetSpeed(c_negElevSpeed);

		} else {
			m_pElevOne->SetSpeed(0);
			m_pElevTwo->SetSpeed(0);
			m_CurrentState = ELEV_IDLE;
		}
	} else {

	}
}
void LiftControl::GrabTote(bool grabing)
{
	if (m_GraberState == GRAB_IDLE){
		m_GraberState = GRAB_MOVING;
		if (grabing == true){
			m_pBinGraber->SetSpeed(c_grabSpeed);
		} else {
			m_pBinGraber->SetSpeed(0);
			m_GraberState = GRAB_IDLE;
		}
	} else {

	}
}
void LiftControl::ReleaseTote(bool release)
{
	if (m_GraberState == GRAB_IDLE){
		m_GraberState = GRAB_MOVING;
		if (release == true){
			m_pBinGraber->SetSpeed(c_releaseSpeed);
		} else {
			m_pBinGraber->SetSpeed(0);
			m_GraberState = GRAB_IDLE;
		}
	} else {

	}
}
void LiftControl::PullInTote(bool moving)
{
	//if (m_IntakeState == INTAKE_IDLE){
		//m_IntakeState = INTAKE_MOVING;
		//if (moving == true)
		//	m_pIntake->Set(c_suckSpeed);
		//else
		//	m_pIntake->Set(0);
			//m_IntakeState = INTAKE_IDLE;
		//}
	//} else {

	//}
	if (m_IntakeState == INTAKE_IDLE && moving == true) {
			m_IntakeState = INTAKE_IN;
			m_pIntake->Set(c_suckSpeed);
		} else if (m_IntakeState == INTAKE_IN && moving == false) {
			m_pIntake->Set(0);
			m_IntakeState = INTAKE_IDLE;
		} else {

		}
}
void LiftControl::PushOutTote(bool moving)
{
	if (m_IntakeState == INTAKE_IDLE && moving == true) {
		m_IntakeState = INTAKE_OUT;
		m_pIntake->Set(c_spitOutSpeed);
	} else if (m_IntakeState == INTAKE_OUT && moving == false) {
		m_pIntake->Set(0);
		m_IntakeState = INTAKE_IDLE;
	} else {

	}
}
void LiftControl::EnableDither(){

	if (false == m_EnableDither ){
		m_DitherTarget = m_pElevEnc->GetRaw();
		m_EnableDither = true;
	} else {
		m_EnableDither = true;
	}
}
void LiftControl::DisableDither()
{
	m_EnableDither = false;
}
void LiftControl::DitherElev()
{
	if (true == m_EnableDither)
	{
		if ((m_pElevEnc->GetRaw() < (m_DitherTarget- c_HalfWindowSize))  ){
			m_pElevOne->SetSpeed(c_ditherSpeed);
			m_pElevTwo->SetSpeed(c_ditherSpeed);
		} else {
			m_pElevOne->SetSpeed(0);
			m_pElevTwo->SetSpeed(0);
		}
	}
}
int LiftControl::GetDitherTarget()
{
	return m_DitherTarget;
}
bool LiftControl::GetDitherEnabled()
{
	return m_EnableDither;
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
				m_pElevOne->SetSpeed(c_negElevSpeed);
				m_pElevTwo->SetSpeed(c_negElevSpeed);

			} else if (m_pElevEnc->GetRaw() < (c_minEncVal-c_deadZone)){
				m_pElevOne->SetSpeed(c_elevSpeed);
				m_pElevTwo->SetSpeed(c_elevSpeed);

			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);

				m_CurrentState = ELEV_IDLE;
			}
			break;
		case MAXPOINT:
			if (m_pElevEnc->GetRaw() < (c_maxEncVal)){
				m_pElevOne->SetSpeed(c_elevSpeed);
				m_pElevTwo->SetSpeed(c_elevSpeed);

			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);

				m_CurrentState = ELEV_IDLE;
			}
			break;
		case MIDPOINT:
			if (m_pElevEnc->GetRaw() > (c_midEncVal+c_deadZone)){
				m_pElevOne->SetSpeed(c_negElevSpeed);
				m_pElevTwo->SetSpeed(c_negElevSpeed);

			} else if (m_pElevEnc->GetRaw() < (c_midEncVal-c_deadZone)){
				m_pElevOne->SetSpeed(c_elevSpeed);
				m_pElevTwo->SetSpeed(c_elevSpeed);

			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);

				m_CurrentState = ELEV_IDLE;
			}
			break;
		case MINPOINT:
			if (m_pElevEnc->GetRaw() > c_minEncVal){
				m_pElevOne->SetSpeed(c_negElevSpeed);
				m_pElevTwo->SetSpeed(c_negElevSpeed);

			} else {
				m_pElevOne->SetSpeed(0);
				m_pElevTwo->SetSpeed(0);
				m_CurrentState = ELEV_IDLE;
			}
			break;
		case FREE:
			break;

		}
	}
}
