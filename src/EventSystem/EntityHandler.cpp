#include "EntityHandler.h"
#include <algorithm>
#include <sstream>

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
	// If the filter contains no entity id
	else
	{
		// Add the listener to the generic list
		mListeners[0].push_back(listener);
	}

}

void EntityHandler::unregisterListener(EventListener* listener, filtermap filter)
{
	std::list<EventListener*>::iterator element;

	GameEntityId entityId;

	// Delete the listener from the indicated entity id list
	if(filter.find("EntityId") != filter.end())
	{
		// Get the game entity id
		entityId = *static_cast<GameEntityId*>(filter["EntityId"]);
	}
	else
	{
		// Or from the generic list, if no entity Id was specified
		entityId = 0;
	}

	// Find the listener in the given list
    element = std::find(mListeners[entityId].begin(), mListeners[entityId].end(), listener);

	// If the listener was found
	if(element != mListeners[entityId].end())
	{
		// Remove the listener
		mListeners[entityId].erase(element);
	}
}

bool EntityHandler::isEmpty()
{
	// For every id in the handler
	for(std::map<GameEntityId, std::list<EventListener*> >::iterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		// If the list contains something
		if(it->second.size() > 0)
		{
			// Then the handler is not empty
			return false;
		}
	}

	// Otherwise, the handler is empty
	return true;
}

std::string EntityHandler::str()
{
    std::stringstream output;

    for(std::map<GameEntityId, std::list<EventListener*> >::iterator listenerList = this->mListeners.begin(); listenerList != this->mListeners.end(); ++listenerList)
    {
        output << "GameEntityId: " << listenerList->first << std::endl;

        for(std::list<EventListener*>::iterator listener = listenerList->second.begin(); listener != listenerList->second.end(); ++listener)
        {
            output << "  " << (*listener)->str() << std::endl;
        }
    }

    return output.str();
}
