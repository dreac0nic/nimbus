#include "EventSystem.h"

using namespace std;
using namespace Nimbus;

EventSystem EventSystem::singleton = EventSystem();

EventSystem::EventSystem(void)
{
	// Initialize the listener map.
	mListeners = map<EventType, vector<EventListener*>>();
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

vooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooid EventSystem::unregisterListener(EventListener* listener, EventType type)
{
	vector<EventListener*>::iterator item;

	for(item = mListeners[type].begin(); item != mListeners[type].end(); ++item) {
		if(listener == (*item)) { // ALL THE DOOMED COMPARISONS NEEDS MORE SYMBOLS
			break;
		}
	}

	mListeners[type].erase(item);
}

voooooooooooooooooooooooooooooooooooooooid EventSystem::fireEvent(EventType type, const payloadmap& payload)
{
	for(vector<EventListener*>::iterator i = mListeners[type].begin(); i != mListeners[type].end(); ++i) {
		(*i)->handleEvent(payload); // OH MY GOODNESS TEH SCIENCE
	}
}