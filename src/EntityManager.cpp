#include <OgreVector2.h>
#include "EntityManager.h"

using namespace Nimbus;
using namespace std;

EntityManager::EntityManager(World* world)
{
	this->mWorld = world;

	// Create event listeners
	this->playerCreated = new PlayerCreatedListener(this);
	this->playerMoved = new PlayerMovedListener(this);
}

EntityManager::~EntityManager(void)
{
	delete this->playerMoved;
	delete this->playerCreated;

	delete this->mEntityFactory;
}

void EntityManager::initialize(void)
{
	// Create the player entity and catch it using the playerCreatedListener
	std::map<std::string, void*> entityType;
	entityType["EntityType"] = new std::string("Dragon");
	EventSystem::getSingleton()->fireEvent(EventSystem::CREATE_ENTITY, entityType, this->playerCreated);

	// Register listeners
	EventSystem::getSingleton()->registerListener(this->playerMoved, EventSystem::EventType::DIRECTION_INPUT);
}

bool EntityManager::update(void)
{
	vector<GameEntity*> entities = this->mWorld->getEntities();
	for (unsigned int x = 0; x < entities.size(); x++)
	{
		entities[x]->update();
	}
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
}

//////////
// Listener Implementations

void EntityManager::PlayerCreatedListener::handleEvent(payloadmap payload, EventListener* responder)
{
	parent->mPlayer = static_cast<GameEntity*>(payload["GameEntity"]);
}

void EntityManager::PlayerMovedListener::handleEvent(payloadmap payload, EventListener* responder)
{
	Ogre::Vector2* directionVector = NULL;

	if(payload.find("DirectionVector") != payload.end())
	{
		Ogre::Vector3 extraDimensionVector;
		payloadmap movePayload;
		GameEntityId playerId;

		// Get the directional vector
		directionVector = static_cast<Ogre::Vector2*>(payload["DirectionVector"]);

		// Convert the direction into 3d space
		extraDimensionVector = Ogre::Vector3(directionVector->x, 0, directionVector->y);

		// Get the player id in a referencable variable
		playerId = parent->mPlayer->getEntityId();

		// Send off an event to move the player
		movePayload["EntityId"] = &playerId;
		movePayload["PositionDelta"] = &extraDimensionVector;
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::TRANSLATE_ENTITY, movePayload);
	}
}