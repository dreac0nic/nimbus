#ifndef NIMBUS_GENERIC_HANDLER_H
#define NIMBUS_GENERIC_HANDLER_H

#include <list>
#include "EventTypeHandler.h"

namespace Nimbus
{
	/** A simple, generic handler for any events that don't require (or permit) special filtering.
	*/
	class GenericHandler :
		public EventTypeHandler
	{
	private:

		/** The listeners for this type of event. */
		std::list<EventListener*> mListeners;

	public:
		/** Creates a generic handler.
		*/
		GenericHandler();
		virtual ~GenericHandler();

		// From Nimbus::EventTypeHandler
		virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		virtual void registerListener(EventListener* listener, filtermap filter = filtermap());
		virtual void unregisterListener(EventListener* listener, filtermap filter = filtermap());
		virtual bool isEmpty();
	};
}

#endif