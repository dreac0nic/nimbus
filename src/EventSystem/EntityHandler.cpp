#include "EntityHandler.h"

using namespace Nimbus;

EntityHandler::EntityHandler() :
	EventTypeHandler(HandlerTypes::ENTITY),
	mListeners()
{
}

EntityHandler::~EntityHandler()
{
}

void EntityHandler::handleEvent(payloadmap payload, EventListener* responder)
{
	std::list<EventListener*>* listeners;

	// If the payload has an entity id
	if(payload.find("EntityId") != payload.end())
	{
		// Get the id
		GameEntityId entityId = *static_cast<GameEntityId*>(payload["EntityId"]);

		// Get the list corresponding to the entity id
		listeners = &mListeners[entityId];
	}
	// If the payload has no entity id
	else
	{
		// Execute the generic listener list
		listeners = &mListeners[0];
	}

	// Fire the event for each listener in the given list
	for(std::list<EventListener*>::iterator it = listeners->begin(); it != listeners->end(); ++it)
	{
		(*it)->handleEvent(payload, responder);
	}
}

void EntityHandler::registerListener(EventListener* listener, filtermap filter)
{
	// If the filter contains an entity id
	if(filter.find("EntityId") != filter.end())
	{
		// Get the entity id
		GameEntityId entityId = *static_cast<GameEntityId*>(filter["EntityId"]);

		// Add the listener to the entity specific list
		mListeners[entityId].push_back(listener);
	}

	// Register for the global case if not only local
	if(filter.find("LocalOnly") == filter.end())
	{
		// If the listener hasn't yet been added to the global list
		if(std::find(mListeners[0].begin(), mListeners[0].end(), listener) == mListeners[0].end())
		{
			// Add the listener to the global list
			mListeners[0].push_back(listener);
		}
	}
}

void EntityHandler::unregisterListener(EventListener* listener, filtermap filter)
{
	std::list<EventListener*>::iterator element;

	// Delete the listener from the indicated entity id list
	if(filter.find("EntityId") != filter.end())
	{
		// Get the game entity id
		GameEntityId entityId = *static_cast<GameEntityId*>(filter["EntityId"]);

		// Find the listener in the given list
		element = find(mListeners[entityId].begin(), mListeners[entityId].end(), listener);

		// If the listener was found
		if(element != mListeners[entityId].end())
		{
			// Remove the listener
			mListeners[entityId].erase(element);
		}
	}

	// Delete the global case if not only local
	if(filter.find("LocalOnly") == filter.end())
	{
		// Find the listener in the global list
		element = find(mListeners[0].begin(), mListeners[0].end(), listener);

		// If the listener was found
		if(element != mListeners[0].end())
		{
			// Remove the listener
			mListeners[0].erase(element);
		}
	}
}