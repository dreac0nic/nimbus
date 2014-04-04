#ifndef NIMBUS_WINDMANAGER_H
#define NIMBUS_WINDMANAGER_H

#include <string>
#include <OgrePlane.h>
#include "EventListener.h"
#include "Manager.h"
#include "WindMap.h"
#include "WindCurrent.h"

namespace Nimbus
{
	const double STRENGTHTOSUBTRACT = 1;
	const double ORIGININFLUENCE = 0.5;
	const double CORNERINFLUENCE = .10355339059327;
	const double SIDEINFLUENCE = .1464466094073;

	/** Takes input and Updates WindMap
	*/
	class WindManager :
		public Manager
	{
	protected:
		// Member variables
		Ogre::SceneManager* mSceneManager;
		Ogre::Plane mWindPlane;
		WindCurrent tempCurrent;

		// Setting up the plane that will register the clicks for the wind creation
		virtual void createClickPlane();

		// Event Listeners

		// Listens for the wind update events
		class MouseWindUpdateListener : 
			public EventListener
		{
		private:
			WindManager* mContainingManager;
			int mCounter;

		public:
			MouseWindUpdateListener(WindManager* containingManager)
				{ this->mContainingManager = containingManager; mCounter = 0; }
			virtual ~MouseWindUpdateListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseWindUpdateListener;

		// Listens for the wind creation events
		class MouseWindStartListener : 
			public EventListener
		{
		private:
			WindManager* mContainingManager;
			int mCounter;

		public:
			MouseWindStartListener(WindManager* containingManager)
				{ this->mContainingManager = containingManager; mCounter = 0; }
			virtual ~MouseWindStartListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseWindStartListener;

		// Listens for the wind creation events
		class MouseWindEndListener : 
			public EventListener
		{
		private:
			WindManager* mContainingManager;
			int mCounter;

		public:
			MouseWindEndListener(WindManager* containingManager)
				{ this->mContainingManager = containingManager; mCounter = 0; }
			virtual ~MouseWindEndListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseWindEndListener;

	private:
		// Stores the game's WindMap
		WindMap mWindMap;

	public:
		WindManager(Ogre::SceneManager* sceneManager, WindMap* mWindMap);
		virtual ~WindManager(void);

		// From Nimbus::Manager
		virtual void initialize(void) {}
		virtual bool update(void);
	};
}
#endif
