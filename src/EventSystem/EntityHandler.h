#ifndef NIMBUS_ENTITY_HANDLER_H
#define NIMBUS_ENTITY_HANDLER_H

#include <list>
#include "EventTypeHandler.h"

namespace Nimbus
{
	// Redefine this here... if it causes problems delete it
	typedef int GameEntityId;

	/** Event type handler that filters events based on game entity id. */
	class EntityHandler :
		public EventTypeHandler
	{
	private:
		/** A map which associates a list of event listeners with a given game entity id. */
		std::map<GameEntityId, std::list<EventListener*> > mListeners;

	public:
		/** Creates an event handler which filters events based on entity id.
		*/
		EntityHandler();
		virtual ~EntityHandler();

		// From EventTypeHandler
		virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		virtual void registerListener(EventListener* listener, filtermap filter = filtermap());
		virtual void unregisterListener(EventListener* listener, filtermap filter = filtermap());
		virtual bool isEmpty();
	};
}

#endif