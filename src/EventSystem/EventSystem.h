#ifndef NIMBUS_EVENTSYSTEM_H
#define NIMBUS_EVENTSYSTEM_H

#include <iostream>
#include <vector>
#include <map>

#include "EventTypeHandler.h"
#include "EventListener.h"

namespace Nimbus
{
	typedef int GameEntityId;

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

			TRANSLATE_ENTITY
			/*! The event that translates an entity in virtual world space. Used to move the entity.

				Payload:
					"EntityId" => int
					"PositionVector" => Ogre::Vector3	// Absolute, world position (optional)
					"PositionDelta" => Ogre::Vector3	// Relative position vector, applied per frame (optional)
					"RotationVector" => Ogre::Vector3	// Absolute, rotation vector <pitch, yaw, roll> (optional)
					"RotationDelta" => Ogre::Vector3	// Relative rotation vector, applied per frame (optional)
					"ScaleVector" => Ogre::Vector3		// Scale vector, should be absolute scale factor with 1.0 being original size (optional)
					"FacingVector" => Ogre::Vector3		// Facing vector, units in world absolute world space,
															begins at the object (optional)
			 */,

			ENTITY_TRANSLATED
			/*! The event that notifies behaviours when the entity is actually translated.

				Payload:
					"EntityId" => int
					"PositionVector" => Ogre::Vector3	// Absolute, world position (optional)
					"FacingVector" => Ogre::Vector3		// The direction the entity is currently facing (optional)
					"RotationVector" => Ogre::Vector3	// The rotation <pitch, yaw, roll> vector (optional)
					"ScaleVector" => Ogre::Vector3		// Scale vector, should be absolute scale factor with 1.0 being original size (optional)
			 */,

			 TRANSLATION_QUERY
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
			 /*! The event which lets a Flocking behaviour know its new entity list.
				
				Payload:
					"EntityId" => GameEntityId
					"EntityList" => list<GameEntityId>
			 */,

			  KEY_PRESS
			 /*! The event which lets a Flocking behaviour know its new entity list.
				
				Payload:
					"KeyCode" => KeyCode
					"KeyReleased" => Bool
			 */,

			  MOUSE_CLICKED
			 /*! The event which lets a Flocking behaviour know its new entity list.
				
				Payload:
					"MouseClicked" => MouseEvent
			 */
		};

		/** Initializes the global event system singleton.

			@return A pointer to the singleton just created.
		*/
		static EventSystem* initializeSingleton();

		/** Gets the singleton.

		 @return
		 The singleton.

		 NOTE: CAN return a null value if the EventSystem has not been initialized!!
		 */
		static EventSystem* getSingleton() { return singleton; }

	protected:

		/** Listens for global events and injects them in the local handlers as appropriate.
		*/
		class GlobalListener :
			public EventListener
		{
		private:
			/** The event system owning this listener. */
			EventSystem* mEventSystem;
			
			/** The event type listened for by this listener. */
			EventType mType;

		public:
			GlobalListener(EventSystem* eventSystem, EventType type) : mEventSystem(eventSystem), mType(type) {}
			virtual ~GlobalListener() {}

			// From Nimbus::EventListener
			void handleEvent(payloadmap payload, EventListener* responder = NULL);
            std::string str();
		};

	private:
		/** A map of event type to the corresponding event type handler. */
		std::map< EventType, std::vector<EventTypeHandler*> > mHandlers;

		/** A map of listeners to capture events from the global event system. */
		std::map<EventType, GlobalListener*> mGlobalListeners;

		/** The id of the owning entity. 0 if the global event system. */
		GameEntityId mOwnerId;

		/** Creates all the handlers needed for any given event type and registers
			them to the handler list for that event type

			@param
			type The type of event to create handlers for.
		*/
		void makeHandlers(EventType type);

        /** Convert the given EventType to a representative string. Useful for debugging purposes.

            @param eventType The EventType to convert.
            @return A std::string representing the EventType.
         */
        std::string eventTypeStr(EventType eventType);

	public:
		// CONSTRUCTORS
		/** EventSystem constructor, yep!
			
			@param
			ownerId The id of the owning entity.
		 */
		EventSystem(GameEntityId owner = 0);

		/** EventSystem destructor, it DESTROYS THINGS!
		 */
		virtual ~EventSystem(void);

		// -- GENERAL METHODS
		/** Used to register an EventListener to the system for a certain type of Event.
		 
		 @param
		 listener A reference to an isntance of the listener for the Event type.
		 @param
		 type The type of Event the listener would like to listen for.
		 @param
		 filter The filter for the events which this listener should listen to.

		 @return
		 True for a successful registration, false if the listener could not be registered.
		 */
		bool registerListener(EventListener* listener, EventType type, filtermap filter = filtermap());

		/** Used to deregister a previously registered EventListener.
		 
		  @param
		  listener A reference to the EventListener to deregister.
		  @param
		  type The type of Event the listener is registered to.
		  @param
		  filter The filter for the event which this listener is listening to.

		  */
		void unregisterListener(EventListener* listener, EventType type, filtermap filter = filtermap());

		/** Fires an Event to the EventSystem to distribute to the appropriate listeners.
		 
		 @param
		 event A reference to the Event to fire.
		 @param
		 type The type of Event to fire.
		 */
		void fireEvent(EventType type, const payloadmap& payload = payloadmap(), EventListener* responder = NULL);

        /** Prints the current event registrations to a string for debug purposes.

         @return A std::string containing the current contents of the EventSystem
         */
        std::string str();
	};
}

#endif
