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

void CDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
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