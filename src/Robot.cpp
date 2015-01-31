#include "CTankDrive.h"
#include "WPILib.h"
#include "navX/IMU.h"
#include "Autonomous.h"


class Robot: public SampleRobot
{
	CTankDrive *m_pDrive; // Drive system
	Joystick m_stickL;
	Joystick m_stickR;

	IMU *m_navX;
	SerialPort *m_navXport;
	NetworkTable *m_table;
	bool m_firstIteration;
	PowerDistributionPanel m_pdp;
	INT32 m_LeftPos, m_RightPos;

public:
	Robot()	: m_stickL(0), m_stickR(1)
	{
		m_pDrive = new CTankDrive(0, 1, 2, 3, &m_stickL, &m_stickR);
		m_pDrive->SetupEncoders(0,1,2,3);
		m_pDrive->WatchdogOff();
		//m_pDrive->WatchdogOn(2.0);

		m_firstIteration = true;
		CAutonomous::Setup(m_pDrive);
		m_pDrive->initNavX();
	}


	// Autonomous Mode
	void Autonomous(void)
	{
		m_pDrive->WatchdogOff();
		while(IsAutonomous() && IsEnabled())
		{
			// Calibrate the navX if necessary
			if ( m_firstIteration )
			{
				bool is_calibrating = m_pDrive->IsNavXCalibrating();
				if ( !is_calibrating )
				{
					//Wait( 0.3 );
					m_pDrive->ZeroNavXYaw();
					m_firstIteration = false;
				}
			}

			// Now do it
			CAutonomous::RunAuto();

			// Wait for a motor update time
			Wait(0.005);
		}
	}



	// Regular Match Play
	void OperatorControl()
	{
		m_pDrive->WatchdogOff();

		while (IsOperatorControl() && IsEnabled())
		{

			// Get/display drive encoder values
			//m_pDrive->GetPositions(&m_LeftPos, &m_RightPos);
			//SmartDashboard::PutNumber( "Left", m_LeftPos);
			//SmartDashboard::PutNumber( "Right", m_RightPos);

			// Get/display navX info
			/*SmartDashboard::PutBoolean( "IMU_Connected", m_Drive.IsNavXConnected());
			SmartDashboard::PutNumber("IMU_Yaw", m_Drive.GetNavXYaw());
			SmartDashboard::PutNumber("IMU_Pitch", m_Drive.GetNavXPitch());
			SmartDashboard::PutNumber("IMU_Roll", m_Drive.GetNavXRoll());
			SmartDashboard::PutNumber("IMU_CompassHeading", m_Drive.GetNavXCompassHeading());
			SmartDashboard::PutNumber("IMU_Update_Count", m_Drive.GetNavXUpdateCount());
			SmartDashboard::PutNumber("IMU_Byte_Count", m_Drive.GetNavXByteCount());
			SmartDashboard::PutBoolean("IMU_IsCalibrating", m_Drive.IsNavXCalibrating());

			// Get/display info from Power Distribution Panel
			SmartDashboard::PutNumber("Current Channel 1", m_pdp.GetCurrent(1));
			SmartDashboard::PutNumber("Voltage", m_pdp.GetVoltage());
			SmartDashboard::PutNumber("Temperature", m_pdp.GetTemperature());
	*/
			// Run the tank drive
			m_pDrive->Go();

			// Wait for a motor update time
			Wait(0.005);
		}
	}

};

START_ROBOT_CLASS(Robot);
