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
