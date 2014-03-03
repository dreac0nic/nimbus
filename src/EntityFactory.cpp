#include "EntityFactory.h"
#include "BlankBehaviour.h"
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
	GameEntityType currentEntityType;

	Behaviour* tempBehaviour = NULL;

	// The type of the current settings section
	string sectionType;

	// Load the behaviour prototype list
	this->mBehaviourInstances["BlankBehaviour"] = new BlankBehaviour();

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
				this->mEntityInstances[currentEntityType] = currentEntity;

				currentEntity = NULL;
			}

			currentEntity = new GameEntity(settings);
			currentEntityType = currentEntity->getEntityType();
		}
		// If defining a behaviour for the current entity type
		else if(sectionType == "Behavior")
		{
			if(currentEntity != NULL)
			{
				// ConfigFile::SettingsMultiMap::iterator settingIterator = settings->find("name");
				tempBehaviour = this->mBehaviourInstances[(*(settings->find("name"))).second]->clone(settings);
				currentEntity->add(tempBehaviour);
			}
		}
	}

	// Store the last entity type loaded
	this->mEntityInstances[currentEntityType] = currentEntity;
}

EntityFactory::~EntityFactory(void)
{
	// Delete all prototype Entities
	for(std::map<GameEntityType, GameEntity*>::iterator i = this->mEntityInstances.begin(); i != this->mEntityInstances.end(); ++i)
	{
		delete i->second;
	}

	// Delete all prototype Behaviours
	for(std::map<std::string, Behaviour*>::iterator i = this->mBehaviourInstances.begin(); i != this->mBehaviourInstances.end(); ++i)
	{
		delete i->second;
	}
}

GameEntity* EntityFactory::createEntity(std::string entityType)
{
	GameEntity* factorizedEntity = new GameEntity;

	return factorizedEntity;
}