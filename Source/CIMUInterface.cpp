/**
 * author:		J. Neilan
 * email:		jimbolysses@gmail.com
 * version:		v0.8
 * notes:
 * 	CTD IMU Interface to the CTD payload. Uses the RTIMULib from Richards Tech for the OpenROV IMU/
 * 	Depth sensor.
 * 
 * License info: Reference to the RTIMULib: https://github.com/richards-tech/RTIMULib
 * 
 * The MIT License (MIT)

	Copyright (c) 2015 All of Us :)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
 * 	
 * 
 */

#include "CIMUInterface.h"	

CIMUInterface::CIMUInterface()
{
	// Grabs the .ini file in local directory
	m_pSettings = new RTIMUSettings( "RTIMULib" );
	
	// Create IMU object
	m_pImu = RTIMU::createIMU( m_pSettings );
	
	// 10DOF IMU - pressure sensor addition
	m_pPressure = RTPressure::createPressure( m_pSettings );
}

CIMUInterface::~CIMUInterface()
{
	// Rid the world of our referenced objects
	delete( m_pSettings );
	delete( m_pImu );
	delete( m_pPressure );
}

void CIMUInterface::Setup( float slerPwrIn, bool enableGyro, bool enableAccel, bool enableComp )
{
	// Initialize IMU
	m_pImu->IMUInit();
	
	// Set power setting
	m_pImu->setSlerpPower( slerPwrIn );
	
	// Enable the Gyro, Accel and Compass
	m_pImu->setGyroEnable( enableGyro );
	
	m_pImu->setAccelEnable( enableAccel );
	
	m_pImu->setCompassEnable( enableComp );

	// If the pressure sensor exists, init it
	if( m_pPressure != nullptr )
	{
		m_pPressure->pressureInit();
	}
}	

RTIMU_DATA CIMUInterface::GetPoseInfo()
{		
	// Returns the RTIMU_DATA object containg current readings
	m_imuData = m_pImu->getIMUData();
	
	// Add pressure to structure
	if( m_pPressure != nullptr )
	{
		m_pPressure->pressureRead( m_imuData );
	}
	
	return m_imuData;
}
