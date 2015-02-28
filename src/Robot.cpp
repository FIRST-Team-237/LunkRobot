#include "CTankDrive.h"
#include "WPILib.h"
#include "navX/IMU.h"
#include "Autonomous.h"
#include "ArmControl.h"
#include "LiftControl.h"
#include "XBoxController.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
	CTankDrive *m_pDrive; // Drive system
	Joystick *m_pStickL;
	Joystick *m_pStickR;
	CXBoxController *m_pStickC;
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
		m_pStickC = new CXBoxController(2);
		m_pDrive = new CTankDrive(0, 1, 2, 3, m_pStickL, m_pStickR);
		m_pArmControl = new ArmControl(1 , 2);
		m_pDrive->SetupEncoders(0,1,2,3);
		m_pLiftControl = new LiftControl(4, 5, 6, 7, 8);
		m_pLiftControl->SetUpEncoder(4, 5);
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
		// Drivetrain
		m_pDrive->Go();

		// Reset encoders ---remove for competition
		if (true == m_pStickL->GetRawButton(6)){
			m_pArmControl->ResetTalons(1,2);
		}

		// Recycle bin grabber (rake)
		if (m_pStickC->GetLeftTrigger() && m_pStickC->GetRightTrigger())
		{
			if (m_pStickC->GetY())
				m_pArmControl->TestControlVert(ArmControl::VERT_UP);
			else if (m_pStickC->GetA())
				m_pArmControl->TestControlVert(ArmControl::VERT_DN);
			else if (m_pStickC->GetX())
				m_pArmControl->TestControlHorz(ArmControl::HOR_IN);
			else if (m_pStickC->GetB())
				m_pArmControl->TestControlHorz(ArmControl::HOR_OUT);
			else
				m_pArmControl->m_ArmState = ArmControl::ARM_IDLE;
		}
		else
			m_pArmControl->m_ArmState = ArmControl::ARM_IDLE;

		if (m_pStickC->GetDPadDown() || m_pStickC->GetDPadUp() || m_pStickC->GetDPadLeft() || m_pStickC->GetDPadRight())
				m_pArmControl->StopRake();
		m_pArmControl->HandleStates();

		// Elevator control
		m_pLiftControl->MoveElevator(m_pStickC->GetLeftY());
		//m_pLiftControl->MoveDown(m_pStickC->GetLeftYDownasButton());
		//m_pLiftControl->MoveUp(m_pStickC->GetLeftYUpasButton());
//		bool test = m_pStickC->GetLeftBumper();
//		if (test == true) {
//			m_pLiftControl->EnableDither();
//		}
//		if (m_pStickC->GetRightBumper()) {
//			m_pLiftControl->DisableDither();
//		}
//		m_pLiftControl->DitherElev();

		// Intake control
		if (m_pStickL->GetRawButton(1))
			m_pLiftControl->SetLeftIntake(c_suckSpeed);
		else if (m_pStickL->GetRawButton(2))
			m_pLiftControl->SetLeftIntake(c_spitOutSpeed);
		else
			m_pLiftControl->SetLeftIntake(0.0);

		if (m_pStickR->GetRawButton(1))
			m_pLiftControl->SetRightIntake(c_suckSpeed);
		else if (m_pStickR->GetRawButton(2))
			m_pLiftControl->SetRightIntake(c_spitOutSpeed);
		else
			m_pLiftControl->SetRightIntake(0.0);

		//m_pLiftControl->PullInTote(LiftControl::INTAKE_LEFT, m_pStickL->GetRawButton(1));
		//m_pLiftControl->PullInTote(LiftControl::INTAKE_RIGHT, m_pStickR->GetRawButton(1));

		//m_pLiftControl->PushOutTote(LiftControl::INTAKE_LEFT, m_pStickL->GetRawButton(2));
		//m_pLiftControl->PushOutTote(LiftControl::INTAKE_RIGHT, m_pStickR->GetRawButton(2));

		// Driver feedback
		m_pDrive->GetPositions(&m_LeftPos, &m_RightPos);
		SmartDashboard::PutNumber("Arm Horizontal Encoder",m_pArmControl->GetEncHorz());
		SmartDashboard::PutNumber("Arm Vertical Encoder",m_pArmControl->GetEncVert());
		SmartDashboard::PutNumber( "Left", m_LeftPos);
		SmartDashboard::PutNumber( "Right", m_RightPos);
		SmartDashboard::PutNumber("Elevator", m_pLiftControl->GetEncCount());
		//SmartDashboard::PutNumber("Elev 1",m_pdp.GetCurrent(2));
		//SmartDashboard::PutNumber("Elev 2",m_pdp.GetCurrent(13));
		SmartDashboard::PutNumber("Dither Target",m_pLiftControl->GetDitherTarget());
		SmartDashboard::PutBoolean("Dither Enable",m_pLiftControl->GetDitherEnabled());
	}


	void TestPeriodic()
	{
		lw->Run();
	}


	void DisabledPeriodic()
	{
		bool CurButState = m_pStickR->GetRawButton(1);

		// Autonomous selection
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

		SmartDashboard::PutNumber("Elevator", m_pLiftControl->GetEncCount());
	}


};

START_ROBOT_CLASS(Robot);













