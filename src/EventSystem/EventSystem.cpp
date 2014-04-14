#include "EventSystem.h"
#include "GenericHandler.h"
#include "EntityHandler.h"
#include <vector>

using namespace std;
using namespace Nimbus;

EventSystem* EventSystem::singleton = NULL;

EventSystem* EventSystem::initializeSingleton()
{
	singleton = new EventSystem();

	return singleton;
}

EventSystem::EventSystem(GameEntityId ownerId) :
	mOwnerId(ownerId)
{
	// Setup the singleton pointer.
	EventSystem::singleton = this;

	// Initialize the listener map.
	mHandlers = map<EventType, vector<EventTypeHandler*> >();
}

EventSystem::~EventSystem(void)
{
	// Provide a warning just in case we are worried about event listeners getting unregistered.
	if(!mHandlers.empty()) {
		cerr << " !!! CAUTION !!! EVENT SYSTEM: Not all listeners unregistered." << endl;
	}
}

bool EventSystem::registerListener(EventListener* listener, EventType type, filtermap filter = filtermap())
{
	// Tell all handlers for this type of event to register the listener according to the given filter
	for(vector<EventTypeHandler*>::iterator item = mHandlers[type].begin(); item != mHandlers[type].end(); ++item)
	{
		(*item)->registerListener(listener, filter);
	}

	return true;
}

void EventSystem::unregisterListener(EventListener* listener, EventType type, filtermap filter = filtermap())
{
	// Tell all handlers for this type of event to unregister the listener according to the given filter
	for(vector<EventTypeHandler*>::iterator item = mHandlers[type].begin(); item != mHandlers[type].end(); ++item) {
		
		(*item)->unregisterListener(listener, filter);
	}
}

void EventSystem::fireEvent(EventType type, const payloadmap& payload, EventListener* responder)
{
	for(std::vector<EventTypeHandler*>::iterator i = this->mHandlers[type].begin(); i != this->mHandlers[type].end(); ++i) {
		(*i)->handleEvent(payload, responder); // OH MY GOODNESS TEH SCIENCE
	}
}

void EventSystem::makeHandlers(EventType type)
{
	switch (type)
	{
		// List of events which use a generic input handler
	case Nimbus::EventSystem::SHUTDOWN:
	case Nimbus::EventSystem::TICK:
	case Nimbus::EventSystem::MOUSE_DOWN:
	case Nimbus::EventSystem::MOUSE_UP:
	case Nimbus::EventSystem::MOUSE_UPDATE:
	case Nimbus::EventSystem::MOUSE_POSITION_UPDATE:
	case Nimbus::EventSystem::MOUSE_POSITION_START:
	case Nimbus::EventSystem::MOUSE_POSITION_END:
	case Nimbus::EventSystem::CREATE_ENTITY:
	case Nimbus::EventSystem::DESTROY_ENTITY:
		// Create the handler
		GenericHandler* handler = new GenericHandler();

		// If we are not the global event system
		if(mOwnerId != 0)
		{
			// Create the listener for the global event
			GlobalListener* listener = new GlobalListener(this, type);

			// Register for the global event
			EventSystem::getSingleton()->registerListener(listener, type);
		}

		// Add the handler to the local map
		this->mHandlers[type].push_back(handler);
		break;

		// List of event which use an entity handler
	case Nimbus::EventSystem::SOAR_ENTITY:
	case Nimbus::EventSystem::TRANSLATE_ENTITY:
	case Nimbus::EventSystem::ENTITY_TRANSLATED:
	case Nimbus::EventSystem::TRANSLATION_QUERY:
	case Nimbus::EventSystem::FLOCK_UPDATE:
		// Create the handler
		EntityHandler* handler = new EntityHandler();

		// If we are no the global event system
		if(mOwnerId != 0)
		{
			// Create the listener fo rthe global event
			GlobalListener* listener = new GlobalListener(this, type);

			// Register for the global event
			filtermap idFilter;
			idFilter["EntityId"] = &mOwnerId;
			EventSystem::getSingleton()->registerListener(listener, type, idFilter);
		}

		this->mHandlers[type].push_back(handler);
		break;
	default:
		break;
	}
}

void EventSystem::GlobalListener::handleEvent(payloadmap payload, EventListener* responder)
{
}