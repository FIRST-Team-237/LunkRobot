#include "CTankDrive.h"
#include "WPILib.h"
#include "navX/IMU.h"
#include "Autonomous.h"
#include "ArmControl.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
	CTankDrive *m_pDrive; // Drive system
	Joystick *m_pStickL;
	Joystick *m_pStickR;
	ArmControl *m_pArmControl;
	bool m_firstIteration;
	PowerDistributionPanel m_pdp;
	INT32 m_LeftPos, m_RightPos;



	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		m_pStickL = new Joystick(0);
		m_pStickR = new Joystick(1);
		m_pDrive = new CTankDrive(0, 1, 2, 3, m_pStickL, m_pStickR);
		m_pArmControl = new ArmControl(1 , 2);
		m_pDrive->SetupEncoders(0,1,2,3);
		//m_pDrive->WatchdogOff();
		m_pDrive->WatchdogOn(2.0);
		m_pArmControl->SetUpEncoders(4,5);
		m_firstIteration = true;
		CAutonomous::Setup(m_pDrive);
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
		if (m_pStickL->GetRawButton(2)){
			m_pArmControl->TestControlVert(m_pStickL->GetRawButton(2),false);
		} else if (m_pStickL->GetRawButton(3)){
			m_pArmControl->TestControlVert(m_pStickL->GetRawButton(3),true);
		} else {
			m_pArmControl->TestControlVert(m_pStickL->GetRawButton(2),false);
			m_pArmControl->TestControlVert(m_pStickL->GetRawButton(3),true);
		}
		if (m_pStickL->GetRawButton(4)){
			m_pArmControl->TestControlHorz(m_pStickL->GetRawButton(4),false);
		} else if (m_pStickL->GetRawButton(5)) {
			m_pArmControl->TestControlHorz(m_pStickL->GetRawButton(5),true);
		} else {
			m_pArmControl->TestControlHorz(m_pStickL->GetRawButton(4),false);
			m_pArmControl->TestControlHorz(m_pStickL->GetRawButton(5),true);
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













