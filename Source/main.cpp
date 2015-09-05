/**
 * Main program entry.
 */

#include "Utility/Utils.h"
#include "CExampleNode.h"

#define ELOG_CONFIG_PATH "Config/log.conf"

INITIALIZE_EASYLOGGINGPP

int main( int argc, char *argv[] )
{
	// Load logger config file
	el::Configurations logConfig( ELOG_CONFIG_PATH );

	// Set configuration
	el::Loggers::reconfigureAllLoggers( logConfig );

	std::unique_ptr<CExampleNode> application( new CExampleNode( argc, argv, "ExampleApp" ) );

	LOG( INFO ) << "<-------------------------------->";
	LOG( INFO ) << "<--------" << application->GetName() << "-------->";
	LOG( INFO ) << "<-------------------------------->";

	try
	{
		application->Initialize();

		application->Run();

	}catch( std::exception &excpt )
	{
		LOG( ERROR ) << "Exception starting application: " << application->GetName() << " " << excpt.what();

		application->CleanUp();
	}


	return 0;
}
