#include "EntityManager.h"

using namespace Nimbus;
using namespace std;

EntityManager::EntityManager(void)
{
}

EntityManager::~EntityManager(void)
{
	delete this->mEntityFactory;
}

bool EntityManager::update(void)
{
	return true;
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);
}
