#ifndef NIMBUS_EVENTSYSTEM_H
#define NIMBUS_EVENTSYSTEM_H

#include <iostream>
#include <vector>
#include <map>

#include "EventListener.h"

namespace Nimbus
{
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
	private:
		// The singleton pointer
		static EventSystem* singleton;

	public:

		// CLASS MEMBERS --
		/* EventType represents the type of event being fired or handled. These enumerations will be added to as the application expands.
		 */
		enum EventType { SHUTDOWN, MOUSE_DOWN, MOUSE_UP, MOUSE_PATH, CREATE_ENTITY };
		/* EVENT TYPE INFORMATION

		 SHUTDOWN:
			The shutdown event is fired when a system asks the program to exit. The event will be handled by the main application 
			and will proceed to cleanly shutdown all subsystems.

			Payload: NONE

		MOUSE_DOWN
			An event fired whenever a button on the mouse is pressed down.

			Payload:
				"ButtonPressed" => OIS::MouseButtonID
				"ScreenPosition" => Ogre::Vector2

		MOUSE_UP
			An event fired whenever a button on the mouse is released.

			Payload:
				"ButtonPressed" => OIS::MouseButtonID
				"ScreenPosition" => Ogre::Vector2

		MOUSE_PATH
			An event fired whenever the path for a wind is finished being created.

			Payload:
				"Rays" => std::list<Ogre::Ray>
				"Points" => std::list<Ogre::Vector2>

		CREATE_ENTITY
			The event that causes the EntityFactory to produce a new entity of given type.

			Payload:
				"EntityType" => std::string
		*/

		/** Gets the singleton.

		 @return
		 The singleton.

		 NOTE: CAN return a null value if the EventSystem has not been initialized!!
		 */
		static EventSystem* getSingleton() { return singleton; }

	private:
		// A map for all the listeners for a certain event.
		std::map< EventType, std::vector<EventListener*> > mListeners;

	public:
		// CONSTRUCTORS
		/* EventSystem constructor, yep!
		 */
		EventSystem(void);

		/* EventSystem destructor, it DESTROYS THINGS!
		 */
		virtual ~EventSystem(void);

		// -- GENERAL METHODS
		/* Used to register an EventListener to the system for a certain type of Event.
		 
		 @param
		 listener A reference to an isntance of the listener for the Event type.
		 @param
		 type The type of Event the listener would like to listen for.

		 @return
		 True for a successful registration, false if the listener could not be registered.
		 */
		bool registerListener(EventListener* listener, EventType type);

		/* Used to deregister a previously registered EventListener.
		 
		  @param
		  listener A reference to the EventListener to deregister.
		  @param
		  type The type of Event the listener is registered to.

		  */
		void unregisterListener(EventListener* listener, EventType type);

		/* Fires an Event to the EventSystem to distribute to the appropriate listeners.
		 
		 @param
		 event A reference to the Event to fire.
		 @param
		 type The type of Event to fire.
		 */
		void fireEvent(EventType type, const payloadmap& payload = payloadmap());
	};
}

#endif