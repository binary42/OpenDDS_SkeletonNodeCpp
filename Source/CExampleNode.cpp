#include "CExampleNode.h"

CExampleNode::CExampleNode( int argc, char *argv[], std::string appNameIn, int domainIDIn ):
	CAppClass( argc, argv, appNameIn )
	, m_applicationTerminate( false)
	, _domainID( domainIDIn )
	, _argCount( argc )
	, _pargVect( argv )
{

}

CExampleNode::~CExampleNode()
{

}

void CExampleNode::Initialize()
{
	try
	{
		// Init the participant factory and participant
		_domainParticipantFactory = DDS::DomainParticipantFactory::_nil();

		_domainParticipantFactory = TheParticipantFactoryWithArgs( _argCount, _pargVect );

		_participant = _domainParticipantFactory->create_participant( 42, PARTICIPANT_QOS_DEFAULT,
																								DDS::DomainParticipantListener::_nil(),
																								OpenDDS::DCPS::DEFAULT_STATUS_MASK );
		// Exit is nil
		if( CORBA::is_nil( _participant.in() ) )
		{
			throw( "Create Participant Failed.");
		}

		// Publisher
		_publisher = _participant->create_publisher( PUBLISHER_QOS_DEFAULT, DDS::PublisherListener::_nil() );

		// Exit if nil
		if( CORBA::is_nil( _publisher.in() ) )
		{
			throw( "Create publisher failed." );
		}

		// Type registration
		_exampleServant = new ExampleApp::EventTypeSupportImpl();

		// Exit if retcode ! ok
		if( DDS::RETCODE_OK != _exampleServant->register_type( _participant.in(), EXAMPLE_DATA_TYPE ) )
		{
			throw( "register type failed." );
		}

		// Create a topic
		_topicTypeName = _exampleServant->get_type_name();

		_topic = _participant->create_topic( "Test Topic", _topicTypeName.in(),
															TOPIC_QOS_DEFAULT,
															DDS::TopicListener::_nil(),
															OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		if( CORBA::is_nil( _topic.in() ) )
		{
			throw( "Create topic failed");
		}

		// Create the data writer
		_writer = _publisher->create_datawriter( _topic.in(), DATAWRITER_QOS_DEFAULT,
																	DDS::DataWriterListener::_nil(),
																	OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		if( CORBA::is_nil( _writer.in() ) )
		{
			throw( "Create datawriter failed");
		}

		_eventWriter = ExampleApp::EventDataWriter::_narrow( _writer.in() );

		if( CORBA::is_nil( _eventWriter.in() ) )
		{
			throw( "_narrow failed");
		}

	}catch( CORBA::SystemException &excpt )
	{
		throw( excpt );
	}

}

std::string CExampleNode::GetName()
{
	return nullptr;
}

void CExampleNode::Run()
{
	while( !m_applicationTerminate == false )
	{

		// Handle recieved messages
		HandleWaitCondition();
	}
}

void CExampleNode::CleanUp()
{
	try
	{
		// Clean-up!
		_participant->delete_contained_entities();

		_domainParticipantFactory->delete_participant( _participant.in() );

		TheServiceParticipant->shutdown();

	}catch( CORBA::SystemException &excpt )
	{
		throw( excpt );
	}
}

void CExampleNode::InitializeWaitSet()
{

}

void CExampleNode::HandleWaitCondition()
{
	// Block until Subscriber is available
	DDS::StatusCondition_var condition = _writer->get_statuscondition();
	condition->set_enabled_statuses( DDS::PUBLICATION_MATCHED_STATUS );

	DDS::WaitSet_var waitSet = new DDS::WaitSet;
	waitSet->attach_condition( condition );

	DDS::ConditionSeq conditions;
	DDS::PublicationMatchedStatus matches = { 0, 0, 0, 0, 0 };
	DDS::Duration_t timeout = { 30, 0 };

	do {
	  if ( waitSet->wait(conditions, timeout ) != DDS::RETCODE_OK ) {
		throw( "wait condition failed." );
	  }

	  if ( _writer->get_publication_matched_status( matches ) != ::DDS::RETCODE_OK ) {
		throw( "Publication matched status failed." );
	  }

	} while ( matches.current_count < 1 );

	waitSet->detach_condition( condition );
}
