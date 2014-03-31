#ifndef NIMBUS_WINDMANAGER_H
#define NIMBUS_WINDMANAGER_H

#include <string>
#include <OgrePlane.h>
#include "EventListener.h"
#include "Manager.h"
#include "WindMap.h"

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

		// Setting up the plane that will register the clicks for the wind creation
		virtual void createClickPlane();

		// Event Listeners

		// Listens for the wind creation events
		class MouseWindListener : 
			public EventListener
		{
		private:
			WindManager* mContainingManager;
			Ogre::SceneManager* mSceneManager;
			int mCounter;

		public:
			MouseWindListener(WindManager* containingManager, Ogre::SceneManager* sceneManager)
				{ this->mContainingManager = containingManager; this->mSceneManager = sceneManager; mCounter = 0; }
			virtual ~MouseWindListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload);
		};

	private:
		// Stores the game's WindMap
		WindMap mWindMap;

	public:
		WindManager(Ogre::SceneManager* sceneManager, WindMap mWindMap);
		virtual ~WindManager(void);

		// From Nimbus::Manager
		virtual bool update(void);
	};
}
#endif
