#include "CAppNodeImpl.h"
#include "Utility/Utils.h"

/**
 * Constructor
 */
CAppNodeImpl::CAppNodeImpl( int argc, ACE_TCHAR *argv[], std::string appNameIn, int domainIDIn ):
	CAppNode( argc, argv, appNameIn )
	, m_applicationTerminate( false)
	, _domainID( domainIDIn )
	, _argCount( argc ), _domainParticipantFactory( nullptr), _participant( nullptr )
	, _publisher( nullptr ), _rovDepthTypeSupport( nullptr ), _subscriber( nullptr)
	, _topic( nullptr ), _writer( nullptr ), _eventWriter( nullptr ),  _listener( nullptr )
	, _reader( nullptr), _readerI( nullptr )
{
	for( int i = 0; i < _argCount; ++i)
	{
		_pargVect[i] = argv[i];
	}
}

/**
 * Destructor
 */
CAppNodeImpl::~CAppNodeImpl()
{

}

/**
 * Void method that initializes all components of the node. Edit as needed.
 */
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

/**
 * Returns the name of the application.
 * @return		_appname		string type
 */
std::string CAppNodeImpl::GetName()
{
	return _appName;
}

/**
 * Main execution method. Application specific code goes here. Calls HandleWaiCondition()
 */
void CAppNodeImpl::Run()
{
	// Example Messaage to write
//	 orov::Depth message;

	while( !_psignalHandler->GotExitSignal() )
	{
		// Write out example message to ourselves
//		 message.id = "test";
//		 message.depths = nodeutils::GetUnixTimestampMs();
//		 DDS::ReturnCode_t ret = _eventWriter->write( message, DDS::HANDLE_NIL );

		// Listening for messages we've subscribed. This is handled by the on_data_available method

//		if( ret != DDS::RETCODE_OK )
//		{
//			LOG( ERROR ) << "Error write returned: ";// << ret;
//		}

		// Handle received messages
		HandleWaitCondition();
	}

	CleanUp();// todo don't think i need this since using unique pointer
}

/**
 * Cleans up the DDS objects. Edited as needed.
 */
void CAppNodeImpl::CleanUp()
{
	LOG( INFO ) << "Cleaning up application...";
	try
	{
		// Clean-up
		_participant->delete_contained_entities();

		_domainParticipantFactory->delete_participant( _participant );

		TheServiceParticipant->shutdown();

	}catch( CORBA::SystemException &excpt )
	{
		LOG( ERROR ) << "Failed: CORBA Exception participants and factory not properly deleted.";
	}
}

/**
 * Handles zero copy message sets. Consider your requirements and edit timeout nad matched status bits as needed.
 * QoS should drive all of you design decisions.
 */
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

	if ( waitSet->wait(conditions, timeout ) != DDS::RETCODE_OK )
	{
		LOG( ERROR ) << "wait condition failed.";
	}

	if ( _reader->get_subscription_matched_status( matches ) != DDS::RETCODE_OK )
	{
		LOG( ERROR ) << "Publication matched status failed.";
	}

	waitSet->detach_condition( condition );
}

/**
 * Initializes the domain participant factory and participant DDS entities. Consider Qos
 * for design changes.
 */
void CAppNodeImpl::InitParticipant()
{
	// Init the participant factory and participant
	_domainParticipantFactory = TheParticipantFactoryWithArgs( _argCount, _pargVect );

	_participant = _domainParticipantFactory->create_participant( _domainID, PARTICIPANT_QOS_DEFAULT,
																							DDS::DomainParticipantListener::_nil(),
																							OpenDDS::DCPS::DEFAULT_STATUS_MASK );
	if( !_participant )
	{
		LOG( ERROR ) << "Create Participant Failed.";
	}
}

/**
 * Initializes the data pubisler and subscriber  DDS entities. Consider Qos
 * for design changes. Comment out or remove what is not needed. You can also create the subs and pubs
 * and not use them.
 */
