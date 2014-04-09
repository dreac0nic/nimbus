#ifndef NIMBUS_EVENTSYSTEM_H
#define NIMBUS_EVENTSYSTEM_H

#include <iostream>
#include <vector>
#include <map>

#include "EventListener.h"

namespace Nimbus
{
	/** The EventSystem framework handles the receving and dispatching of events to registered EventListeners. 
	 
	 The EventSystem is responsible for dispatching and allowing the firing of events to the registered EventListeners. 
	 EventListeners are registered and deregistered via their appropriate commands. When an EventListener is registered 
	 it will be registered to a specific type of event that that listener is built to handle.

	 The Events will be fired using EventSystem's functions. These events will be evaluated and distributed to their 
	 registered events.

	 All hail the glorious helix fossil.

	 */
	class EventSystem
	{
	private:
		// The singleton pointer
		static EventSystem* singleton;

	public:

		// CLASS MEMBERS --
		/*! EventType represents the type of event being fired or handled. These enumerations will be added to as the application expands.
		 */
		enum EventType {
			SHUTDOWN
			/*! The shutdown event is fired when a system asks the program to exit. The event will be handled by the main application 
				and will proceed to cleanly shutdown all subsystems.

				Payload: NONE
			 */,
			
			TICK
			/*! An event which fires at a given rate so that more expensive operations may be spaced out.
				
				Payload: NONE
			 */,

			MOUSE_DOWN
			/*! An event fired whenever a button on the mouse is pressed down.

				Payload:
					"ButtonPressed" => OIS::MouseButtonID
					"ScreenPosition" => Ogre::Vector2
			 */,

			MOUSE_UP
			/*! An event fired whenever a button on the mouse is released.

				Payload:
					"ButtonPressed" => OIS::MouseButtonID
					"ScreenPosition" => Ogre::Vector2
			 */,

			MOUSE_UPDATE
			/*! An event fired whenever the mouse moves.

				Payload:
					"ScreenPosition" => Ogre::Vector2
			 */,

			MOUSE_POSITION_UPDATE
			/*! An event fired whenever the mouse moves.

				Payload:
					"Context" => std::string
					"ScreenPosition" => Ogre::Vector2
					"WorldRay" => Ogre::Ray
			 */,
			 
			MOUSE_POSITION_START
			/*! An event fired whenever the mouse moves.

				// May not include a payload
				Payload:
					"Context" => std::string
					"ScreenPosition" => Ogre::Vector2
					"WorldRay" => Ogre::Ray
			*/,
			
			MOUSE_POSITION_END
			/*! An event fired whenever the mouse moves.

				// May not include a payload
				Payload:
					"Context" => std::string
					"ScreenPosition" => Ogre::Vector2
					"WorldRay" => Ogre::Ray
			*/,

			CREATE_ENTITY
			/*! The event that causes the EntityFactory to produce a new entity of given type.

				Payload:
					"EntityType" => std::string
					"PositionVector" => Ogre::Vector3	// Absolute, world position (optional)
					"FacingVector" => Ogre::Vector3		// Facing vector, units in world absolute world space,
															begins at the object... have fun interpretting that (optional)
					"RotationVector" => Ogre::Vector3	// Absolute, rotation vector <pitch, yaw, roll> (optional)

				Responder Payload:
					"EntityId" => GameEntityId
			 */,

			DESTROY_ENTITY
			/*! The event that causes an entity to be dropped from the world and destroyed. (Don't worry... it's humane.)

				Payload:
					"EntityId" => GameEntityId
			 */,

			SOAR_ENTITY
			/*! The event which carries wind induced movement information. Handled by the flocking group associated with the given entity.

				Payload:
					"EntityId" => GameEntityId
					"PositionDelta" => Ogre::Vector3
			 */,

			POSITION_ENTITY
			/*! The event that positions an entity in absolute world space. This can be useful for spawning, etc.

				Payload:
					"EntityId" => int
					"PositionVector" => Ogre::Vector3	// Absolute, world position (optional)
					"FacingVector" => Ogre::Vector3		// Facing vector, units in world absolute world space,
															begins at the object (optional)
					"RotationVector" => Ogre::Vector3	// Absolute, rotation vector <pitch, yaw, roll> (optional)
			 */,

			BEGIN_TRANSLATE_ENTITY
			/*! The event that causes an entity to move from one place to another. Generally speaking, this is the more
				appropriate event to use to cause an entity to move as it is blended with other movement during a frame
				while PositionEntity is not blended at all.

				Payload:
					"EntityId" => int
					"PositionDelta" => Ogre::Vector3	// Relative position vector (optional)
					"RotationDelta" => Ogre::Vector3	// Relative rotation vector (optional)
					"FacingVector" => Ogre::Vector3		// Facing vector, units in world space, begins at object...
															there is no relative facing vector... while all facing
															vectors are relative... it's complicated (optional)
			 */,

			END_TRANSLATE_ENTITY
			/*! The event that causes an entity to stop moving in the given direction. Simply stops translating the entity.

				Payload:
					"EntityId" => int
					"EndTranslate" => NULL
			 */,

			ENTITY_MOVED
			/*! The event that notifies behaviours when the position of an entity is updated.

				Payload:
					"EntityId" => int
					"PositionVector" => Ogre::Vector3	// Absolute, world position (optional)
					"FacingVector" => Ogre::Vector3		// The direction the entity is currently facing (optional)
					"RotationVector" => Ogre::Vector3	// The rotation <pitch, yaw, roll> vector (optional)
			 */,

			 POSITION_QUERY
			 /*! The event that requests about an entity's position. Quite possibly a very bad idea to use en masse...
				which is, of course, what I intend to do (for lack of a better idea).

				Payload:
					"EntityId" => int

				Responder Payload:
					"PositionVector" => Ogre::Vector3	// Absolute, world position
					"FacingVector" => Ogre::Vector3		// The direction the entity is currently facing
					"RotationVector" => Ogre::Vector3	// The rotation <pitch, yaw, roll> vector
			 */,

			 FLOCK_UPDATE
			 /*! The event which lets a Flocking behaviour know it's new entity list.
				
				Payload:
					"EntityId" => GameEntityId
					"EntityList" => list<GameEntityId>
			 */
		};

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
		/** EventSystem constructor, yep!
		 */
		EventSystem(void);

		/** EventSystem destructor, it DESTROYS THINGS!
		 */
		virtual ~EventSystem(void);

		// -- GENERAL METHODS
		/** Used to register an EventListener to the system for a certain type of Event.
		 
		 @param
		 listener A reference to an isntance of the listener for the Event type.
		 @param
		 type The type of Event the listener would like to listen for.

		 @return
		 True for a successful registration, false if the listener could not be registered.
		 */
		bool registerListener(EventListener* listener, EventType type);

		/** Used to deregister a previously registered EventListener.
		 
		  @param
		  listener A reference to the EventListener to deregister.
		  @param
		  type The type of Event the listener is registered to.

		  */
		void unregisterListener(EventListener* listener, EventType type);

		/** Fires an Event to the EventSystem to distribute to the appropriate listeners.
		 
		 @param
		 event A reference to the Event to fire.
		 @param
		 type The type of Event to fire.
		 */
		void fireEvent(EventType type, const payloadmap& payload = payloadmap(), EventListener* responder = NULL);
	};
}

#endif
