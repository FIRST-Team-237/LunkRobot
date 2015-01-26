#include "CTankDrive.h"
#include "WPILib.h"
#include "navX/IMU.h"
#include "Autonomous.h"


class Robot: public SampleRobot
{
	CTankDrive m_Drive; // Drive system
	Joystick m_stickL;
	Joystick m_stickR;

	IMU *m_navX;
	SerialPort *m_navXport;
	NetworkTable *m_table;
	bool m_firstIteration;
	PowerDistributionPanel m_pdp;
	INT32 m_LeftPos, m_RightPos;

public:
	Robot() :
			m_Drive(0, 1, 2, 3, &m_stickL, &m_stickR),
			m_stickL(0),
			m_stickR(1)
	{
		m_Drive.SetupEncoders(0,1,2,3);
		m_Drive.WatchdogOff();
		//m_Drive.WatchdogOn(2.0);
		m_table = NetworkTable::GetTable("datatable");
		m_navXport = new SerialPort(57600,SerialPort::kMXP);
		uint8_t updateRateHz = 50;
		m_navX = new IMU(m_navXport,updateRateHz);
		if ( m_navX ) {
			LiveWindow::GetInstance()->AddSensor("IMU","Gyro",m_navX);
		}
		m_firstIteration = true;
		CAutonomous::Setup(&m_Drive, m_navX);
	}


	// Autonomous Mode
	void Autonomous(void)
	{
		while(IsAutonomous() && IsEnabled())
		{
			// Calibrate the navX if necessary
			if ( m_firstIteration )
			{
				bool is_calibrating = m_navX->IsCalibrating();
				if ( !is_calibrating )
				{
					Wait( 0.3 );
					m_navX->ZeroYaw();
					m_firstIteration = false;
				}
			}

			// Now do it
			CAutonomous::RunAuto();
		}
	}



	// Regular Match Play
	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			// Calibrate the navX if necessary
			if ( m_firstIteration ) {
				bool is_calibrating = m_navX->IsCalibrating();
				if ( !is_calibrating ) {
					Wait( 0.3 );
					m_navX->ZeroYaw();
					m_firstIteration = false;
				}
			}

			// Get/display drive encoder values
			m_Drive.GetPositions(&m_LeftPos, &m_RightPos);
			SmartDashboard::PutNumber( "Left", m_LeftPos);
			SmartDashboard::PutNumber( "Right", m_RightPos);

			// Get/display navX info
			SmartDashboard::PutBoolean( "IMU_Connected", m_navX->IsConnected());
			SmartDashboard::PutNumber("IMU_Yaw", m_navX->GetYaw());
			SmartDashboard::PutNumber("IMU_Pitch", m_navX->GetPitch());
			SmartDashboard::PutNumber("IMU_Roll", m_navX->GetRoll());
			SmartDashboard::PutNumber("IMU_CompassHeading", m_navX->GetCompassHeading());
			SmartDashboard::PutNumber("IMU_Update_Count", m_navX->GetUpdateCount());
			SmartDashboard::PutNumber("IMU_Byte_Count", m_navX->GetByteCount());
			SmartDashboard::PutBoolean("IMU_IsCalibrating", m_navX->IsCalibrating());

			// Get/display info from Power Distribution Panel
			SmartDashboard::PutNumber("Current Channel 1", m_pdp.GetCurrent(1));
			SmartDashboard::PutNumber("Voltage", m_pdp.GetVoltage());
			SmartDashboard::PutNumber("Temperature", m_pdp.GetTemperature());

			// Run the tank drive
			m_Drive.Go();

			// Wait for a motor update time
			Wait(0.005);
		}
	}

};

START_ROBOT_CLASS(Robot);
