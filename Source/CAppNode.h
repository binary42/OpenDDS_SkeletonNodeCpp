#pragma once
// OpenDDS includes

#include <ace/Global_Macros.h>
#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>

#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/Marked_Default_Qos.h>

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include "dds/DCPS/StaticIncludes.h"

// Signal Handling
#include "Utility/CSignalException.h"

#include <dds/DCPS/Definitions.h>

// Type support for idl
#include "../OpenDDS/Source/Cpp/Generated/ExampleTypeSupportC.h"
#include "../OpenDDS/Source/Cpp/Generated/ExampleTypeSupportImpl.h"

#include <iostream>

class CAppNode : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{
public:
	CAppNode( int argc, ACE_TCHAR *argv[], std::string appNameIn );
	virtual ~CAppNode();

	// Methods - interface - pure virtual
	virtual std::string GetName() 					= 0;
	virtual void CleanUp() 							= 0;

	virtual void Initialize() 						= 0;

	virtual void HandleWaitCondition() 				= 0;
	virtual void Run() 								= 0;

	// OpenDDS Reader Inherited Impl - override in CAppNodeImpl class
	virtual void on_requested_deadline_missed( DDS::DataReader_ptr reader, const DDS::RequestedDeadlineMissedStatus& status);

    virtual void on_requested_incompatible_qos(	DDS::DataReader_ptr reader,	const DDS::RequestedIncompatibleQosStatus& status);

    virtual void on_sample_rejected( DDS::DataReader_ptr reader, const DDS::SampleRejectedStatus& status);

    virtual void on_liveliness_changed(	DDS::DataReader_ptr reader,	const DDS::LivelinessChangedStatus& status);

    virtual void on_data_available(	DDS::DataReader_ptr reader);

    virtual void on_subscription_matched( DDS::DataReader_ptr reader, const DDS::SubscriptionMatchedStatus& status);

  	virtual void on_sample_lost( DDS::DataReader_ptr reader, const DDS::SampleLostStatus& status);

	// Attributes
	std::vector<std::string> 						m_commandLineArguments;

protected:
	// Attributes
	std::string										_appName;
	CSignalHandler									*_psignalHandler;
};
