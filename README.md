# OpenDDS_SkeletonNode

Modeled from spiderkeys RTI impl. https://github.com/spiderkeys/SensorPublisher

OpenDDS Application Skeleton for P2P RTPS Distributed Frameworks. MIT License

This node was designed to encapsulate the OCI OpenDDS package, enabling researchers and system architects with a peer to peer nodal 
approach to distributed realt-time capable framework.

Developed and tested in Ubuntu 14.04. One day I may get back into windows. Please make a branch if you would like to persue the WinOS
development path.

   NOTE: I am learning this as I go along. Please consult the OpenDDS manual for further information on utilizing DDS and 
   QoS to best develop your applications: http://www.opendds.org/documentation.html
   
   Also, if you clone this repo, use the --recursive flag, otherwise you will not pull down required submodules.

##Hard Requirements:

   TAO ( The Ace ORB ) is required:

		From OpenDDS Install README:

		OpenDDS requires TAO for both IDL compilation as well as interaction
		with the DCPSInfoRepo.  If you will be using the "configure" script for OpenDDS
		(see the INSTALL file for details), you do not need to download TAO first --
		the "configure" script will download it for you.
		
		At a minimum, you must be at one of the following versions in order to properly
		compile OpenDDS:
		
		*** TAO 2.0a patch 7
		*** TAO 2.2a patch 8
		*** TAO 2.3.3 (DOC Group)
		
		Note that the 2.0a and 2.2a releases are from OCI and can be obtained
		from http://www.theaceorb.com/.  The DOC Group releases can be obtained from
		http://download.dre.vanderbilt.edu/.
		
		OpenDDS Safety Profile requires OCI TAO 2.2a patch 8 or
		DOC Group TAO from GitHub master (after 2.3.3 release).

   OCI OpenDDS install for platform of your choice: [OCI OpenDDS](http://www.ociweb.com/products/opendds/)
		
      	OpenDDS OS Support: http://www.opendds.org/downloads.html 
      
      	Or on github: https://github.com/objectcomputing/OpenDDS
	
##Soft Requirements:

   Programming, OS:
      
      https://eclipse.org/ "Eclipse ( used for this project )"
      http://www.ubuntu.com/ "Linux"
      arm-linux: Pick your flavor and platform. Many to choose from

##Code Modifications for Node personalization:

  The code should be easy to read. Modification areas are highlights in CAppNodeImpl and CDataReaderListenerImpl. You should not
  need to modify the base classes nor the main.cpp. 
  
  The CDataReaderListenerImpl contains all message logic for stubbed conditions. The class allows you to use the DCPS architecture to
  it's fullest.

#Running the Node:

    ./OpenDDS_SkeletonNodeApp -DCPSConfigFile Config/rtps.ini

# STILL UNDER CONSTRUCTION >>> USE AT YOUR OWN RISK

Much improvement is needed. Namespaces, utilities, cmake files, etc needed. Plus, better implementation
after a bit more research and testing.