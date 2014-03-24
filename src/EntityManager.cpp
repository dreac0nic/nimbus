#include "EntityManager.h"
#include "GameEntitySet.h"
#include <OgreLogManager.h>
#include <sstream>

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
	GameEntitySet::GeneralEntityIterator entity = entities->beginGeneralIterator(); 
	while (entity != entities->endGeneralIterator())
	{
		entity->update();
		entity++;
	}

	std::list<GameEntity*> dragons = entities->getEntitiesOfType("Dragon");
	for (std::list<GameEntity*>::iterator it = dragons.begin(); it != dragons.end(); it++)
	{
		Ogre::LogManager::getSingleton().logMessage("I found a dragon");
	}
	Ogre::LogManager::getSingleton().logMessage(entities->getEntity(4)->getEntityType());

	return true;
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);
}
