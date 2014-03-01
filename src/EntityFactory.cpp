#include "EntityFactory.h"
#include <OgreConfigFile.h>

using namespace Nimbus;
using namespace Ogre;
using namespace std;

EntityFactory::EntityFactory(std::string entityDefinitionFile)
{
	// LOAD ENTITY DEFINITIONS
	ConfigFile entityConfig;

	// The current entity
	GameEntity* currentEntity = NULL;
	string currentEntityTypeName;

	// The type of the current settings section
	string sectionType;

	// Load the entity type config file
	// ../../assets/scripts/EntityTypes.ini
	entityConfig.load(entityDefinitionFile);

	// Iterate over each section of the config file
	ConfigFile::SectionIterator sectionIterator = entityConfig.getSectionIterator();
	while(sectionIterator.hasMoreElements())
	{
		sectionType = sectionIterator.peekNextKey();

		ConfigFile::SettingsMultiMap* settings = sectionIterator.getNext();

		// If defining a new entity type
		if(sectionType == "Entity")
		{
			if(currentEntity != NULL)
			{
				this->mEntityInstances[currentEntityTypeName] = currentEntity;

				currentEntity = NULL;
			}

			currentEntity = new GameEntity(settings);
			currentEntityTypeName = currentEntity->getEntityType();
		}
	}
}

EntityFactory::~EntityFactory(void)
{
	// EMPTY DESTRUCTOR
}

GameEntity* EntityFactory::createEntity(std::string entityType)
{
	GameEntity* factorizedEntity = new GameEntity;

	return factorizedEntity;
}