#include "CSignalException.h"
#include "Utils.h"

#include <signal.h>
#include <errno.h>

bool CSignalHandler::mbGotExitSignal = false;

CSignalHandler::CSignalHandler()
{

}

CSignalHandler::~CSignalHandler()
{

}

/**
* Returns the bool flag indicating whether we received an exit signal
* @return Flag indicating shutdown of program
*/
bool CSignalHandler::GotExitSignal()
{
    return mbGotExitSignal;
}

/**
* Sets the bool flag indicating whether we received an exit signal
*/
void CSignalHandler::SetExitSignal( bool exitSignalIn )
{
    mbGotExitSignal = exitSignalIn;
}

/**
* Sets exit signal to true.
* @param[in] _ignored Not used but required by function prototype
*                     to match required handler.
*/
void CSignalHandler::ExitSignalHandler( int ignoredIn )
{
    mbGotExitSignal = true;
}

/**
* Set up the signal handlers for CTRL-C.
*/
void CSignalHandler::SetupSignalHandlers()
{
    if ( signal( (int) SIGINT, CSignalHandler::ExitSignalHandler ) == SIG_ERR )
    {
        throw CSignalException( "!!!!! Error setting up signal handlers !!!!!" );
    }
}
