#include "CExampleNode.h"

CExampleNode::CExampleNode( int argc, char *argv[], std::string appNameIn, int domainIDIn ):
	CAppClass( argc, argv, appNameIn )
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

		DDS::DomainParticipantFactory_var domainParticipantFactory = DDS::DomainParticipantFactory::_nil();
		domainParticipantFactory = TheParticipantFactoryWithArgs( _argCount, _pargVect );

		DDS::DomainParticipant_var participant = domainParticipantFactory->create_participant( 42, PARTICIPANT_QOS_DEFAULT,
																								DDS::DomainParticipantListener::_nil(),
																								OpenDDS::DCPS::DEFAULT_STATUS_MASK );
		// Exit is nil
		if( CORBA::is_nil( participant.in() ) )
		{
			throw( "Create Participant Failed.");
		}

		// Publisher
		DDS::Publisher_var publisher = participant->create_publisher( PUBLISHER_QOS_DEFAULT, DDS::PublisherListener::_nil() );

		// Exit if nil
		if( CORBA::is_nil( publisher.in() ) )
		{
			throw( "Create publisher failed." );
		}

		// Type registration
		ExampleApp::EventTypeSupport_var exampleServant = new ExampleApp::EventTypeSupportImpl();

		// Exit if retcode ! ok
		if( DDS::RETCODE_OK != exampleServant->register_type( participant.in(), EXAMPLE_DATA_TYPE ) )
		{
			throw( "register type failed." );
		}

		// Create a topic
		CORBA::String_var topicTypeName = exampleServant->get_type_name();
		DDS::Topic_var topic = participant->create_topic( "Test Topic", topicTypeName.in(),
															TOPIC_QOS_DEFAULT,
															DDS::TopicListener::_nil(),
															OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		if( CORBA::is_nil( topic.in() ) )
		{
			throw( "Create topic failed");
		}

		// Create the data writer
		DDS::DataWriter_var writer = publisher->create_datawriter( topic.in(), DATAWRITER_QOS_DEFAULT,
																	DDS::DataWriterListener::_nil(),
																	OpenDDS::DCPS::DEFAULT_STATUS_MASK );

		if( CORBA::is_nil( writer.in() ) )
		{
			throw( "Create datawriter failed");
		}

		ExampleApp::EventDataWriter_var eventWriter = ExampleApp::EventDataWriter::_narrow( writer.in() );

		if( CORBA::is_nil( messageWriter.in() ) )
		{
			throw( "_narrow failed");
		}

		// Blakc until subscriber is available
		DDS::StatusCondition_var condition = writer_.get_statuscondition();

		condition->set_enabled_statuses( DDS::PUBLICATION_MATCHED_STATUS );

		DDS::WaitSet_var waitSet = new DDS::Waitset;
		waitSet->attach_condition( condition );

		DDS::ConditionSeq conditionSeq;
		DDS::PublicationMatchedStatus matches = { 0, 0, 0, 0, 0 };
		DDS::Duration_t timeout = { 30. 0 };


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
	throw( "Run Failed" );
}

void CExampleNode::CleanUp()
{
	try
	{

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

}
