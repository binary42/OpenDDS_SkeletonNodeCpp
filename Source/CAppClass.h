#pragma once
// OpenDDS includes
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include "dds/DCPS/StaticIncludes.h"

class CAppClass
{
public:
	CAppClass( int argc, ACE_TCHAR *argv[], std::string appNameIn );
	virtual ~CAppClass();

	// Methods - interface - pure virtual
	virtual std::string GetName() 					= 0;
	virtual void CleanUp() 							= 0;

	virtual void Initialize() 						= 0;
	virtual void InitializeWaitSet() 				= 0;

	virtual void HandleWaitCondition() 				= 0;
	virtual void Run() 								= 0;

	// Attributes
	std::vector<std::string> 				m_commandLineArguments;

protected:
	// Attributes
	std::string								_appName;
};
