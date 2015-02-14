#include "CTankDrive.h"
#include "WPILib.h"
#include "navX/IMU.h"
#include "Autonomous.h"
#include "ArmControl.h"
#include "LiftControl.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
	CTankDrive *m_pDrive; // Drive system
	Joystick *m_pStickL;
	Joystick *m_pStickR;
	Joystick *m_pStickC;
	ArmControl *m_pArmControl;
	LiftControl *m_pLiftControl;
	bool m_firstIteration;
	PowerDistributionPanel m_pdp;
	INT32 m_LeftPos, m_RightPos;



	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		m_pStickL = new Joystick(0);
		m_pStickR = new Joystick(1);
		m_pStickC = new Joystick(3);
		m_pDrive = new CTankDrive(0, 1, 2, 3, m_pStickL, m_pStickR);
		m_pArmControl = new ArmControl(1 , 2);
		m_pDrive->SetupEncoders(0,1,2,3);
		m_pLiftControl = new LiftControl(4, 5, 6, 7);
		//m_pDrive->WatchdogOff();
		m_pDrive->WatchdogOn(2.0);
		//m_pArmControl->SetUpEncoders(4,5);
		m_firstIteration = true;
		CAutonomous::Setup(m_pDrive, m_pArmControl);
		m_pDrive->initNavX();
	}


	void AutonomousInit()
	{
		m_pDrive->ZeroNavXYaw();
		m_pDrive->ResetEncoders();
	}


	void TeleopInit()
	{

	}





	void AutonomousPeriodic()
	{
		CAutonomous::RunAuto();
	}


	void TeleopPeriodic()
	{
		m_pDrive->Go();


		if (true == m_pStickL->GetRawButton(6)){
			m_pArmControl->ResetTalons(1,2);
		}

		if (m_pStickL->GetRawButton(2))
			m_pArmControl->TestControlVert(ArmControl::VERT_UP);
		else if (m_pStickL->GetRawButton(3))
			m_pArmControl->TestControlVert(ArmControl::VERT_DN);
		else if (m_pStickL->GetRawButton(4))
			m_pArmControl->TestControlHorz(ArmControl::HOR_IN);
		else if (m_pStickL->GetRawButton(5))
			m_pArmControl->TestControlHorz(ArmControl::HOR_OUT);
		else
			m_pArmControl->m_ArmState = ArmControl::ARM_IDLE;

		if (m_pStickL->GetRawButton(8))
		{
			m_pArmControl->GrabBin();
		}
		if (m_pStickL->GetRawButton(9))
			m_pArmControl->HandleStates();
		else
		{
			m_pArmControl->m_pHorizontalMotor->Set(0, 0);
			m_pArmControl->m_pVerticalMotor->Set(0, 0);
		}

		if (m_pStickC->GetRawButton(8))
		{
			m_pLiftControl->GrabTote(m_pStickC->GetRawButton(4));
			m_pLiftControl->ReleaseTote(m_pStickC->GetRawButton(5));
			m_pLiftControl->PullInTote(m_pStickC->GetRawButton(1));
			m_pLiftControl->PushOutTote(m_pStickC->GetRawButton(2));

		}
		// Get/display drive encoder values
					//m_pDrive->GetPositions(&m_LeftPos, &m_RightPos);
					//SmartDashboard::PutNumber( "Left", m_LeftPos);
					//SmartDashboard::PutNumber( "Right", m_RightPos);

					// Get/display navX info
					/*SmartDashboard::PutBoolean( "IMU_Connected", m_pDrive.IsNavXConnected());
					SmartDashboard::PutNumber("IMU_Yaw", m_pDrive.GetNavXYaw());
					SmartDashboard::PutNumber("IMU_Pitch", m_pDrive.GetNavXPitch());
					SmartDashboard::PutNumber("IMU_Roll", m_pDrive.GetNavXRoll());
					SmartDashboard::PutNumber("IMU_CompassHeading", m_pDrive.GetNavXCompassHeading());
					SmartDashboard::PutNumber("IMU_Update_Count", m_pDrive.GetNavXUpdateCount());
					SmartDashboard::PutNumber("IMU_Byte_Count", m_pDrive.GetNavXByteCount());
					SmartDashboard::PutBoolean("IMU_IsCalibrating", m_pDrive.IsNavXCalibrating());

					// Get/display info from Power Distribution Panel
					SmartDashboard::PutNumber("Current Channel 1", m_pdp.GetCurrent(1));
					SmartDashboard::PutNumber("Voltage", m_pdp.GetVoltage());
					SmartDashboard::PutNumber("Temperature", m_pdp.GetTemperature());
			*/
		SmartDashboard::PutNumber("Arm Horizontal Encoder",m_pArmControl->GetEncHorz());
		SmartDashboard::PutNumber("Arm Vertical Encoder",m_pArmControl->GetEncVert());

	}


	void TestPeriodic()
	{
		lw->Run();
	}


	void DisabledPeriodic()
	{
		// Get/display drive encoders
		m_pDrive->GetPositions(&m_LeftPos, &m_RightPos);
		SmartDashboard::PutNumber( "Left", m_LeftPos);
		SmartDashboard::PutNumber( "Right", m_RightPos);

		// Get/display navX info
		SmartDashboard::PutBoolean( "IMU_Connected", m_pDrive->IsNavXConnected());
		SmartDashboard::PutNumber("IMU_Yaw", m_pDrive->GetNavXYaw());
	}


};

START_ROBOT_CLASS(Robot);













