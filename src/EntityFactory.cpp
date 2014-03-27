// Includes in source ...
#include <sstream>
#include <OgreConfigFile.h>
#include <OgreLogManager.h>

#include "EntityFactory.h"
#include "./Behaviours/Renderable.h"
#include "./Behaviours/Positional.h"

using namespace Nimbus;
using namespace Ogre;
using namespace std;

Nimbus::EntityFactory::EntityFactory(World* world, std::string filePathsFile)
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
	this->mBehaviourInstances["Renderable"] = new Renderable("Renderable", world);
	this->mBehaviourInstances["Positional"] = new Positional("Positional", world);

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
				GameEntity* entity = new GameEntity(this->mWorld->getCurrentId(), entityName);

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

						entity->addBehaviour(this->mBehaviourInstances[entitySectionType]->clone(entitySettings),
							entitySectionType);
					}
				}

				// Adding the entity into the map
				this->mEntityInstances[entityName] = entity;
			}
		}
	}

	EventSystem::getSingleton()->registerListener(new CreateEntityListener(this), EventSystem::EventType::CREATE_ENTITY);
}

Nimbus::EntityFactory::~EntityFactory(void)
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

GameEntity* Nimbus::EntityFactory::createEntity(std::string entityType)
{
	GameEntity* factorizedEntity = new GameEntity(this->mWorld->getCurrentId(), this->mEntityInstances[entityType]);

	return factorizedEntity;
}

void Nimbus::EntityFactory::CreateEntityListener::handleEvent(payloadmap payload, EventListener* responder)
{
	if (payload.find("EntityType") != payload.end())
	{
		payloadmap positionalPayload;
		GameEntity* entity = containingFactory->createEntity(*(static_cast<string*>(payload["EntityType"])));
		containingFactory->mWorld->addEntity(entity);

		// If we are setting the position on creation
		if(payload.find("PositionVector") != payload.end() ||
			payload.find("FacingVector") != payload.end() ||
			payload.find("RotationVector") != payload.end())
		{
			// Get the entity id
			GameEntityId entityId = entity->getEntityId();
			positionalPayload["EntityId"] = &entityId;

			// Set an initial position vector if we have one
			if(payload.find("PositionVector") != payload.end())
			{
				positionalPayload["PositionVector"] = payload["PositionVector"];
			}

			// Set an initial facing vector if we have one
			if(payload.find("FacingVector") != payload.end())
			{
				positionalPayload["FacingVector"] = payload["FacingVector"];
			}

			// Set an initial rotation vector if we have one
			if(payload.find("RotationVector") != payload.end())
			{
				positionalPayload["RotationVector"] = payload["RotationVector"];
			}

			// Fire the event to set the position
			EventSystem::getSingleton()->fireEvent(EventSystem::EventType::POSITION_ENTITY, positionalPayload);
		}
	}

	// Cleaning up any unneeded payload memory space (why are we doing this?)
	/*payloadmap::iterator payloads = payload.begin();
	while (payloads != payload.end())
	{
		delete payloads->second;
		payloads++;
	}*/
}