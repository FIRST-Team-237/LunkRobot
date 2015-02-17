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
	DriverStation *pDS;
	bool m_PrevButState;

	void RobotInit()
	{
		m_PrevButState = false;
		pDS = DriverStation::GetInstance();
		lw = LiveWindow::GetInstance();
		m_pStickL = new Joystick(0);
		m_pStickR = new Joystick(1);
		m_pStickC = new Joystick(2);
		m_pDrive = new CTankDrive(0, 1, 2, 3, m_pStickL, m_pStickR);
		m_pArmControl = new ArmControl(1 , 2);
		m_pDrive->SetupEncoders(0,1,2,3);
		m_pLiftControl = new LiftControl(4, 5, 6, 7);
		//m_pDrive->WatchdogOff();
		m_pDrive->WatchdogOn(2.0);
		m_firstIteration = true;
		CAutonomous::Setup(m_pDrive, m_pArmControl);
		m_pDrive->initNavX();
	}


	void AutonomousInit()
	{
		m_pDrive->ZeroNavXYaw();
		m_pDrive->ResetEncoders();
		m_pArmControl->ResetTalons(1,2);
		if (CAutonomous::GetMode() == 0)
			m_pArmControl->GrabBin();
	}


	void TeleopInit()
	{
		m_pArmControl->m_ArmState = ArmControl::ARM_IDLE;
	}





	void AutonomousPeriodic()
	{
		CAutonomous::RunAuto();
		m_pDrive->GetPositions(&m_LeftPos, &m_RightPos);
		SmartDashboard::PutNumber("IMU_Yaw", m_pDrive->GetNavXYaw());
		SmartDashboard::PutNumber( "Left", m_LeftPos);
		SmartDashboard::PutNumber( "Right", m_RightPos);
		SmartDashboard::PutNumber("Arm Horizontal Encoder",m_pArmControl->GetEncHorz());
		SmartDashboard::PutNumber("Arm Vertical Encoder",m_pArmControl->GetEncVert());
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

		//if (m_pStickL->GetRawButton(8))
		//{
		//	m_pArmControl->GrabBin();
		//}
		//if (m_pStickL->GetRawButton(9))
			m_pArmControl->HandleStates();
		//else
		//{
		//	m_pArmControl->m_pHorizontalMotor->Set(0, 0);
		//	m_pArmControl->m_pVerticalMotor->Set(0, 0);
		//}

		m_pLiftControl->MoveDown(m_pStickC->GetRawButton(1));
		m_pLiftControl->MoveUp(m_pStickC->GetRawButton(2));
		m_pLiftControl->PullInTote(m_pStickR->GetRawButton(1));
		m_pLiftControl->PushOutTote(m_pStickR->GetRawButton(2));

		m_pDrive->GetPositions(&m_LeftPos, &m_RightPos);
		SmartDashboard::PutNumber("Arm Horizontal Encoder",m_pArmControl->GetEncHorz());
		SmartDashboard::PutNumber("Arm Vertical Encoder",m_pArmControl->GetEncVert());
		SmartDashboard::PutNumber( "Left", m_LeftPos);
		SmartDashboard::PutNumber( "Right", m_RightPos);
	}


	void TestPeriodic()
	{
		lw->Run();
	}


	void DisabledPeriodic()
	{
		bool CurButState = m_pStickR->GetRawButton(1);

		if (CurButState && m_PrevButState == false)
		{
			m_PrevButState = true;
			if (CAutonomous::GetMode() == 0)
			{
				CAutonomous::SetMode(1);
				pDS->ReportError("Skip first bin");
			}
			else
			{
				CAutonomous::SetMode(0);
				pDS->ReportError("Grab all bins");
			}
		}
		else if (CurButState == true)
			m_PrevButState = true;
		else
			m_PrevButState = false;

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













