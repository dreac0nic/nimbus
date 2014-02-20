#ifndef NIMBUS_EVENTLISTENER_H
#define NIMBUS_EVENTLISTENER_H

#include <string>
#include <map>

namespace Nimbus 
{
	/* EventListener is a pure virtual class outlining a class that handles specific events.
	 
	 The idea behind EventListener being pure virtual means that any object can create
	 its own subclass of EventListenter, providing handling for their own specific events.
	 This allows the EventListener to handle its parent's member variables and functions, 
	 while providing a simple layer of inheritence for the EventListener.
	 */
	class EventListener
	{
	public:
		/* handleEvent is called when the listener is required to handle an event.
		 This function should be overridden and implemented in order to handle the specific
		 event this handler is destined to handle.

		 @param payload A C++ STL map using a string key and a void* for data storage.
		 */
		virtual void handleEvent(std::map<std::string, void*> payload) = 0;
	};
}

#endif