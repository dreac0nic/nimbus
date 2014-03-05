#include "EntityFactory.h"
#include "BlankBehaviour.h"
#include <OgreConfigFile.h>

using namespace Nimbus;
using namespace Ogre;
using namespace std;

EntityFactory::EntityFactory(World* world, std::string filePathsFile)
{
	// Storing the world for future use
	this->mWorld = world;

	// LOAD ENTITY DEFINITIONS
	ConfigFile filePathsConfig;

	// The type of the current settings section
	string filePathsSectionType;

	// Load the entity type config file
	// ../../assets/scripts/ConfigFiles.ini
	filePathsConfig.load(filePathsFile);

	// Iterate over each section of the config file
	ConfigFile::SectionIterator filePathsSectionIterator = filePathsConfig.getSectionIterator();
	while(filePathsSectionIterator.hasMoreElements())
	{
		filePathsSectionType = filePathsSectionIterator.peekNextKey();

		ConfigFile::SettingsMultiMap* filePathsSettings = filePathsSectionIterator.getNext();

		// If defining the entity types
		if(filePathsSectionType == "Entities")
		{
			// For each entity type in the section
			for(ConfigFile::SettingsIterator::iterator entityPaths = filePathsSettings->begin();
				entityPaths != filePathsSettings->end();
				++entityPaths)
			{
				// Store the entity name and the path to the entity file
				string entityName = entityPaths->first;
				string entityPath = entityPaths->second;

				// Setup a new entity config file for the specific entity type
				ConfigFile entityConfig;
				string entitySectionType;
				entityConfig.load(entityPath);
				ConfigFile::SectionIterator entitySectionIterator = entityConfig.getSectionIterator();
				while(entitySectionIterator.hasMoreElements())
				{
					entitySectionType = entitySectionIterator.peekNextKey();

					ConfigFile::SettingsMultiMap* entitySettings = entitySectionIterator.getNext();
				}
			}
			
		}
	}
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