/*
 * TankDrive.cpp
 *
 *  Created on: Jan 16, 2015
 *      Author: DanTest
 */

#include <CTankDrive.h>

CTankDrive::CTankDrive(int Left, int Right)
{
  m_pLRDrive = new Jaguar(Left);
  m_pRRDrive = new Jaguar(Right);
  m_pLFDrive = NULL;
  m_pRFDrive = NULL;
  m_pRobot = new RobotDrive(m_pLRDrive, m_pRRDrive);
  m_pLStick = NULL;
  m_pRStick = NULL;
}


CTankDrive::CTankDrive(int Left,int Right, Joystick *pLS, Joystick *pRS)
{
  m_pLRDrive = new Jaguar(Left);
  m_pLFDrive = NULL;
  m_pRRDrive = new Jaguar(Right);
  m_pRFDrive = NULL;
  m_pRobot = new RobotDrive(m_pLRDrive, m_pRRDrive);
  m_pLStick = pLS;
  m_pRStick = pRS;
  //initNavX();
}


CTankDrive::CTankDrive(int Left1, int Left2, int Right1, int Right2)
{
  m_pLRDrive = new Jaguar(Left1);
  m_pLFDrive = new Jaguar(Left2);
  m_pRRDrive = new Jaguar(Right1);
  m_pRFDrive = new Jaguar(Right2);
  m_pRobot = new RobotDrive(m_pLRDrive, m_pLFDrive, m_pRRDrive, m_pRFDrive);
  m_pLStick = NULL;
  m_pRStick = NULL;
  //initNavX();
}


CTankDrive::CTankDrive(int Left1, int Left2, int Right1, int Right2, Joystick *pLS, Joystick *pRS)
{
  m_pLRDrive = new Jaguar(Left1);
  m_pLFDrive = new Jaguar(Left2);
  m_pRRDrive = new Jaguar(Right1);
  m_pRFDrive = new Jaguar(Right2);
  m_pRobot = new RobotDrive(m_pLRDrive, m_pLFDrive, m_pRRDrive, m_pRFDrive);
  m_pLStick = pLS;
  m_pRStick = pRS;
  //initNavX();
}


CTankDrive::CTankDrive()
{

}


void CTankDrive::SetupEncoders(UINT32 LeftA, UINT32 LeftB, UINT32 RightA, UINT32 RightB)
{
  m_pLEnc = new Encoder( LeftA, LeftB, true, Encoder::k4X);
  m_pREnc = new Encoder( RightA, RightB, false, Encoder::k4X);
  m_pLEnc->Reset();
  m_pREnc->Reset();
}


void CTankDrive::ResetEncoders()
{
	if (m_pLEnc)
		m_pLEnc->Reset();
	if (m_pREnc)
		m_pREnc->Reset();
}


void CTankDrive::SetupGears(int HighSolenoid, int LowSolenoid)
{
  m_pHigh = new Solenoid(HighSolenoid);
  m_pLow = new Solenoid(LowSolenoid);
}


void CTankDrive::PrepforAuto()
{

}


void CTankDrive::Go()
{
  if (m_pLStick && m_pRStick)
  {
      m_pRobot->TankDrive(m_pLStick, m_pRStick);
      CheckGear();
  }
}


void CTankDrive::Go(Joystick *pL, Joystick *pR)
{
  m_pRobot->TankDrive(pL, pR);
  CheckGear();
}


void CTankDrive::Go(Joystick *pXBoxController, UINT32 LeftAxis, UINT32 RightAxis)
{
	double LeftSpeed, RightSpeed;

	LeftSpeed = DampenLeft(pXBoxController->GetRawAxis(LeftAxis));
	RightSpeed = DampenRight(pXBoxController->GetRawAxis(RightAxis));
	//SetMotorSpeeds(pXBoxController->GetRawAxis(LeftAxis), pXBoxController->GetRawAxis(RightAxis));
	SetMotorSpeeds(LeftSpeed, RightSpeed);
	CheckGear(pXBoxController);
}


void CTankDrive::GetPositions(INT32 *pLeft, INT32 *pRight)
{
  if (m_pLEnc && m_pREnc)
  {
      *pLeft = m_pLEnc->GetRaw();
      *pRight = m_pREnc->GetRaw();
  }
}


void CTankDrive::SetMotorSpeeds(double Left, double Right, bool UsePID)
{
  m_pLRDrive->Set(0.0 - Left, 1);
  if (m_pLFDrive)
    m_pLFDrive->Set(0.0 - Left, 1);
  m_pRRDrive->Set(Right, 1);
  if (m_pRFDrive)
    m_pRFDrive->Set(Right, 1);
}


void CTankDrive::SetGear(GearMode Gear)
{
  if (m_pHigh == NULL || m_pLow == NULL)
    return;

  switch (Gear)
  {
    case kHighSpeed:
      m_pHigh->Set(true);
      m_pLow->Set(false);
      break;
    case kLowSpeed:
      m_pHigh->Set(false);
      m_pLow->Set(true);
      break;
    default:
      m_pHigh->Set(false);
      m_pLow->Set(false);
      return;
  }
}


