# OpenDDS_SkeletonNode

This is a C++ application node encapsulating OCI's OpenDDS DDS libraries.

There are 6 branches. Master contains the cmake version of the node and constitutes the main stable(ish) development of the skeleton node.
 
Devel contains testing code of the master cmake. The eclipse branch contains the eclipse project version and may not be up to date with the cmake. 

The imu-node and rtimu-app-node are IMU impls as examples. However, these two nodes are very much out of date and most likely do not work. They are also eclipse project based, not cmake.

All of this is still in development so take nothing for granted and assume the node needs much work and improvement. Use at your own risk, as I am not liable for any use...as a matter of fact, just forget about this repo. Best of luck.

## Notes on branch master:
Currently, this is an example node, highlighting how to create a listener for depth data coming from the OpenROV Trident ROV, that implements RTI Connext DDS. This was created to give enthusiasts the ability to interface with the Trident using DDS without needing an expensive license.

Modeled from spiderkeys RTI impl. https://github.com/spiderkeys/SensorPublisher

OpenDDS Application Skeleton for P2P (Peer-2-Peer) RTPS (Real Time Publish Subscribe) Distributed Frameworks. MIT License

Developed and tested in Ubuntu 16.04. OpenDDS 3.13.

I am learning this as I go along. Please consult the OpenDDS manual for further information on utilizing DDS and QoS to best develop your applications: http://www.opendds.org/documentation.html
   
When cloning this repository, use the --recursive flag, otherwise you will not pull down required submodules. The OpenDDS submodule contains the IDL and command scripts for generating your C++ code for given IDLs.

## Code Modifications for Node personalization:

The code should be easy to read. Modification areas are highlights in CAppNodeImpl and CDataReaderListenerImpl. You should not need to modify the base classes nor the main.cpp. 
  
The CDataReaderListenerImpl is now part of the CAppNode class. The virtual methods contain all message logic for stubbed conditions. The class allows you to use the DCPS architecture to it's fullest.

## Running the Node:

    ./OpenDDS_SkeletonNodeApp -DCPSConfigFile Config/rtps.ini

## Continually under construction >>> USE AT YOUR OWN RISK

Much improvement is needed. Namespaces, utilities, cmake files, etc needed. Plus, better implementation
after a bit more research and testing.