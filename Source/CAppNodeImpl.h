#pragma once

#include "../OpenDDS/Source/Cpp/Generated/ImuDataTypeSupportImpl.h"
#include "CAppNode.h"
#include "CIMUInterface.h"

class CAppNodeImpl : CAppNode
{
public:
	CAppNodeImpl( int argc, ACE_TCHAR *argv[], std::string appNameIn, int domainIDIn );
	virtual ~CAppNodeImpl();

	// Methods
	void Initialize();
	std::string GetName();

	virtual void Run();
	void CleanUp();

	//Attributes
	bool								m_applicationTerminate;

protected:
	// Methods
	void virtual InitParticipant();
	void virtual InitPublisherAndSubscriber();

	void virtual InitTopicinfo();
	void virtual InitDataWriter();
	void virtual InitDataReader();

private:
	// Methods
	void HandleWaitCondition();

	//Attributes
	std::unique_ptr<CIMUInterface> 		_imuInterface;
	RTIMU_DATA							_imuData;

	DDS::DomainId_t 					_domainID;

	int									_argCount;
	ACE_TCHAR							*_pargVect[];

	DDS::DomainParticipantFactory_ptr 	_domainParticipantFactory;
	DDS::DomainParticipant_ptr 			_participant;

	// Publisher
	DDS::Publisher_ptr 					_publisher;
	ImuNodeApp::ImuDataTypeSupport_ptr 	_imuDataTypeSupport;

	// Topic info
	CORBA::String_var 					_topicTypeName;
	DDS::Topic_ptr						_topic;

	// Data writer
	DDS::DataWriter_ptr					_writer;
	ImuNodeApp::TImuData_out			_eventWriter;
};
