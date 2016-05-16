#include "CAppNode.h"
#include "Utility/Utils.h"

/**
 * Constructor. Accepts the command line arguments and application string represetning the app name.
 * @param		argc				int type argument count
 * @param		argv[]				ACE_TCHAR type pointer containing arguments
 * @param		appNameIn			string name of application
 */
CAppNode::CAppNode( int argc, ACE_TCHAR *argv[], std::string appNameIn ) : _appName( appNameIn )

{
	for( size_t i = 0; i < argc; ++i )
	{
		std::string temp( argv[i] );
		m_commandLineArguments.push_back( temp );
	}
	_psignalHandler = new CSignalHandler();
}

/**
 *	Destructor. Deletes the _psignalhandle pointer
 */
CAppNode::~CAppNode()
{
	SafeDelete( _psignalHandler );
}


void CAppNode::on_requested_deadline_missed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedDeadlineMissedStatus& /*status*/)
{

}

void CAppNode::on_requested_incompatible_qos(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedIncompatibleQosStatus& /*status*/)
{

}

void CAppNode::on_sample_rejected(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleRejectedStatus& /*status*/)
{

}

void CAppNode::on_liveliness_changed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::LivelinessChangedStatus& /*status*/)
{

}
//--------
// Modify to fit user specific messages and types
//-------
void CAppNode::on_data_available(DDS::DataReader_ptr reader)
{
  ExampleApp::EventDataReader_var reader_i =
		  ExampleApp::EventDataReader::_narrow(reader);

  if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

  ExampleApp::EventSeq messages;
  DDS::SampleInfoSeq info;

  DDS::ReturnCode_t error = reader_i->take(messages,
                                           info,
                                           DDS::LENGTH_UNLIMITED,
                                           DDS::ANY_SAMPLE_STATE,
                                           DDS::ANY_VIEW_STATE,
                                           DDS::ANY_INSTANCE_STATE);

  if (error == DDS::RETCODE_OK) {
//    std::cout << "SampleInfo.sample_rank = " << info[0].sample_rank << std::endl;
//    std::cout << "SampleInfo.instance_state = " << info[0].instance_state << std::endl;
//
//    if (info[0].valid_data) {
//      std::cout << "Message: subject    = " << messages[0].subject.in() << std::endl
//                << "         subject_id = " << messages[0].subject_id   << std::endl
//                << "         from       = " << messages[0].from.in()    << std::endl
//                << "         count      = " << messages[0].count        << std::endl
//                << "         text       = " << messages[0].text.in()    << std::endl;

//    }
	  std::cout << "SampleInfo " << messages[0].kicker;

  } else {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" take failed!\n")));
  }

  reader_i->return_loan(messages, info);
}

void CAppNode::on_subscription_matched(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SubscriptionMatchedStatus& /*status*/)
{

}

void CAppNode::on_sample_lost(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleLostStatus& /*status*/)
{
}
