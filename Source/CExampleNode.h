#pragma once

#include "CAppClass.h"

class CExampleNode : CAppClass
{
public:
	CExampleNode( int argc, char *argv[], std::string appNameIn, int domainIDIn );
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
	// OpenDDS Domain id
	DDS::DomainId_t 				_domainID;

	int								_argCount;
	char							*_pargVect[];
};
