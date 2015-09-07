/**
 * Main program entry.
 */

#include "Utility/Utils.h"
#include "CExampleNode.h"

#define ELOG_CONFIG_PATH "Config/log.conf"
#define DOMAIN_ID 0
#define APP_NAME "ExampleApp"

INITIALIZE_EASYLOGGINGPP

int main( int argc, ACE_TCHAR *argv[] )
{
	// Load logger config file
	el::Configurations logConfig( ELOG_CONFIG_PATH );

	// Set configuration
	el::Loggers::reconfigureAllLoggers( logConfig );

	//std::unique_ptr<CExampleNode> application( new CExampleNode( argc, argv, APP_NAME, DOMAIN_ID ) );

	CExampleNode *application = new CExampleNode( argc, argv, APP_NAME, DOMAIN_ID );

	LOG( INFO ) << "<-------------------------------->";
	LOG( INFO ) << "<--------" << application->GetName();
	LOG( INFO ) << "<-------------------------------->";

	try
	{
		application->Initialize();

		application->Run();

	}catch( std::exception &excpt )
	{
		LOG( ERROR ) << "Exception starting application: " << application->GetName() << " " << excpt.what();

		// Clean up
		application->CleanUp();
	}

	return 0;
}
