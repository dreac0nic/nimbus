#include <sstream>
#include <OgreConfigFile.h>
#include <OgreLogManager.h>
#include "EntityFactory.h"
#include "BlankBehaviour.h"
#include "Renderable.h"

using namespace Nimbus;
using namespace Ogre;
using namespace std;

EntityFactory::EntityFactory(World* world, std::string filePathsFile)
{
	// Debugness! An logs!
	std::stringstream logBuilder;

	// Storing the world for future use
	this->mWorld = world;

	// LOAD ENTITY DEFINITIONS
	ConfigFile filePathsConfig;

	// The type of the current settings section
	string filePathsSectionType;

	// Load the behaviour prototype list
	this->mBehaviourInstances.clear();
	this->mBehaviourInstances["BlankBehaviour"] = new BlankBehaviour(world);
	this->mBehaviourInstances["Renderable"] = new Renderable(world);

	logBuilder << "(Nimbus) Loading game entity types from " << filePathsFile;
	LogManager::getSingleton().logMessage(logBuilder.str());
	logBuilder.str("");

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

				logBuilder << "(Nimbus)     Loading " << entityName << " from " << entityPath;
				LogManager::getSingleton().logMessage(logBuilder.str());
				logBuilder.str("");

				// Setup a new entity config file for the specific entity type
				ConfigFile entityConfig;
				string entitySectionType;
				entityConfig.load(entityPath);

				// Create a blank entity to morph into the prototype
				GameEntity* entity = new GameEntity();

				// Iterate through all the sections to initialize the prototype entity
				ConfigFile::SectionIterator entitySectionIterator = entityConfig.getSectionIterator();
				while(entitySectionIterator.hasMoreElements())
				{
					entitySectionType = entitySectionIterator.peekNextKey();

					ConfigFile::SettingsMultiMap* entitySettings = entitySectionIterator.getNext();

					// If this is the general settings section
					if(entitySectionType == "General")
					{
						logBuilder << "(Nimbus)         Setting general settings for " << entityName;
						LogManager::getSingleton().logMessage(logBuilder.str());
						logBuilder.str("");

						// Configure the entity itself
						entity->configure(entitySettings);
					}
					// Otherwise add the appropriate behaviour to the entity
					else if(entitySectionType != "")
					{
						logBuilder << "(Nimbus)         Adding the " << entitySectionType << " behaviour to " << entityName;
						LogManager::getSingleton().logMessage(logBuilder.str());
						logBuilder.str("");

						entity->add(this->mBehaviourInstances[entitySectionType]->clone(entitySettings));
					}
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