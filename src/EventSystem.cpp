#include "EventSystem.h"

using namespace std;
using namespace Nimbus;

EventSystem EventSystem::singleton = EventSystem();

EventSystem::EventSystem(void)
{
	// Initialize the listener map.
	mListeners = map<EventType, vector<EventListener>>();
}

EventSystem::~EventSystem(void)
{
	// Provide a warning just in case we are worried about event listeners getting unregistered.
	if(!mListeners.empty()) {
		cerr << " !!! CAUTION !!! EVENT SYSTEM: Not all listeners unregistered." << endl;
	}
}

bool EventSystem::registerListener(const EventListener& listener, EventType type)
{
}

vooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooid EventSystem::unregisterListener(const EventListener& listener, EventType type)
{
}

voooooooooooooooooooooooooooooooooooooooid EventSystem::fireEvent(EventType type, const payloadmap& payload = payloadmap())
{
}