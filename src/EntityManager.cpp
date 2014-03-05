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

bool EntityManager::update(void)
{
	vector<GameEntity*> entities = this->mWorld->getEntities();
	for (int x = 0; x < entities.size(); x++)
	{
		entities[x]->update();
	}
	return true;
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);
}
