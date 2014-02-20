#ifndef NIMBUSAPPLICATION_H
#define NIMBUSAPPLICATION_H

#include "RunMode.h"
#include "InputManager.h"
#include "EventListener.h"
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
	independent rather than depending on Ogre for it's vital running
	functionality.
	*/
	class NimbusApplication : public Ogre::FrameListener
	{
	private:
		// The singleton variable (this application overuses the Singleton design)
		static NimbusApplication app;

		// Member Variables

		// The Ogre::Root object for this application
		Ogre::Root* mRoot;
		// The Ogre::RenderWindow for this application
		Ogre::RenderWindow* mWindow;

		// The InputManager (This probably goes here since if it is independent of RunMode)
		InputManager* mInputManager;

		// The current RunMode of the application
		RunMode* mCurrentRunMode;

		// Member Functions

		/** Loads all configuration files for the application.
	 
		This function creates the Ogre::Root object, initializes the resource
		groups, and sets up the RenderSystem.
	 
		@return Bool indicating success of loading.
		*/
		bool loadConfiguration(void);

		/** Private default constructor to prevent outside instances of the
		NimbusApplication class.
		*/
		NimbusApplication(void);

	protected:
		// Ogre::FrameListener
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
			virtual void handleEvent(payloadmap payload);
		};

	public:
		virtual ~NimbusApplication(void);

		/** Starts the application running.
	
		The application will continue running according to Ogre::FrameListener
		triggers until internally terminated. (Specifically renderFrameQueued.)
		*/
		static void begin(void);

		/** A hack function designed to emulate an exit event.
		Kills the application. Replace with appropriate event system equivalent.
		*/
		//static void exitEvent(void) { app.mCurrentRunMode = 0; }
	};
}

#endif
