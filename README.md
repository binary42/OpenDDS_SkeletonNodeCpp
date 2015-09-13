# OpenDDS_SkeletonNode Implementation - RTIMU Publisher
OpenDDS Application for interfacing with support IMUs, utilizing the RTIMU Library for P2P RTPS Distributed Frameworks.

This node was designed to encapsulate the RTIMU library within a DDS framework as an example of a sensor publisher.

   NOTE: I am learning this as I go along. Please consult the OpenDDS manual for further information on utilizing DDS and 
   QoS to best develop your applications: http://www.opendds.org/documentation.html

##Hard Requirements:

   OCI OpenDDS install for ARM processors and a Raspberry Pi board.
		
      OpenDDS OS Support: http://www.opendds.org/downloads.html 
      
      R-Pi: https://www.raspberrypi.org/
	
##Soft Requirements:

   Programming, OS:
      
      Eclipse (used for this project):https://eclipse.org/ 
      Rasbian OD for the Pi.
      
##Code Modifications for Node personalization:

  The code should be farely easy to read. Modification areas are highlights in CAppNodeImpl and CDataReaderListenerImpl. You should not
  need to modify the base classes nor the main.cpp. 
  
  The CDataReaderListenerImpl contains all message logic for stubbed conditions. The class allows you to use the DCPS architecture to
  it's fullest.
  
  CIMUInterface contains IMU and RTIMU implmenetation methods.
  
  Publishes IMU data over the "ImuData" topic. 
  
  IDL:
  
  #include "orbsvcs/TimeBase.idl"

  module ImuNodeApp
  {
	#pragma DCPS_DATA_TYPE "ImuNodeApp::TImuData"
	#pragma DCPS_DATA_KEY  "ImuNodeApp::TImuData name"

	struct TImuData
	{
		string name;
	    
	    boolean	fusionPoseValid;
	    // Euler
	    float	fusionPoseX;
	    float	fusionPosey;
	    float	fusionposez;
	    
	    // Quarternion
	    boolean	fusionQValid;
	    float	fusionQ1Pose;
	    float	fusionQ2Pose;
	    float	fusionQ3Pose;
	    float	fusionQ4Pose;
	    
	    // Gyrp
	    boolean	gyroValid;
	    float	gyroX;
	    float	gyroY;
	    float	gyroZ;
	    
	    // Accel
	    boolean	accelValid;
	    float	accelX;
	    float	accelY;
	    float	accelZ;
	    
	    // Mag
	    boolean	magValid;
	    float	magX;
	    float	magY;
	    float	magZ;
	    
    	// Pressure
    	boolean	pressureValid;
    	float	pressure;
	    
    	// Temp
    	boolean	tempValid;
    	float 	temperature;
	    
	    	//Humdity
		boolean	humidValid;
		float	humidity;
		
		TimeBase::TimeT timestamp;
	 };
  };
	 
# STILL UNDER CONSTRUCTION >>> USE AT YOUR OWN RISK

Much improvement is needed. Namespaces, utilities, cmake files, etc needed. Plus, better implementation
after a bit more research and testing.