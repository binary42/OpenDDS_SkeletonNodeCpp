#pragma once
// OpenDDS includes
#include "../ThirdPartyLibs/OCI/dds/DCPS/Service_Participant.h"
#include "../ThirdPartyLibs/OCI/dds/DCPS/Marked_Default_Qos.h"
#include "../ThirdPartyLibs/OCI/dds/DCPS/PublisherImpl.h"
#include "../ThirdPartyLibs/OCI/dds/DCPS/SubscriberImpl.h"

// Framework includes
#include "Utility/Utils.h"

class CAppClass
{
public:
	CAppClass( int argc, char *argv[], std::string appNameIn );
	virtual ~CAppClass();

	// Methods
	virtual std::string GetName() = 0;
	virtual void CleanUp() = 0;

	virtual void Initialize() = 0;
	virtual void InitializeWaitSet() = 0;

	virtual void HandleWaitCondition() = 0;
	virtual void Run() = 0;

	// Attributes
	std::vector<std::string> 				m_commandLineArguments;

private:
	// Attributes
	std::string								_appName;

};
