#include "EntityManager.h"
#include "GameEntitySet.h"

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
	GameEntitySet* entities = this->mWorld->getEntities();
	GameEntitySet::EntityIterator entity = entities->begin(); 
	while (entity != entities->end())
	{
		entity->update();
		entity++;
	}
	return true;
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);
}
