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

	GameEntitySet::TypeEntityIterator entityByType = entities->beginTypeIterator("Dragon");
	for(; entityByType != entities->endTypeIterator("Dragon"); ++entities)
	{
		stringstream debugstream("");
		debugstream << "(Nimbus) I found a draaaaaagon. <insert musical notation here in the key of G>\n";
		debugstream << "         " << entityByType->getEntityId() << ": " << entityByType->getEntityType() << "\n";
		Ogre::LogManager::getSingleton().logMessage(debugstream.str());
	}

	return true;
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);
}