void CAppNodeImpl::InitPublisherAndSubscriber()
{
	// Publisher
	DDS::PublisherQos publisher_qos;
	DDS::ReturnCode_t ret = _participant->get_default_publisher_qos(publisher_qos);

	if (ret != DDS::RETCODE_OK) {

		LOG( ERROR )<< "get_default_publisher_qos error.";
	}

	publisher_qos.partition.name.length(1);

	publisher_qos.partition.name[0] = CORBA::string_dup("4ec14b6");

	_publisher = _participant->create_publisher( publisher_qos,
																	DDS::PublisherListener::_nil(),
																	OpenDDS::DCPS::DEFAULT_STATUS_MASK );
	if( !_publisher )
	{
		LOG( ERROR ) << "Create publisher failed.";
	}

	DDS::SubscriberQos subscriber_qos;
	ret = _participant->get_default_subscriber_qos(subscriber_qos);

	if (ret != DDS::RETCODE_OK) {

		LOG( ERROR )<< "get_default_subscriber_qos error.";
	}

	subscriber_qos.partition.name.length(1);

	subscriber_qos.partition.name[0] = CORBA::string_dup("4ec14b6");

	// Subscriber
	_subscriber = _participant->create_subscriber( subscriber_qos,
													DDS::SubscriberListener::_nil(),
													OpenDDS::DCPS::DEFAULT_STATUS_MASK );
	if( !_subscriber )
	{
		LOG( ERROR ) << "Create subscriber failed.";
	}
}

/**
 * Initializes the topic. You will need to modify and or add to this black for varying data types and
 * topics given your message requirements and QoS design decisions.
 */
void CAppNodeImpl::InitTopicinfo()
{
	// Type registration
	_rovDepthTypeSupport = new orov::DepthTypeSupportImpl();

	// Exit if retcode ! ok
	if( DDS::RETCODE_OK != _rovDepthTypeSupport->register_type( _participant, "" ) )
	{
		LOG( ERROR ) << "register type failed.";
	}

	// Create a topic
	_topicTypeName = _rovDepthTypeSupport->get_type_name();

	_topic = _participant->create_topic( "rov_depth", _topicTypeName.in(),
														TOPIC_QOS_DEFAULT,
														DDS::TopicListener::_nil(),
														OpenDDS::DCPS::DEFAULT_STATUS_MASK );
	if( !_topic )
	{
		LOG( ERROR ) << "Create topic failed";
	}
}

/**
 * initializes the data writer. Consider your design QoS design requirements. Can ignore if not
 * needed.
 */
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

	_eventWriter = orov::DepthDataWriter::_narrow( _writer );

	if( !_eventWriter )
	{
		LOG( ERROR ) << "_narrow failed";
	}
}

/**
 * Initializes the data reader. As with the writer, may not need. Modify with QoS design in mind.
 */
void CAppNodeImpl::InitDataReader()
{
	// Data Reader
	_listener = this;// new CDataReaderListenerImpl;

	_reader = _subscriber->create_datareader( _topic, DATAREADER_QOS_DEFAULT,
												_listener,
												OpenDDS::DCPS::DEFAULT_STATUS_MASK );
	if( !_reader )
	{
		LOG( ERROR ) << " Create data reader failed";
	}

	_readerI = orov::DepthDataReader::_narrow( _reader );

	if( !_readerI )
	{

		LOG( ERROR ) << " _narrow failed";
	}
}

// Reader overrides
void CAppNodeImpl::on_data_available(DDS::DataReader_ptr reader)
{
  orov::DepthSeq messages;
  DDS::SampleInfoSeq info;

  DDS::ReturnCode_t status = _readerI->take(messages,
                                           info,
                                           DDS::LENGTH_UNLIMITED,
                                           DDS::ANY_SAMPLE_STATE,
                                           DDS::ANY_VIEW_STATE,
                                           DDS::ANY_INSTANCE_STATE);

  if (status == DDS::RETCODE_OK) {
    LOG( INFO ) << "SampleInfo.sample_rank = " << info[0].sample_rank;
    LOG( INFO ) << "SampleInfo.instance_state = " << info[0].instance_state;

    if (info[0].valid_data) {
    	LOG( INFO ) << "Rov Depth Data: " << messages[0].depth;
    }

  } else {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" take failed!\n")));
  }

  _readerI->return_loan(messages, info);
}
