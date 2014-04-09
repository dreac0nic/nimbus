#include "EventSystem.h"
#include <vector>

using namespace std;
using namespace Nimbus;

EventSystem* EventSystem::singleton = NULL;

EventSystem::EventSystem(void)
{
	// Setup the singleton pointer.
	EventSystem::singleton = this;

	// Initialize the listener map.
	mListeners = map<EventType, vector<EventListener*> >();
}

EventSystem::~EventSystem(void)
{
	// Provide a warning just in case we are worried about event listeners getting unregistered.
	if(!mListeners.empty()) {
		cerr << " !!! CAUTION !!! EVENT SYSTEM: Not all listeners unregistered." << endl;
	}
}

bool EventSystem::registerListener(EventListener* listener, EventType type)
{
	mListeners[type].push_back(listener);

	return true;
}

void EventSystem::unregisterListener(EventListener* listener, EventType type)
{
	vector<EventListener*>::iterator item;

	for(item = mListeners[type].begin(); item != mListeners[type].end(); ++item) {
		if(listener == (*item)) { // ALL THE DOOMED COMPARISONS NEEDS MORE SYMBOLS
			break;
		}
	}

	mListeners[type].erase(item);
}

void EventSystem::fireEvent(EventType type, const payloadmap& payload, EventListener* responder)
{
	vector<EventListener*> temp = mListeners[type];

	for(std::vector<EventListener*>::iterator i = temp.begin(); i != temp.end(); ++i) {
		(*i)->handleEvent(payload, responder); // OH MY GOODNESS TEH SCIENCE
	}
}