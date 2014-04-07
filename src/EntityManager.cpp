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
	delete this->mCreateEntityListener;
	delete this->mDestroyEventListener;
	delete this->mEntityFactory;
}

// Ensures world is initialized only after EntityManager is prepared
void EntityManager::initialize(void)
{
	EventSystem::getSingleton()->registerListener(this->mCreateEntityListener, EventSystem::EventType::CREATE_ENTITY);
	EventSystem::getSingleton()->registerListener(this->mDestroyEventListener, EventSystem::EventType::DESTROY_ENTITY);
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

	/** Example of how to access entities
	std::list<GameEntity*> dragons = entities->getEntitiesOfType("Dragon");
	for (std::list<GameEntity*>::iterator it = dragons.begin(); it != dragons.end(); it++)
	{
		Ogre::LogManager::getSingleton().logMessage("I found a dragon");
	}
	Ogre::LogManager::getSingleton().logMessage(entities->getEntity(4)->getEntityType());
	*/

	return true;
}

void EntityManager::pause(void)
{
}

void EntityManager::stop(void)
{
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);

	this->mCreateEntityListener = new CreateEntityListener(this->mEntityFactory, world);
	this->mDestroyEventListener = new DestroyEntityListener(world);
}

void Nimbus::EntityManager::CreateEntityListener::handleEvent(payloadmap payload, EventListener* responder)
{
	if (payload.find("EntityType") != payload.end())
	{
		payloadmap positionalPayload;
		GameEntity* entity = mFactory->createEntity(*(static_cast<string*>(payload["EntityType"])));
		mWorld->addEntity(entity);

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

		if(responder != NULL)
		{
			// Create the entity ID payload variables
			payloadmap entityIdPayload;
			GameEntityId entityId = entity->getEntityId();

			// Store the id in the payload
			entityIdPayload["EntityId"] = &entityId;

			// Call the responder
			responder->handleEvent(entityIdPayload);
		}
	}
}

void EntityManager::DestroyEntityListener::handleEvent(payloadmap payload, EventListener* responder)
{
	if(payload.find("EntityId") != payload.end())
	{
		GameEntityId entityId = *static_cast<GameEntityId*>(payload["EntityId"]);
		this->mWorld->getEntities()->removeGameEntity(entityId);
	}
}
