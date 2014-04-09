#include "GenericHandler.h"
#include <algorithm>

using namespace Nimbus;

GenericHandler::GenericHandler() :
	EventTypeHandler(HandlerTypes::GENERIC),
	mListeners()
{
}

GenericHandler::~GenericHandler()
{
}

void GenericHandler::handleEvent(payloadmap payload, EventListener* responder)
{
	// Go through each listener and handle the event
	for(std::list<EventListener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->handleEvent(payload, responder);
	}
}

void GenericHandler::registerListener(EventListener* listener, filtermap filter)
{
	// Add the listener to the list
	mListeners.push_back(listener);
}

void GenericHandler::unregisterListener(EventListener* listener, filtermap filter)
{
	// Find the listener in the list
	std::list<EventListener*>::iterator element = find(mListeners.begin(), mListeners.end(), listener);

	// If the element was found
	if(element != mListeners.end())
	{
		// Remove the element from the listeners
		mListeners.erase(element);
	}
}