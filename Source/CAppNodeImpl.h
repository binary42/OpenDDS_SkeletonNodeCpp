#pragma once

#include "CAppNode.h"

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

	// Reader overrides
	void on_data_available(	DDS::DataReader_ptr reader);

private:
	// Methods
	void HandleWaitCondition();

	//Attributes
	DDS::DomainId_t 					_domainID;

	int									_argCount;
	ACE_TCHAR							*_pargVect[];

	DDS::DomainParticipantFactory_ptr 	_domainParticipantFactory;
	DDS::DomainParticipant_ptr 			_participant;

	// Publisher
	DDS::Publisher_ptr 					_publisher;
	orov::DepthTypeSupport_ptr 			_rovDepthTypeSupport;

	// Subscriber
	DDS::Subscriber_ptr					_subscriber;

	// Topic info
	CORBA::String_var 					_topicTypeName;
	DDS::Topic_ptr						_topic;

	// Data writer
	DDS::DataWriter_ptr					_writer;
	orov::DepthDataWriter_ptr 	_eventWriter;

	// Data reader
	DDS::DataReaderListener_ptr			_listener;
	DDS::DataReader_ptr					_reader;

	orov::DepthDataReader_ptr			_readerI;

};
