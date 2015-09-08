#include "CAppNodeImpl.h"
#include "Utility/Utils.h"

CAppNodeImpl::CAppNodeImpl( int argc, ACE_TCHAR *argv[], std::string appNameIn, int domainIDIn ):
	CAppNode( argc, argv, appNameIn )
	, m_applicationTerminate( false)
	, _domainID( domainIDIn )
	, _argCount( argc )
{
	for( size_t i = 0; i < _argCount; ++i)
	{
		_pargVect[i] = argv[i];
	}
}

CAppNodeImpl::~CAppNodeImpl()
{

}

void CAppNodeImpl::Initialize()
{
	try
	{
		// Setup signal handler for ctrl-c
		_psignalHandler->SetupSignalHandlers();

		// Init all DDS related components
		InitParticipant();

		InitPublisherAndSubscriber();

		InitTopicinfo();

		InitDataWriter();

		InitDataReader();

	}catch( CORBA::SystemException &excpt )
	{
		LOG( ERROR ) << "Failed: CORBA Exception.";

		CleanUp();
	}

}

std::string CAppNodeImpl::GetName()
{
	return _appName;
}

void CAppNodeImpl::Run()
{
	// Example Messaage to write
	ExampleApp::Event message;

	// For example writes
	int count = 0;

	while( !_psignalHandler->GotExitSignal() )
	{
		// Write out example message to ourselves
		message.kicker = "test";
		message.timestamp = nodeutils::GetUnixTimestampMs();

		//if( count % 5 != 0 )
		//{
			DDS::ReturnCode_t ret = _eventWriter->write( message, DDS::HANDLE_NIL );

			if( ret != DDS::RETCODE_OK )
			{
				LOG( ERROR ) << "Error write returned: " << ret;
			}
		//s}

		count++;

		//Wait for acknowledgement
//		DDS::Duration_t timeout = { 30, 0 };
//		_eventWriter->wait_for_acknowledgments( timeout );

		// Handle received messages
		HandleWaitCondition();
	}

	CleanUp();
}

void CAppNodeImpl::CleanUp()
{
	LOG( INFO ) << "Cleaning up application...";
	try
	{
		// Clean-up!
		_participant->delete_contained_entities();

		_domainParticipantFactory->delete_participant( _participant );

		TheServiceParticipant->shutdown();

	}catch( CORBA::SystemException &excpt )
	{
		LOG( ERROR ) << "Failed: CORBA Exception participants and factory not properly deleted.";
	}
}

void CAppNodeImpl::HandleWaitCondition()
{
	// Block until Subscriber is available
	DDS::StatusCondition_var condition = _writer->get_statuscondition();
	condition->set_enabled_statuses( DDS::PUBLICATION_MATCHED_STATUS );

	DDS::WaitSet_var waitSet = new DDS::WaitSet;
	waitSet->attach_condition( condition );

	DDS::ConditionSeq conditions;
	DDS::SubscriptionMatchedStatus matches = { 0, 0, 0, 0, 0 };
	DDS::Duration_t timeout = { 30, 0 };

	//do {
	  if ( waitSet->wait(conditions, timeout ) != DDS::RETCODE_OK )
	  {
		LOG( ERROR ) << "wait condition failed.";
	  }

	  if ( _reader->get_subscription_matched_status( matches ) != DDS::RETCODE_OK )
	  {
		LOG( ERROR ) << "Publication matched status failed.";
	  }

	//} while ( matches.current_count > 0 );

	waitSet->detach_condition( condition );
}

void CAppNodeImpl::InitParticipant()
{
	// Init the participant factory and participant
	_domainParticipantFactory = TheParticipantFactoryWithArgs( _argCount, _pargVect );

	_participant = _domainParticipantFactory->create_participant( _domainID, PARTICIPANT_QOS_DEFAULT,
																							DDS::DomainParticipantListener::_nil(),
																							OpenDDS::DCPS::DEFAULT_STATUS_MASK );

	// Exit is nil
	if( !_participant )
	{
		LOG( ERROR ) << "Create Participant Failed.";
	}
}

void CAppNodeImpl::InitPublisherAndSubscriber()
{
	// Publisher
	_publisher = _participant->create_publisher( PUBLISHER_QOS_DEFAULT,
																	DDS::PublisherListener::_nil(),
																	OpenDDS::DCPS::DEFAULT_STATUS_MASK );

	// Exit if nil
	if( !_publisher )
	{
		LOG( ERROR ) << "Create publisher failed.";
	}

	// Subscriber
	_subscriber = _participant->create_subscriber( SUBSCRIBER_QOS_DEFAULT,
													DDS::SubscriberListener::_nil(),
													OpenDDS::DCPS::DEFAULT_STATUS_MASK );
	if( !_subscriber )
	{
		LOG( ERROR ) << "Create subscriber failed.";
	}
}

void CAppNodeImpl::InitTopicinfo()
{
	// Type registration
	_exampleTypeSupport = new ExampleApp::EventTypeSupportImpl();

	// Exit if retcode ! ok
	if( DDS::RETCODE_OK != _exampleTypeSupport->register_type( _participant, "" ) )
	{
		LOG( ERROR ) << "register type failed.";
	}

	// Create a topic
	_topicTypeName = _exampleTypeSupport->get_type_name();

	_topic = _participant->create_topic( "Test Topic", _topicTypeName.in(),
														TOPIC_QOS_DEFAULT,
														DDS::TopicListener::_nil(),
														OpenDDS::DCPS::DEFAULT_STATUS_MASK );

	if( !_topic )
	{
		LOG( ERROR ) << "Create topic failed";
	}
}

void CAppNodeImpl::InitDataWriter()
{
	// Create the data writer
	_writer = _publisher->create_datawriter( _topic, DATAWRITER_QOS_DEFAULT,
																DDS::DataWriterListener::_nil(),
																OpenDDS::DCPS::DEFAULT_STATUS_MASK );

	if( !_writer )
	{
		LOG( ERROR ) << "Create datawriter failed";
	}

	_eventWriter = ExampleApp::EventDataWriter::_narrow( _writer );

	if( !_eventWriter )
	{
		LOG( ERROR ) << "_narrow failed";
	}

}

void CAppNodeImpl::InitDataReader()
{
	// Data Reader
	_listener = new CDataReaderListenerImpl;

	_reader = _subscriber->create_datareader( _topic, DATAREADER_QOS_DEFAULT,
												_listener,
												OpenDDS::DCPS::DEFAULT_STATUS_MASK );

	if( !_reader )
	{
		LOG( ERROR ) << " Create data reader failed";
	}

	_readerI = ExampleApp::EventDataReader::_narrow( _reader );

	if( !_readerI )
	{

		LOG( ERROR ) << " _narrow failed";
	}
}