void CTankDrive::CheckGear()
{
  if (m_pLStick && m_pRStick && m_pHigh && m_pLow)
  {
      if (m_pRStick->GetRawButton(1))
        SetGear(kHighSpeed);
      else if (m_pRStick->GetRawButton(2))
        SetGear(kLowSpeed);
      else
        SetGear(kNone);
  }
}


void CTankDrive::CheckGear(Joystick *pStick)
{
  if (m_pHigh && m_pLow)
  {
      if (pStick->GetRawButton(5))
        SetGear(kHighSpeed);
      else if (pStick->GetRawButton(6))
        SetGear(kLowSpeed);
      else
        SetGear(kNone);
  }
}


void CTankDrive::SetBalanceGyro(UINT32 Channel)
{
  m_pBalGyro = new Gyro(Channel);
  m_pBalGyro->Reset();
}


void CTankDrive::AutoBalance()
{
/*
  float Angle = m_pBalGyro->GetAngle();
  float Speed;

  if (Angle > 0.0f && Angle < 10.0f)
    Speed = 0.5;
  this->SetMotorSpeeds(0.0f, 0.0f);
*/
}


double CTankDrive::DampenLeft(double LeftIn)
{
  static int BuffPos = 0;
  double TotalSpeeds = 0.0;
  int CurrentIndex;

  m_LeftBuff[BuffPos++] = LeftIn;
  if (BuffPos >= DAMP_BUF_SIZE)
	  BuffPos = 0;
  CurrentIndex = BuffPos;
  for (int x = 0; x < DAMP_BUF_SIZE; x++)
  {
      TotalSpeeds += (m_LeftBuff[CurrentIndex] * (double)(x + 1));
      if (++CurrentIndex >= DAMP_BUF_SIZE)
		  CurrentIndex = 0;
  }
  return (TotalSpeeds / (double)(((DAMP_BUF_SIZE * DAMP_BUF_SIZE) + DAMP_BUF_SIZE) /2));
}


double CTankDrive::DampenRight(double RightIn)
{
  static int BuffPos = 0;
  double TotalSpeeds = 0.0;
  int CurrentIndex;

  m_RightBuff[BuffPos++] = RightIn;
  if (BuffPos >= DAMP_BUF_SIZE)
	  BuffPos = 0;
  CurrentIndex = BuffPos;
  for (int x = 0; x < DAMP_BUF_SIZE; x++)
  {
      TotalSpeeds += (m_RightBuff[CurrentIndex] * (double)(x + 1));
      if (++CurrentIndex >= DAMP_BUF_SIZE)
		  CurrentIndex = 0;
  }
  return (TotalSpeeds / (double)(((DAMP_BUF_SIZE * DAMP_BUF_SIZE) + DAMP_BUF_SIZE) /2));
}


void CTankDrive::WatchdogOff()
{
	if (m_pLRDrive)
		m_pLRDrive->SetSafetyEnabled(false);
	if (m_pLFDrive)
		m_pLFDrive->SetSafetyEnabled(false);
	if (m_pRRDrive)
		m_pRRDrive->SetSafetyEnabled(false);
	if (m_pRFDrive)
		m_pRFDrive->SetSafetyEnabled(false);
}

void CTankDrive::WatchdogOn(float timeout)
{
	if (m_pLRDrive)
	{
		m_pLRDrive->SetExpiration(timeout);
		m_pLRDrive->SetSafetyEnabled(true);
	}
	if (m_pLFDrive)
	{
		m_pLFDrive->SetExpiration(timeout);
		m_pLFDrive->SetSafetyEnabled(true);
	}
	if (m_pRRDrive)
	{
		m_pRRDrive->SetExpiration(timeout);
		m_pRRDrive->SetSafetyEnabled(true);
	}
	if (m_pRFDrive)
	{
		m_pRFDrive->SetExpiration(timeout);
		m_pRFDrive->SetSafetyEnabled(true);
	}
}
// NavX Functions
void CTankDrive::initNavX()
{
	//m_pNetworkTable = NetworkTable::GetTable("datatable");
	m_pNavXPort = new SerialPort(57600,SerialPort::kMXP);
	m_navXUpdateRateHz = 50;
	m_pNavX = new IMU(m_pNavXPort,m_navXUpdateRateHz);
	if ( m_pNavX ) {
		//LiveWindow::GetInstance()->AddSensor("IMU","Gyro",m_pNavX);
	}
}
float CTankDrive::GetNavXYaw()
{
	return m_pNavX->GetYaw();
}
float CTankDrive::GetNavXRoll()
{
	return m_pNavX->GetRoll();
}
float CTankDrive::GetNavXPitch()
{
	return m_pNavX->GetPitch();
}
double CTankDrive::GetNavXByteCount()
{
	return m_pNavX->GetByteCount();
}
double CTankDrive::GetNavXUpdateCount()
{
	return m_pNavX->GetUpdateCount();
}
void CTankDrive::ResetNavX()
{
	m_pNavX->Restart();
}
void CTankDrive::ZeroNavXYaw()
{
	m_pNavX->ZeroYaw();
}
bool CTankDrive::IsNavXCalibrating()
{
	return m_pNavX->IsCalibrating();
}
bool CTankDrive::IsNavXConnected()
{
	return m_pNavX->IsConnected();
}
float CTankDrive::GetNavXCompassHeading()
{
	return m_pNavX->GetCompassHeading();
}
