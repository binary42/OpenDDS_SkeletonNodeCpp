#pragma once

#include "CAppClass.h"

class CExampleNode : CAppClass
{
public:
	CExampleNode( int argc, char *argv[], std::string appNameIn, int domainIDIn );
	virtual ~CExampleNode();

	// Methods
	void Initialize();
	std::string GetName();
	void Run();
	void CleanUp();

	//Attributes
	bool								m_applicationTerminate;

private:
	// Methods
	void InitializeWaitSet();
	void HandleWaitCondition();

	//Attributes
	// OpenDDS Domain id
	DDS::DomainId_t 					_domainID;

	int									_argCount;
	char								*_pargVect[];
	DDS::DomainParticipantFactory_var 	_domainParticipantFactory;
	DDS::DomainParticipant_var 			_participant;
	DDS::Publisher_var 					_publisher;
	ExampleApp::EventTypeSupport_var 	_exampleServant;
	CORBA::String_var 					_topicTypeName;
	DDS::Topic_var 						_topic;
	DDS::DataWriter_var 				_writer;
	ExampleApp::EventDataWriter_var 	_eventWriter;

};
