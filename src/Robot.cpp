#include "CTankDrive.h"
#include "WPILib.h"
#include "navX/IMU.h"
#include "Autonomous.h"
#include "ArmControl.h"
#include "LiftControl.h"
#include "XBoxController.h"
#include <string.h>

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
	bool m_PrevButState, m_PrevButState1;
	Compressor *m_pCompressor;
	char m_strDir[20];
	char m_strMode[20];
	char m_strFull[50];

	void RobotInit()
	{
		strcpy(m_strDir, "Forward\0");
		strcpy(m_strMode, "Grab all bins\0");
		m_PrevButState = false;
		m_PrevButState1 = false;
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
		m_pLiftControl->SetUpSolenoid(0);
		m_pCompressor = new Compressor(0); // <------------------------------FIND THE CORRECT CAN ID
		m_pCompressor->SetClosedLoopControl(true);
		m_pCompressor->Start();
		//m_pDrive->WatchdogOff();
		m_pDrive->WatchdogOn(2.0);
		m_firstIteration = true;
		CAutonomous::Setup(m_pDrive, m_pArmControl, m_pLiftControl);
		m_pDrive->initNavX();
	}


	void AutonomousInit()
	{
		switch (CAutonomous::GetMode())
		{
			case 0:
				CAutonomous::Index = 0;
				break;
			case 1:
				CAutonomous::Index = 2;
				break;
			case 2:
				CAutonomous::Index = 4;
				break;
			case 4:
				CAutonomous::Index = 6;
				break;
			case 3:
				CAutonomous::Index = 0;
				break;
		}

		m_pDrive->ZeroNavXYaw();
		m_pDrive->ResetEncoders();
		m_pArmControl->ResetTalons(1,2);
		if (CAutonomous::GetMode() != 3)
			m_pArmControl->GrabBin();
	}


	void TeleopInit()
	{
		m_pCompressor->Start();
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
		// Toggle Solenoid
		if (m_pStickC->GetRightBumper())
			m_pLiftControl->ToggleSolenoid(true);
		if (m_pStickC->GetLeftBumper())
			m_pLiftControl->ToggleSolenoid(false);

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

		if (m_pStickC->GetLeftTrigger() && !m_pStickC->GetRightTrigger())
		//if (m_pStickC->GetDPadDown() || m_pStickC->GetDPadUp() || m_pStickC->GetDPadLeft() || m_pStickC->GetDPadRight())
		{
			m_pArmControl->StopRake();
			pDS->ReportError("D Pad!");
		}
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
		SmartDashboard::PutNumber("Current Reading From one", m_pdp.GetCurrent(1));
	}


	void TestPeriodic()
	{
		lw->Run();
	}

	void DisabledInit()
	{
		strcpy(m_strFull, m_strDir);
		strcat(m_strFull, m_strMode);
		for (int x = 0; x < 20; x++)
			pDS->ReportError("");
		pDS->ReportError(m_strFull);
	}

	void DisabledPeriodic()
	{
		int x;
		bool CurButState = m_pStickR->GetRawButton(1);
		bool CurButState1 = m_pStickL->GetRawButton(1);


		//Direction selection
		if (CurButState1 && m_PrevButState1 == false)
		{
			m_PrevButState = true;
			switch (CAutonomous::GetDir())
			{
				case 0:
					CAutonomous::SetDir(1);
					strcpy(m_strDir, "Reverse, \0");
					//pDS->ReportError("Reverse");
					break;
				case 1:
					CAutonomous::SetDir(0);
					strcpy(m_strDir, "Forward, \0");
					//pDS->ReportError("Forward");
					break;
			}
			strcpy(m_strFull, m_strDir);
			strcat(m_strFull, m_strMode);
			for (x = 0; x < 20; x++)
				pDS->ReportError("");
			pDS->ReportError(m_strFull);
		}
		else if (CurButState1 == true)
			m_PrevButState1 = true;
		else
			m_PrevButState1 = false;

		// Autonomous selection
		if (CurButState && m_PrevButState == false)
		{
			m_PrevButState = true;
			switch (CAutonomous::GetMode())
			{
			case 0:
				CAutonomous::SetMode(1);
				strcpy(m_strMode, "Grab 3\0");
				//pDS->ReportError("Grab 3");
				break;
			case 1:
				CAutonomous::SetMode(2);
				strcpy(m_strMode, "Grab 2\0");
				//pDS->ReportError("Grab 2");
				break;
			case 2:
				CAutonomous::SetMode(3);
				strcpy(m_strMode, "Skip First Bin\0");
				//pDS->ReportError("Skip First Bin");
				break;
			case 3:
				CAutonomous::SetMode(4);
				strcpy(m_strMode, "Grab 1\0");
				//pDS->ReportError("Grab 1");
				break;
			case 4:
				CAutonomous::SetMode(0);
				strcpy(m_strMode, "Grab all bins\0");
				//pDS->ReportError("Grab all bins");
				break;
			}
			strcpy(m_strFull, m_strDir);
			strcat(m_strFull, m_strMode);
			for (x = 0; x < 20; x++)
				pDS->ReportError("");
			pDS->ReportError(m_strFull);

			/*
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
			*/
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













