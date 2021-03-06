#ifndef NIMBUS_EVENT_TYPE_HANDLER_H
#define NIMBUS_EVENT_TYPE_HANDLER_H

#include "EventListener.h"

namespace Nimbus
{
	/** A map for storing filter information for Event Handlers. */
	typedef payloadmap filtermap;

	/** Handles a specific type of event according to the specific
		rules defined in the given implementation.
		This is the parent class for handlers of specific types of events.
		To determine the type (implementation) of handler, examine the mHandlerType value.
	*/
	class EventTypeHandler
	{
	public:
		/*! An enum to distinguish types of event handlers */
		enum HandlerTypes
		{
			GENERIC
			/*! Used for generically handling any type of event.
				
				No filter
			*/,

			ENTITY
			/*! Used for filtering events based on game entity id.
				
				Filter:
					"EntityId" => GameEntityId
			*/,

			ENTITY_GROUP
			/*! Used for filtering events based on a group of game entity ids.
				
				Filter:
					"EntityIds" => std::list<GameEntityId>
			*/
		} mHandlerType;

	protected:
		EventTypeHandler(HandlerTypes type) : mHandlerType(type) {}

	public:

		virtual ~EventTypeHandler() {}

		/** Handle the event as efficiently as possible for this type of event.
			This function enables us to write specialized filters for each type
			of event that needs a special filter.
		*/
		virtual void handleEvent(payloadmap payload, EventListener* responder) = 0;

		/** Registers a listener with the event handler.
			@param listener A pointer to the event listener which should be registered.
			@param filter A filtermap which contains the filter for the event handler to classify the listener.
		*/
		virtual void registerListener(EventListener* listener, filtermap filter = filtermap()) = 0;

		/** Unregisters a listener with the event handler.
			@param listener A pointer to the event listener which should be unregistered.
			@param filter A filtermap which contains the filter for the event handler to classify the listener.
		*/
		virtual void unregisterListener(EventListener* listener, filtermap filter = filtermap()) = 0;

		/** Indicates if the handler is empty of listeners.

			@return True if the handler contains no listeners.
		*/
		virtual bool isEmpty() = 0;

        /** Generates a string representing the state of the event handler.

            @return A std::string representing the state of the event handler.
         */
        virtual std::string str() = 0;
	};
}

#endif
