#include "CExampleNode.h"
#include "Utility/Utils.h"

CExampleNode::CExampleNode( int argc, ACE_TCHAR *argv[], std::string appNameIn, int domainIDIn ):
	CAppClass( argc, argv, appNameIn )
	, m_applicationTerminate( false)
	, _domainID( domainIDIn )
	, _argCount( argc )
{
	for( size_t i = 0; i < _argCount; ++i)
	{
		_pargVect[i] = argv[i];
	}
}

CExampleNode::~CExampleNode()
{

}

void CExampleNode::Initialize()
{
	try
	{
		// Init the participant factory and participant
		DDS::DomainParticipantFactory_var _domainParticipantFactory = TheParticipantFactoryWithArgs( _argCount, _pargVect );

		DDS::DomainParticipant_var _participant = _domainParticipantFactory->create_participant( _domainID, PARTICIPANT_QOS_DEFAULT,
																								DDS::DomainParticipantListener::_nil(),
																								OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		// Exit is nil
		if( !_participant )
		{
			LOG( ERROR ) << "Create Participant Failed.";
		}

		// Publisher
		DDS::Publisher_var _publisher = _participant->create_publisher( PUBLISHER_QOS_DEFAULT,
																		DDS::PublisherListener::_nil(),
																		OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		// Exit if nil
		if( !_publisher )
		{
			LOG( ERROR ) << "Create publisher failed.";
		}

		// Type registration
		ExampleApp::EventTypeSupport_var _exampleTypeSupport = new ExampleApp::EventTypeSupportImpl();

		// Exit if retcode ! ok
		if( DDS::RETCODE_OK != _exampleTypeSupport->register_type( _participant.in(), "" ) )
		{
			LOG( ERROR ) << "register type failed.";
		}

		// Create a topic
		CORBA::String_var _topicTypeName = _exampleTypeSupport->get_type_name();

		DDS::Topic_var _topic = _participant->create_topic( "Test Topic", _topicTypeName.in(),
															TOPIC_QOS_DEFAULT,
															DDS::TopicListener::_nil(),
															OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		if( CORBA::is_nil( _topic.in() ) )
		{
			LOG( ERROR ) << "Create topic failed";
		}

		// Create the data writer
		DDS::DataWriter_var _writer = _publisher->create_datawriter( _topic.in(), DATAWRITER_QOS_DEFAULT,
																	DDS::DataWriterListener::_nil(),
																	OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		if( CORBA::is_nil( _writer.in() ) )
		{
			LOG( ERROR ) << "Create datawriter failed";
		}

		ExampleApp::EventDataWriter_var _eventWriter = ExampleApp::EventDataWriter::_narrow( _writer.in() );

		if( CORBA::is_nil( _eventWriter.in() ) )
		{
			LOG( ERROR ) << "_narrow failed";
		}

	}catch( CORBA::SystemException &excpt )
	{
		LOG( ERROR ) << "Failed: CORBA Exception.";

		CleanUp();
	}

}

std::string CExampleNode::GetName()
{
	return _appName;
}

void CExampleNode::Run()
{
	while( !m_applicationTerminate )
	{

		// Handle recieved messages
		//HandleWaitCondition();
	}
}

void CExampleNode::CleanUp()
{
	LOG( INFO ) << "Cleaning up application...";
	try
	{
		// Clean-up!
//		_participant->delete_contained_entities();
//
//		_domainParticipantFactory->delete_participant( _participant.in() );
//
//		TheServiceParticipant->shutdown();

	}catch( CORBA::SystemException &excpt )
	{
		LOG( ERROR ) << "Failed: CORBA Exception participants and factory not properly deleted.";
	}
}

void CExampleNode::InitializeWaitSet()
{

}

void CExampleNode::HandleWaitCondition()
{
	// Block until Subscriber is available
//	DDS::StatusCondition_var condition = _writer->get_statuscondition();
//	condition->set_enabled_statuses( DDS::PUBLICATION_MATCHED_STATUS );
//
//	DDS::WaitSet_var waitSet = new DDS::WaitSet;
//	waitSet->attach_condition( condition );
//
//	DDS::ConditionSeq conditions;
//	DDS::PublicationMatchedStatus matches = { 0, 0, 0, 0, 0 };
//	DDS::Duration_t timeout = { 30, 0 };
//
//	do {
//	  if ( waitSet->wait(conditions, timeout ) != DDS::RETCODE_OK ) {
//		throw( "wait condition failed." );
//	  }
//
//	  if ( _writer->get_publication_matched_status( matches ) != ::DDS::RETCODE_OK ) {
//		throw( "Publication matched status failed." );
//	  }
//
//	} while ( matches.current_count < 1 );
//
//	waitSet->detach_condition( condition );
}
