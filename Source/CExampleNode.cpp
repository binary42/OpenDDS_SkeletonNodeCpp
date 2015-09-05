#include "CExampleNode.h"

CExampleNode::CExampleNode( int argc, char *argv[], std::string appNameIn ):
	CAppClass( argc, argv, appNameIn )
{

}

CExampleNode::~CExampleNode()
{

}

void CExampleNode::Initialize()
{
	throw( "Initialize Failed" );
}

std::string CExampleNode::GetName()
{
	return nullptr;
}

void CExampleNode::Run()
{
	throw( "Run Failed" );
}

void CExampleNode::CleanUp()
{

}

void CExampleNode::InitializeWaitSet()
{

}

void CExampleNode::HandleWaitCondition()
{

}
