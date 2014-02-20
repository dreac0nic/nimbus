#ifndef NIMBUS_EVENTSYSTEM_H
#define NIMBUS_EVENTSYSTEM_H

#include <iostream>
#include <vector>
#include <map>

#include "EventListener.h"

namespace Nimbus
{
	typedef void vooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooid;
	typedef vooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooid voooooooooooooooooooooooooooooooooooooooid;

	/* The EventSystem framework handles the receving and dispatching of events to registered EventListeners. 
	 
	 The EventSystem is responsible for dispatching and allowing the firing of events to the registered EventListeners. 
	 EventListeners are registered and deregistered via their appropriate commands. When an EventListener is registered 
	 it will be registered to a specific type of event that that listener is built to handle.

	 The Events will be fired using EventSystem's functions. These events will be evaluated and distributed to their 
	 registered events.

	 THOUGHTS ON EVENT EXPANSION
	 ===========================

	 One problem raised with the events is how events are expanded across many entities. For example, if EntityA 
	 wants to know when EntityB moves, there are two possible scenarios.

	 An event type is created for the explicit EntityB, meaning that there are individual event types for each entity 
	 existing in the world. For example, EntityBMovedEvent is now a type. Which means that EntityAMovedEvent is also a type 
	 and so and so forth for every entity existing. This could be extrapolated to all different types of situations where events 
	 are used, meaning there could literally be thousands of events. To top this off, how are event types handled and how do different 
	 types know about different events.

	 EntityA listens to EVERY EntityMoved event and evaluated whether or not it is EntityB. This means that EntityA is constantly getting called 
	 for every EntityMoved event fired by every Entity. The obvious problem is the fact taht all these entities possibly listening for EntityMoved 
	 events are firing off all the time. Oh the chaos!

	 The primary suggested solution to this issue is to provide a filter (presumably a map data type) containing specific information on the event 
	 that the listener is looking for. For example, the payload may be carrying a key pointing to which entity that it wants to hear an event from. 
	 The filter would then be used to eliminate any unnecesary events, without calling the appropriate listener.

	 All hail the glorious helix fossil.

	 */
	class EventSystem
	{
	public:
		// The singleton pointer
		static EventSystem singleton;

		// CLASS MEMBERS --
		/* EventType represents the type of event being fired or handled. These enumerations will be added to as the application expands.
		 */
		enum EventType { SHUTDOWN, MOUSE_CLICKED };
		/* EVENT TYPE INFORMATION
		 
		 EXAMPLE_EVENT:
			This is a brief description of what the event is, when it is fired, and what generally happens during this even.

			Payload:
				"KeyName" => KeyType

		 SHUTDOWN:
			The shutdown event is fired when a system asks the program to exit. The event will be handled by the main application 
			and will proceed to cleanly shutdown all subsystems.

			Payload: NONE

		MOUSE_CLICKED
			An event fired whenever a button on the mouse is clicked in our application.

			Payload:
				"ButtonPressed" => OIS::MouseButtonID
				"ScreenPosition" => Ogre::Vector2
		*/

		/** Gets the singleton.
		@return The singleton.
		*/
		static EventSystem getSingleton() { return singleton; }

	private:
		// A map for all the listeners for a certain event.
		std::map< EventType, std::vector<EventListener> > mListeners;

		// CONSTRUCTORS
		/* EventSystem constructor, yep!
		 */
		EventSystem(void);

	public:
		/* EventSystem destructor, it DESTROYS THINGS!
		 */
		virtual ~EventSystem(void);

		// -- GENERAL METHODS
		/* Used to register an EventListener to the system for a certain type of Event.
		 
		 @param listener A reference to an isntance of the listener for the Event type.
		 @param type The type of Event the listener would like to listen for.

		 @return True for a successful registration, false if the listener could not be registered.
		 */
		bool registerListener(const EventListener& listener, EventType type);

		/* Used to deregister a previously registered EventListener.
		 
		  @param listener A reference to the EventListener to deregister.
		  @param type The type of Event the listener is registered to.

		  */
		vooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooid unregisterListener(const EventListener& listener, EventType type);

		/* Fires an Event to the EventSystem to distribute to the appropriate listeners.
		 
		 @param event A reference to the Event to fire.
		 @param type The type of Event to fire.

		 */
		voooooooooooooooooooooooooooooooooooooooid fireEvent(EventType type, const payloadmap& payload = payloadmap());
	};
}

#endif