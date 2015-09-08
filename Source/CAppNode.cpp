#include "CAppNode.h"
#include "Utility/Utils.h"

CAppNode::CAppNode( int argc, ACE_TCHAR *argv[], std::string appNameIn ) : _appName( appNameIn )

{
	for( size_t i = 0; i < argc; ++i )
	{
		std::string temp( argv[i] );
		m_commandLineArguments.push_back( temp );
	}
	_psignalHandler = new CSignalHandler();
}

CAppNode::~CAppNode()
{
	SafeDelete( _psignalHandler );
}
