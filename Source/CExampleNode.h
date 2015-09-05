#pragma once

#include "CAppClass.h"

class CExampleNode : CAppClass
{
public:
	CExampleNode( int argc, char *argv[], std::string appNameIn );
	virtual ~CExampleNode();

	// Methods
	void Initialize();
	std::string GetName();
	void Run();
	void CleanUp();
	//Attributes

private:
	// Methods
	void InitializeWaitSet();
	void HandleWaitCondition();
	//Attributes
};
