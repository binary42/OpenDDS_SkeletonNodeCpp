#include "CDataReaderListenerImpl.h"

CDataReaderListenerImpl::CDataReaderListenerImpl()
{

}

CDataReaderListenerImpl::~CDataReaderListenerImpl()
{

}

void CDataReaderListenerImpl::on_requested_deadline_missed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedDeadlineMissedStatus& /*status*/)
{

}

void CDataReaderListenerImpl::on_requested_incompatible_qos(
  DDS::DataReader_ptr /*reader*/,
  const DDS::RequestedIncompatibleQosStatus& /*status*/)
{

}

void CDataReaderListenerImpl::on_sample_rejected(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleRejectedStatus& /*status*/)
{

}

void CDataReaderListenerImpl::on_liveliness_changed(
  DDS::DataReader_ptr /*reader*/,
  const DDS::LivelinessChangedStatus& /*status*/)
{

}
//--------
// Modify to fit user specific messages and types
//-------
void CDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
  ImuNodeApp::TImuDataDataReader_var reader_i =
		  ImuNodeApp::TImuDataDataReader::_narrow(reader);

  if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

  ImuNodeApp::TImuDataSeq messages;
  DDS::SampleInfoSeq info;

  DDS::ReturnCode_t error = reader_i->take(messages,
                                           info,
                                           DDS::LENGTH_UNLIMITED,
                                           DDS::ANY_SAMPLE_STATE,
                                           DDS::ANY_VIEW_STATE,
                                           DDS::ANY_INSTANCE_STATE);

  if (error == DDS::RETCODE_OK) {
	  // Send data to IMU app here

	  // Debug output
	  std::cout << "SampleInfo " << messages[0].name;

  } else {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" take failed!\n")));
  }

  reader_i->return_loan(messages, info);
}

void CDataReaderListenerImpl::on_subscription_matched(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SubscriptionMatchedStatus& /*status*/)
{

}

void CDataReaderListenerImpl::on_sample_lost(
  DDS::DataReader_ptr /*reader*/,
  const DDS::SampleLostStatus& /*status*/)
{

}
