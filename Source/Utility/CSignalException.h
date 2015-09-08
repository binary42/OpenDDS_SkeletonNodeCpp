#pragma once
// From http://www.yolinux.com/TUTORIALS/C++Signals.html
#include <stdexcept>

class CSignalException : public std::runtime_error
{
public:
	CSignalException( const std::string &messageIn ): std::runtime_error( messageIn ){}
};

class CSignalHandler
{
protected:
	static bool mbGotExitSignal;

public:
	CSignalHandler();
	virtual ~CSignalHandler();

	// Attributes
	static bool GotExitSignal();
	static void SetExitSignal( bool exitSignalIn );

	// Attributes
	void SetupSignalHandlers();
	static void ExitSignalHandler( int ignoredIn );
};
