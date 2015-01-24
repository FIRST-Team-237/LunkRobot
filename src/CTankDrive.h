/*
 * TankDrive.h
 *
 *  Created on: Jan 16, 2015
 *      Author: DanTest
 */
#include "WPILib.h"
#include <stdint.h>
#ifndef SRC_CTANKDRIVE_H_
#define SRC_CTANKDRIVE_H_
#define DAMP_BUF_SIZE	20
#define UINT32 uint32_t
#define INT32 int32_t
class CTankDrive {
public:
  typedef enum {kHighSpeed, kLowSpeed, kNone} GearMode;
  CTankDrive(int Left, int Right);
  CTankDrive(int Left, int Right,  Joystick *pLS, Joystick *pRS);
  CTankDrive(int Left1, int Left2, int Right1, int Right2);
  CTankDrive(int Left1, int Left2, int Right1, int Right2, Joystick *pLS, Joystick *pRS);
  void SetupGears(int HighSolenoid, int LowSolenoid);
  void SetupEncoders(UINT32 LeftA, UINT32 LeftB, UINT32 RightA, UINT32 RightB);
  void SetBalanceGyro(UINT32 Channel);
  void SetMotorSpeeds(double Left, double Right);
  void GetPositions(INT32 *pLeft, INT32 *pRight);
  void Go();
  void Go(Joystick *pL, Joystick *pR);
  void Go(Joystick *pXBoxController, UINT32 LeftAxis, UINT32 RightAxis);
  void PrepforAuto();
  void SetGear(GearMode Gear);
  void AutoBalance();
  void WatchdogOff();
  void WatchdogOn(float timeout);

private:
  CTankDrive();
  void CheckGear();
  void CheckGear(Joystick *pStick);
  double DampenLeft(double LeftIn);
  double DampenRight(double RightIn);

private:
  Jaguar *m_pLRDrive;
  Jaguar *m_pLFDrive;
  Jaguar *m_pRRDrive;
  Jaguar *m_pRFDrive;
  Solenoid *m_pHigh;
  Solenoid *m_pLow;
  Encoder *m_pLEnc;
  Encoder *m_pREnc;
  Joystick *m_pLStick;
  Joystick *m_pRStick;
  RobotDrive *m_pRobot;
  Gyro* m_pBalGyro;
  double m_LeftBuff[DAMP_BUF_SIZE];
  double m_RightBuff[DAMP_BUF_SIZE];
};

#endif /* SRC_TANKDRIVE_H_ */
