#ifndef NIMBUSAPPLICATION_H
#define NIMBUSAPPLICATION_H

#include "RunMode.h"
#include "InputManager.h"
#include "EventSystem/EventSystem.h"
#include <OgreRoot.h>
#include <OgreFrameListener.h>

namespace Nimbus
{
	/** The driving application base for Nimbus.

	 This application current handles much of the interface with Ogre.
	 Eventually, a lot of this should eventually be delegated to appropriate
	 subclasses. (e.g. input management should be given to InputManager)
 
	 One possible consideration we should take is making a main loop that drives
	 the application independent from Ogre. This would make our game more
	 independent rather than depending on Ogre for its vital running
	 functionality.
	 */
	class NimbusApplication : public Ogre::FrameListener
	{
	private:
		// Class/Static Members

		// The singleton variable (this application overuses the Singleton design)
		static NimbusApplication* app;

	public:
		/** Starts the application running.
	
		 The application will continue running according to Ogre::FrameListener
		 triggers until internally terminated. (Specifically renderFrameQueued.)
		 */
		static void begin(void);

		/** Gets the Ogre::RenderWindow for the current application.

		 @return
		 A pointer to the Ogre::RenderWindow of NimbusApplication.
		 */
		static Ogre::RenderWindow* getRenderWindow(void);

	private:
		// Member Variables

		// The Ogre::Root object for this application
		Ogre::Root* mRoot;
		// The Ogre::RenderWindow for this application
		Ogre::RenderWindow* mWindow;

		// The InputManager (This probably goes here since if it is independent of RunMode)
		InputManager* mInputManager;

		// The EventSystem handle, for future reference and cleanup.
		EventSystem* mEventSystem;

		// The current RunMode of the application
		RunMode* mCurrentRunMode;

		// Member Functions

		/** Loads all configuration files for the application.
	 
		 This function creates the Ogre::Root object, initializes the resource
		 groups, and sets up the RenderSystem.
	 
		 @return
		 Bool indicating success of loading.
		 */
		bool loadConfiguration(void);

	protected:
		// From Ogre::FrameListener
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		// Event Listeners

		/** Listens for the shutdown event
		*/
		class ShutdownListener : 
			public EventListener
		{
		public:
			ShutdownListener() {}
			virtual ~ShutdownListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
            virtual std::string str();
		};

	public:
		NimbusApplication(void);

		virtual ~NimbusApplication(void);
	};
}

#endif
