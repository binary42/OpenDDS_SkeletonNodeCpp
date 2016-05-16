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

/**
 * Data Reader impl. Not sure if this is a good place for them. modify as needed.
 */
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
