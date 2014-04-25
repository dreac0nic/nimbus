#include "EventSystem.h"
#include "GenericHandler.h"
#include "EntityHandler.h"
#include <vector>
#include <map>

using namespace std;
using namespace Nimbus;

EventSystem* EventSystem::singleton = NULL;

EventSystem* EventSystem::initializeSingleton()
{
	singleton = new EventSystem();

	return singleton;
}

EventSystem::EventSystem(GameEntityId ownerId) :
	mOwnerId(ownerId),
	mGlobalListeners(),
	mHandlers()
{
	// Events which should be registered globally, but through the local event system
	EventType tempArray[] = {FLOCK_UPDATE, TRANSLATE_ENTITY, TRANSLATION_QUERY, TICK, SOAR_ENTITY};
	vector<EventType> globallySensitive(tempArray, tempArray + sizeof(tempArray) / sizeof(tempArray[0]));

	// Add them, uninitialized, to the global listeners list
	for(vector<EventType>::iterator it = globallySensitive.begin(); it != globallySensitive.end(); ++it)
	{
		mGlobalListeners[*it] = NULL;
	}
}

EventSystem::~EventSystem(void)
{
	// Provide a warning just in case we are worried about event listeners getting unregistered.
	if(!mHandlers.empty()) {
		cerr << " !!! CAUTION !!! EVENT SYSTEM: Not all listeners unregistered." << endl;
	}
}

bool EventSystem::registerListener(EventListener* listener, EventType type, filtermap filter)
{
	// If there are no handlers for this type of event
	if(mHandlers[type].size() <= 0)
	{
		// Make them
		makeHandlers(type);
	}

	// Tell all handlers for this type of event to register the listener according to the given filter
	for(vector<EventTypeHandler*>::iterator item = mHandlers[type].begin(); item != mHandlers[type].end(); ++item)
	{
		(*item)->registerListener(listener, filter);
	}

	// If not the global event system
	if(this->mOwnerId != 0)
	{
		// If there is not yet a global listener
		if(mGlobalListeners.find(type) != mGlobalListeners.end() && mGlobalListeners[type] == NULL)
		{
			// Create one
			mGlobalListeners[type] = new GlobalListener(this, type);
			
			// And register the created listener with the global system
			singleton->registerListener(mGlobalListeners[type], type, filter);
		}
	}

	return true;
}

void EventSystem::unregisterListener(EventListener* listener, EventType type, filtermap filter)
{
	// All handlers are empty (assume true until proven false)
	bool empty = true;

	// Tell all handlers for this type of event to unregister the listener according to the given filter
	for(vector<EventTypeHandler*>::iterator item = mHandlers[type].begin(); item != mHandlers[type].end(); ++item) {
		
		(*item)->unregisterListener(listener, filter);

		// If the handler is not empty
		if(!(*item)->isEmpty())
		{
			// Then indicate that not all handlers are empty for this type of event
			empty = false;
		}
	}

	// If not the global system and if empty
	if(mOwnerId != 0 && empty)
	{
		// Unregister from the global system
		singleton->unregisterListener(mGlobalListeners[type], type, filter);

		// Delete the listener corresponding to the given type
		delete mGlobalListeners[type];

		// Set the listener to NULL
		mGlobalListeners[type] = NULL;
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
	EventTypeHandler* handler;

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
		handler = new GenericHandler();
		break;

		// List of event which use an entity handler
	case Nimbus::EventSystem::SOAR_ENTITY:
	case Nimbus::EventSystem::TRANSLATE_ENTITY:
	case Nimbus::EventSystem::ENTITY_TRANSLATED:
	case Nimbus::EventSystem::TRANSLATION_QUERY:
	case Nimbus::EventSystem::FLOCK_UPDATE:
		// Create the handler
		handler = new EntityHandler();
		break;
	default:
		break;
	}

	// Add the handler to the local map
	this->mHandlers[type].push_back(handler);
}

void EventSystem::GlobalListener::handleEvent(payloadmap payload, EventListener* responder)
{
	this->mEventSystem->fireEvent(this->mType, payload, responder);
}