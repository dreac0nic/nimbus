#include "EntityFactory.h"
#include "BlankBehaviour.h"
#include "Renderable.h"
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

	/* The current entity
	GameEntity* currentEntity = NULL;
	GameEntityType currentEntityType;

	Behaviour* tempBehaviour = NULL;*/

	// Load the behaviour prototype list
	this->mBehaviourInstances["BlankBehaviour"] = new BlankBehaviour(world);

	// Load the entity type config file
	// ../../assets/scripts/ConfigFiles.ini
	filePathsConfig.load(filePathsFile);

	// Iterate over each section of the config file
	ConfigFile::SectionIterator filePathsSectionIterator = filePathsConfig.getSectionIterator();
	while(filePathsSectionIterator.hasMoreElements())
	{
		filePathsSectionType = filePathsSectionIterator.peekNextKey();

		ConfigFile::SettingsMultiMap* filePathsSettings = filePathsSectionIterator.getNext();

		// If defining a new entity type
		if(filePathsSectionType == "Entities")
		{
			/*if(currentEntity != NULL)
			{
				this->mEntityInstances[currentEntityType] = currentEntity;

				currentEntity = NULL;
			}

			currentEntity = new GameEntity(settings);
			currentEntityType = currentEntity->getEntityType();*/

			GameEntity* currentEntity = new GameEntity();
			Behaviour* tempBehaviour = NULL;

			ConfigFile::SettingsMultiMap::iterator entityPaths = filePathsSettings->begin();
			string entityName = entityPaths->first;
			string entityPath = entityPaths->second;

			ConfigFile entityConfig;
			string entitySectionType;
			entityConfig.load(entityPath);
			
			ConfigFile::SectionIterator entitySectionIterator = entityConfig.getSectionIterator();
			while(entitySectionIterator.hasMoreElements())
			{
				entitySectionType = entitySectionIterator.peekNextKey();

				ConfigFile::SettingsMultiMap* entitySettings = entitySectionIterator.getNext();

				if (entitySectionType == "General")
				{
					currentEntity->initialize(entitySettings);
				}
				else if (entitySectionType == "Renderable")
				{
					if (this->mBehaviourInstances.find(entitySectionType) == this->mBehaviourInstances.end())
					{
						this->mBehaviourInstances["Renderable"] = new Nimbus::Renderable(world, entitySettings);
					}
					tempBehaviour = this->mBehaviourInstances[entitySectionType]->clone(entitySettings);
					currentEntity->add(tempBehaviour);
				}
			}
		}
		/* If defining a behaviour for the current entity type
		else if(sectionType == "Behaviour")
		{
			if(currentEntity != NULL)
			{
				// ConfigFile::SettingsMultiMap::iterator settingIterator = settings->find("name");
				tempBehaviour = this->mBehaviourInstances[(*(settings->find("name"))).second]->clone(settings);
				currentEntity->add(tempBehaviour);
			}
		}*/
	}

	// Store the last entity type loaded
	//this->mEntityInstances[currentEntityType] = currentEntity;
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