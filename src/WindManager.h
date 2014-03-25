#ifndef NIMBUS_WINDMANAGER_H
#define NIMBUS_WINDMANAGER_H

#include <string>
#include <OgrePlane.h>

#include "EventListener.h"
#include "Manager.h"

namespace Nimbus
{
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

		/** Listens for the mouse path event
		*/
		class PathListener : 
			public EventListener
		{
		private:
			WindManager* mContainingManager;

		public:
			PathListener(WindManager* containingManager) { this->mContainingManager = containingManager; }
			virtual ~PathListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload);
		};

	public:
		WindManager(Ogre::SceneManager* sceneManager);
		virtual ~WindManager(void);

		virtual Ogre::Plane getWindPlane();

		// From Nimbus::Manager
		virtual bool update(void);
	};
}
#endif
