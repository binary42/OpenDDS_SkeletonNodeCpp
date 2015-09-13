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
	, _publisher( nullptr), _imuDataTypeSupport( nullptr )
	, _topic( nullptr ), _writer( nullptr )

{
	for( int i = 0; i < _argCount; ++i)
	{
		_pargVect[i] = argv[i];
	}

	// Imu interface pointer
	_imuInterface->Setup( 0.02, true, true, true );

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
 * Retruns the name of the application.
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
	ImuNodeApp::TImuData message;

	while( !_psignalHandler->GotExitSignal() )
	{
		// Get the IMU data
		_imuData = _imuInterface->GetPoseInfo();

		// Write out the IMU data
		message.name = "OpenROV IMU Data";
		message.timestamp = _imuData.timestamp;

		message.accelValid 	= _imuData.accelValid;
		message.accelX 		= _imuData.accel.data( 0 );
		message.accelY		= _imuData.accel.data( 0 );
		message.accelZ		= _imuData.accel.data( 0 );

		message.fusionPoseValid = _imuData.fusionPoseValid;
		message.fusionPoseX		= _imuData.fusionPose.data( 0 );
		message.fusionPosey		= _imuData.fusionPose.data( 1 );
		message.fusionposez		= _imuData.fusionPose.data( 2 );

		message.fusionQValid	= _imuData.fusionQPoseValid;
		message.fusionQ1Pose	= _imuData.fusionQPose.data( 0 );
		message.fusionQ2Pose	= _imuData.fusionQPose.data( 1 );
		message.fusionQ3Pose	= _imuData.fusionQPose.data( 2 );

		message.gyroValid		= _imuData.gyroValid;
		message.gyroX			= _imuData.gyro.data( 0 );
		message.gyroY			= _imuData.gyro.data( 1 );
		message.gyroZ			= _imuData.gyro.data( 2 );

		message.magValid		= _imuData.compassValid;
		message.magX			= _imuData.compass.data( 0 );
		message.magY			= _imuData.compass.data( 1 );
		message.magZ			= _imuData.compass.data( 2 );

		message.pressureValid	= _imuData.pressureValid;
		message.pressure		= _imuData.pressure;

		message.humidValid		= _imuData.humidity;

		message.tempValid		= _imuData.temperatureValid;
		message.temperature		= _imuData.temperature;

		DDS::ReturnCode_t ret = _eventWriter->write( message, DDS::HANDLE_NIL );

		if( ret != DDS::RETCODE_OK )
		{
			LOG( ERROR ) << "Error write returned: " << ret;
		}

		// Handle received messages
		HandleWaitCondition();
	}

	CleanUp();
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
 * for design changes. Comment out or remove what is not need. You can also create the subs and pubs
 * and not use them.
 */
void CAppNodeImpl::InitPublisherAndSubscriber()
{
	// Publisher
	_publisher = _participant->create_publisher( PUBLISHER_QOS_DEFAULT,
																	DDS::PublisherListener::_nil(),
																	OpenDDS::DCPS::DEFAULT_STATUS_MASK );
	if( !_publisher )
	{
		LOG( ERROR ) << "Create publisher failed.";
	}

	// Subscriber stub
//	_subscriber = _participant->create_subscriber( SUBSCRIBER_QOS_DEFAULT,
//													DDS::SubscriberListener::_nil(),
//													OpenDDS::DCPS::DEFAULT_STATUS_MASK );
//	if( !_subscriber )
//	{
//		LOG( ERROR ) << "Create subscriber failed.";
//	}
}

/**
 * Initializes the topic. You will need to modify and or add to this black for varying data types and
 * topics given your message requirements and QoS design decisions.
 */
void CAppNodeImpl::InitTopicinfo()
{
	// Type registration
	_imuDataTypeSupport = new ImuNodeApp::TImuData_var;

	// Exit if retcode ! ok
	if( DDS::RETCODE_OK != _imuDataTypeSupport->register_type( _participant, "" ) )
	{
		LOG( ERROR ) << "register type failed.";
	}

	// Create a topic
	_topicTypeName = _imuDataTypeSupport->get_type_name();

	_topic = _participant->create_topic( "ImuDataTopic", _topicTypeName.in(),
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

	_eventWriter = ImuNodeApp::ImuDataDataWriter::_narrow( _writer );

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
	// Data Reader stub
//	_listener = new CDataReaderListenerImpl;
//
//	_reader = _subscriber->create_datareader( _topic, DATAREADER_QOS_DEFAULT,
//												_listener,
//												OpenDDS::DCPS::DEFAULT_STATUS_MASK );
//	if( !_reader )
//	{
//		LOG( ERROR ) << " Create data reader failed";
//	}
//
//	_readerI = ImuNodeApp::DataReader::_narrow( _reader );
//
//	if( !_readerI )
//	{
//
//		LOG( ERROR ) << " _narrow failed";
//	}
}
