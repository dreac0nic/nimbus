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
	delete this->mCatchEntityListener;
	delete this->mDestroyEventListener;
	delete this->mTickListener;
	delete this->mPositionResponseListener;
	delete this->mEntityFactory;
}

void EntityManager::initialize(void)
{
	EventSystem::getSingleton()->registerListener(this->mCreateEntityListener, EventSystem::EventType::CREATE_ENTITY);
	EventSystem::getSingleton()->registerListener(this->mDestroyEventListener, EventSystem::EventType::DESTROY_ENTITY);
	EventSystem::getSingleton()->registerListener(this->mTickListener, EventSystem::EventType::TICK);
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
	EventSystem::getSingleton()->unregisterListener(this->mCreateEntityListener, EventSystem::EventType::CREATE_ENTITY);
	EventSystem::getSingleton()->unregisterListener(this->mDestroyEventListener, EventSystem::EventType::DESTROY_ENTITY);
	EventSystem::getSingleton()->unregisterListener(this->mTickListener, EventSystem::EventType::TICK);
}

void EntityManager::stop(void)
{
	EventSystem::getSingleton()->unregisterListener(this->mCreateEntityListener, EventSystem::EventType::CREATE_ENTITY);
	EventSystem::getSingleton()->unregisterListener(this->mDestroyEventListener, EventSystem::EventType::DESTROY_ENTITY);
	EventSystem::getSingleton()->unregisterListener(this->mTickListener, EventSystem::EventType::TICK);
}

void EntityManager::configureEntityTypes(string entityTypesFile, World* world)
{
	this->mEntityFactory = new EntityFactory(world, entityTypesFile);

	this->mCreateEntityListener = new CreateEntityListener(this->mEntityFactory, world);
	this->mCatchEntityListener = new CatchEntityListener();
	this->mDestroyEventListener = new DestroyEntityListener(world);
	this->mTickListener = new TickListener(this);
	this->mPositionResponseListener = new PositionResponseListener();
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

		// If there is a responder, send a response
		if(responder != NULL)
		{
			// Set up necessary payload variables
			payloadmap responderPayload;
			GameEntityId entityId = entity->getEntityId();

			// Load the payload
			responderPayload["EntityId"] = &entityId;

			// Fire off the response
			responder->handleEvent(responderPayload);
		}
	}
}

void EntityManager::CatchEntityListener::handleEvent(payloadmap payload, EventListener* responder)
{
	if(payload.find("EntityId") != payload.end())
	{
		this->mEntityId = *static_cast<GameEntityId*>(payload["EntityId"]);
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

void EntityManager::TickListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Refresh cloud groups
	generateCloudGroups();
}

void EntityManager::TickListener::generateCloudGroups()
{
	// Get all entities with the Soaring behaviour
	list<GameEntity*> clouds = mParent->mWorld->getEntities()->getEntitiesWithBehaviour("Soaring");

	// Create an adjacency matrix
	Grid<Ogre::Real> proximityGrid(clouds.size(), clouds.size());

	// A map to store the groups discovered by the cluster algorithm
	map<int, list<GameEntityId> > groups;

	// Query variables
	payloadmap queryPayload;
	int entityId;
	map<GameEntityId, Ogre::Vector3> positions;

	// Preload the query payload (since it's a pointer, it should update with the changing values)
	queryPayload["EntityId"] = &entityId;

	// Get the position of all the entities
	list<GameEntity*>::iterator currentCloud;
	for(currentCloud = clouds.begin(); currentCloud != clouds.end(); ++currentCloud)
	{
		// Fire off a position query
		entityId = (*currentCloud)->getEntityId();
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::POSITION_QUERY, queryPayload, mParent->mPositionResponseListener);

		// Store the position
		positions[entityId] = mParent->mPositionResponseListener->getPosition();
	}

	// Create the initial adjacency matrix
	// Initialize the top right portion (assuming top and left 0 indices) to take advantage of triangularization
	map<GameEntityId, Ogre::Vector3>::iterator currentCloudPosition;
	map<GameEntityId, Ogre::Vector3>::iterator compareCloudPosition;
	currentCloudPosition = positions.begin();
	for(unsigned int x = 0; x < positions.size(); ++x)
	{
		// Store the cloud's entity id and column number
		groups[x].push_back(currentCloudPosition->first);

		// Reset the compare cloud
		compareCloudPosition = positions.begin();

		for(unsigned int y = 0; y < x; ++y)
		{
			// Calculate the distance between the two clouds
			proximityGrid.set(x, y, currentCloudPosition->second.distance(compareCloudPosition->second));

			// Store the other side of the grid as well
			proximityGrid.set(y, x, proximityGrid.get(x, y));

			// Compare to the next cloud
			++compareCloudPosition;
		}

		// Go to the next cloud
		++currentCloudPosition;
	}

	// Begin the cluster algorithm
	cluster(proximityGrid, groups);

	// Debug output
	stringstream logger;
	logger << "Cloud groups generated\n";
	logger << "Found " << groups.size() << " groups\n";
	for(map<int, list<GameEntityId> >::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		logger << "    " << it->first << ": ";
		for(list<GameEntityId>::iterator listIt = it->second.begin(); listIt != it->second.end(); ++listIt)
		{
			logger << *listIt << " ";
		}
		logger << endl;
	}
	Ogre::LogManager::getSingleton().logMessage(logger.str());

	// Update the appropriate cloud group
	payloadmap groupUpdatePayload;
	GameEntityId groupEntityId;
	for(map<int, list<GameEntityId> >::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		payloadmap updateCloudGroupPayload;

		// If there is not currently a cloud group for this number, create it
		if(mParent->cloudGroups.find(it->first) == mParent->cloudGroups.end())
		{
			// Load a create payload
			payloadmap createCloudGroupPayload;
			string type = "CloudGroup"; // Hmm... hard code much?
			createCloudGroupPayload["EntityType"] = &type;

			// Fire the event with a catch responder
			EventSystem::getSingleton()->fireEvent(
				EventSystem::EventType::CREATE_ENTITY,
				createCloudGroupPayload,
				mParent->mCatchEntityListener);

			// Get the new entity's id and store it
			mParent->cloudGroups[it->first] = mParent->mCatchEntityListener->getEntityId();
		}

		// Load the update event payload
		updateCloudGroupPayload["EntityId"] = &mParent->cloudGroups[it->first];
		updateCloudGroupPayload["EntityList"] = &it->second;

		// Fire off the update event
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::FLOCK_UPDATE, updateCloudGroupPayload);
	}
}

