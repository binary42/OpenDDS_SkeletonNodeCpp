# OpenDDS_SkeletonNode Imlementation - RTIMU Reader Demo
OpenDDS Application Skeleton for P2P RTPS Distributed Frameworks.

This node was designed to encapsulate the RTIMU Demo Application GUI written in C++ using Qt as the user interface builder.

##Hard Requirements:

   OCI OpenDDS install for platform of your choice:[OCI OpenDDS](http://www.ociweb.com/products/opendds/)
		
      OpenDDS OS Support: http://www.opendds.org/downloads.html 
      Qt
	
##Usage Notes

   This node readers remote IMU DDS messages, and deplays a subset, demonstrating the ability to recieve IMU data from a remote 
   system, communicating over DDS with the Best Effort QoS parameter.
   
  Receives IMU data over the "ImuData" topic. 
  
##IDL:
  
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