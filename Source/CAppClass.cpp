#include "CAppClass.h"

CAppClass::CAppClass( int argc, char *argv[], std::string appNameIn ) : _appName( appNameIn )
{
	for( size_t i = 0; i < argc; ++i )
	{
		std::string temp( argv[i] );
		m_commandLineArguments.push_back( temp );
	}
}

CAppClass::~CAppClass()
{

}
