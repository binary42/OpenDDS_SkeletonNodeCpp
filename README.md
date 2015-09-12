# OpenDDS_SkeletonNode
OpenDDS Application Skeleton for P2P RTPS Distributed Frameworks. MIT License

This node was designed to encapsulate the OCI OpenDDS package, enabling researchers and system architects with a peer to peer nodal 
approach to distributed realt-time capable framework.

Developed and tested in Ubuntu 14.04. One day I may get back into windows. Please make a branch if you would like to persue the WinOS
development path.

   NOTE: I am learning this as I go along. Please consult the OpenDDS manual for further information on utilizing DDS and 
   QoS to best develop your applications: http://www.opendds.org/documentation.html
   
   Also, if you clone this repo, use the --recursive flag, otherwise you will not pull doen required submodules.

##Hard Requirements:

   OCI OpenDDS install for platform of your choice: [OCI OpenDDS](http://www.ociweb.com/products/opendds/)
		
      OpenDDS OS Support: http://www.opendds.org/downloads.html 
      
      Or on github: https://github.com/objectcomputing/OpenDDS
	
##Soft Requirements:

   Programming, OS:
      
      https://eclipse.org/ "Eclipse ( used for this project )"
      http://www.ubuntu.com/ "Linux"
      arm-linux: Pick your flavor and platform. Many to choose from

##Code Modifications for Node personalization:

  The code should be farely easy to read. Modification areas are highlights in CAppNodeImpl and CDataReaderListenerImpl. You should not
  need to modify the base classes nor the main.cpp. 
  
  The CDataReaderListenerImpl contains all message logic for stubbed conditions. The class allows you to use the DCPS architecture to
  it's fullest.

# STILL UNDER CONSTRUCTION >>> USE AT YOUR OWN RISK

Much improvement is needed. Namespaces, utilities, cmake files, etc needed. Plus, better implementation
after a bit more research and testing.