#include "EnvironmentManager.h"

using namespace Nimbus;

EnvironmentManager::EnvironmentManager(void)
{
	mWindManager = new WindManager();
}

EnvironmentManager::~EnvironmentManager(void)
{
}

bool EnvironmentManager::update(void)
{
	return true;
}
