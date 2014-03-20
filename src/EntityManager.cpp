#include "EntityManager.h"

using namespace Nimbus;
using namespace std;

EntityManager::EntityManager(World* world)
{
	this->mWorld = world;
}

EntityManager::~EntityManager(void)
{
	delete this->mEntityFactory;
}

void EntityManager::initialize(void)
{
}

bool EntityManager::update(void)
{
	vector<GameEntity*> entities = this->mWorld->getEntities();
	for (unsigned int x = 0; x < entities.size(); x++)
	{
		entities[x]->update();
	}
	return true;
}

void EntityManager::pause(void)
{
}

void EntityManager::stop(void)
{
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);
}