void EntityManager::TickListener::cluster(Grid<float>& proximityGrid, map<int, list<GameEntityId> >& groups)
{
	int minX = 0, minY = 0;

	//////////
	// Find the next two clusters to join based on minimum distance

	// Find the minimum distance between two clusters
	for(int x = 0; x < proximityGrid.getXDimension(); ++x)
	{
		for(int y = 0; y < x; ++y)
		{
			if(minX == minY ||
				(proximityGrid.get(x, y) < proximityGrid.get(minX, minY) && proximityGrid.get(x,y) > 0))
			{
				minX = x;
				minY = y;
			}
		}
	}

	// If there are no clusters close enough together,
	// or all clouds have been clustered, then we are done
	if(//proximityGrid.getXDimension() == proximityGrid.getYDimension() ||
		proximityGrid.getXDimension() <= 1 ||
		proximityGrid.get(minX, minY) > maxGroupingDistance)
	{
		return;
	}

	// If equal, no switch is necessary
	if(minY == minX)
	{
		return;
	}
	// Make sure that minX < minY
	else if(minY < minX)
	{
		int temp = minY;
		minY = minX;
		minX = temp;
	}

	//////////
	// Reconfigure the proximityGrid

	// Create a new grid based on the old one with the minY group merged into minX
	Grid<float> tempGrid(proximityGrid.getXDimension()-1, proximityGrid.getYDimension()-1);

	// Merge minX and minY
	for(int y = 0; y < proximityGrid.getYDimension(); ++y)
	{
		Ogre::Real valueX = proximityGrid.get(minX, y);
		Ogre::Real valueY = proximityGrid.get(minY, y);

		if(valueX <= valueY)
		{
			proximityGrid.set(minX, y, valueX);
			proximityGrid.set(y, minX, valueX);
		}
		else
		{
			proximityGrid.set(minX, y, valueY);
			proximityGrid.set(y, minX, valueY);
		}
	}

	// Delete both the minY column and the minY row
	for(int x = 0; x < minY; ++x)
	{
		for(int y = 0; y < x && y < minY; ++y)
		{
			tempGrid.set(x,y, proximityGrid.get(x,y));
			tempGrid.set(y,x, proximityGrid.get(x,y));
		}

		for(int y = minY; y < x; ++y)
		{
			tempGrid.set(x, y, proximityGrid.get(x, y+1));
			tempGrid.set(y, x, proximityGrid.get(x, y+1));
		}
	}

	for(int x = minY; x < tempGrid.getXDimension(); ++x)
	{
		for(int y = 0; y < x && y < minY; ++y)
		{
			tempGrid.set(x,y, proximityGrid.get(x+1,y));
			tempGrid.set(y,x, proximityGrid.get(x+1,y));
		}

		for(int y = minY; y < x; ++y)
		{
			tempGrid.set(x, y, proximityGrid.get(x+1, y+1));
			tempGrid.set(y, x, proximityGrid.get(x+1, y+1));
		}
	}

	// Store the new grid so that it can be returned
	proximityGrid = tempGrid;

	//////////
	// Reconfigure the groups

	// Merge group minY into group minX
	for(list<GameEntityId>::iterator it = groups[minY].begin(); it != groups[minY].end(); ++it)
	{
		groups[minX].push_back(*it);
	}

	// Rename all groups after minY to accommodate the loss
	unsigned int i;
	for(i = minY; i < groups.size() - 1; ++i)
	{
		groups[i] = groups[i+1];
	}

	// Erase the final group (it's extra)
	groups.erase(groups.find(i));

	//////////
	// Continue to recurse until all groups are found
	cluster(proximityGrid, groups);
}

void EntityManager::PositionResponseListener::handleEvent(payloadmap payload, EventListener* responder)
{
	if(payload.find("PositionVector") != payload.end())
	{
		this->mPosition = *static_cast<Ogre::Vector3*>(payload["PositionVector"]);
	}
	else
	{
		this->mPosition = Ogre::Vector3::ZERO;
	}
}
