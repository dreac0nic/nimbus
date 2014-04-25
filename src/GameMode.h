#ifndef NIMBUS_GAMEMODE_H
#define NIMBUS_GAMEMODE_H

#include "RunMode.h"
#include "EnvironmentManager.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "World.h"
#include "EventSystem/EventSystem.h"
#include "Camera.h"

namespace Nimbus
{
	/** RunMode that drives the game.

	 This RunMode contains the world object which contains all information about
	 the game world (the environment and the entities). This RunMode is also
	 responsible for the managers which run the game.
	 */
	class GameMode :
		public RunMode
	{
	private:
		// Member variables
		// *******************************************************************

		Camera* mCamera;

		// The Environment Manager
		EnvironmentManager* mEnvironmentMan;

		// The Entity Manager
		EntityManager* mEntityMan;

		// The game World
		World* mWorld;

		// Elapsed time since tick
		Ogre::Real elapsedTime;

		//MenuMode Pointer
		RunMode* menuModePointer;

	protected:
		
		// The time step between each tick
		Ogre::Real timePerTick;

		// Wind path creation
		bool mCreatingWind;

		// Event Listeners
		// *******************************************************************

		// Listens for any mouse down events
		class MouseDownListener : 
			public EventListener
		{
		private:
			GameMode* mContainingMode;

		public:
			MouseDownListener(GameMode* containingMode)
				{ this->mContainingMode = containingMode; }
			virtual ~MouseDownListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseDownListener;

		// Listens for any mouse update events
		class MouseUpdateListener : 
			public EventListener
		{
		private:
			GameMode* mContainingMode;

		public:
			MouseUpdateListener(GameMode* containingMode)
				{ this->mContainingMode = containingMode; }
			virtual ~MouseUpdateListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseUpdateListener;

		// Listens for any mouse up events
		class MouseUpListener : 
			public EventListener
		{
		private:
			GameMode* mContainingMode;

		public:
			MouseUpListener(GameMode* containingMode)
				{ this->mContainingMode = containingMode; }
			virtual ~MouseUpListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseUpListener;

		//Listens for Key Events
		class KeyListener: 
			public EventListener
		{
		public:
			KeyListener() {}
			virtual ~KeyListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL );
		} *keyListener;

	public:
		GameMode(void);
		virtual ~GameMode(void);

		// From Nimbus::RunMode
		virtual void initialize();
		virtual RunMode* run(const Ogre::FrameEvent& evt);
		virtual void pause();
		virtual void stop();
	};

}

#endif
