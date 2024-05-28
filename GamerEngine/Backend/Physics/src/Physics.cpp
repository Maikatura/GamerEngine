#include "Physics.h"

#include "Utilites/VisualProfiler.h"


Physics::Physics()
{
}

Physics::~Physics()
{
	
}

Physics& Physics::Get()
{
	static Physics instance;
	return instance;
}

void Physics::Init()
{
	PROFILE_SCOPE("Physics::Init");
}

void Physics::Update()
{
	
}
