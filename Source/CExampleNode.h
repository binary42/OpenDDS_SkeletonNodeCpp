#pragma once

#include "CAppClass.h"

#include "../OpenDDS/Source/Cpp/Generated/ExampleTypeSupportImpl.h"
#include "CDataReaderListenerImpl.h"

class CExampleNode : CAppClass
{
public:
	CExampleNode( int argc, ACE_TCHAR *argv[], std::string appNameIn, int domainIDIn );
	virtual ~CExampleNode();

	// Methods
	void Initialize();
	std::string GetName();

	void Run();
	void CleanUp();

	void SignalHandler( int sigNumIn );

	//Attributes
	bool								m_applicationTerminate;

private:
	// Methods
	void HandleWaitCondition();

	static void HandleSignal( int sigNumIn );

	void InitParticipant();
	void InitPublisherAndSubscriber();

	void InitTopicinfo();
	void InitDataWriter();
	void InitDataReader();

	//Attributes
	// OpenDDS Domain id
	DDS::DomainId_t 					_domainID;

	int									_argCount;
	ACE_TCHAR							*_pargVect[];

	DDS::DomainParticipantFactory_ptr 	_domainParticipantFactory;
	DDS::DomainParticipant_ptr 			_participant;

	// Publisher
	DDS::Publisher_ptr 					_publisher;
	ExampleApp::EventTypeSupport_ptr 	_exampleTypeSupport;

	// Subscriber
	DDS::Subscriber_ptr					_subscriber;

	// Topic info
	CORBA::String_var 					_topicTypeName;
	DDS::Topic_ptr						_topic;

	// Data writer
	DDS::DataWriter_ptr					_writer;
	ExampleApp::EventDataWriter_ptr 	_eventWriter;

	// Data reader
	DDS::DataReaderListener_ptr			_listener;
	DDS::DataReader_ptr					_reader;

	ExampleApp::EventDataReader_ptr		_readerI;

};
