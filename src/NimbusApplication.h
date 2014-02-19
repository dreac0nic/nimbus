#ifndef NIMBUSAPPLICATION_H
#define NIMBUSAPPLICATION_H

#include "RunMode.h"
#include "SelfInitializingSingleton.h"
#include <OgreRoot.h>
#include <OgreFrameListener.h>

using namespace Ogre;

/** The driving application base for Nimbus.

Nimbus application is a singleton class as it is the base from which
the rest of the application runs. It is static for the purpose of making a
clean external interface. It should not be accessed directly by game internal
classes.

This application current handles much of the interface with Ogre.
Eventually, a lot of this should eventually be delegated to appropriate
subclasses. (e.g. input management should be given to InputManager)
 
One possible consideration we should take is making a main loop that drives
the application independent from Ogre. This would make our game more
independent rather than depending on Ogre for it's vital running
functionality.
*/
class NimbusApplication :
	public FrameListener,
	public SelfInitializingSingleton<NimbusApplication>
{
private:
	// Member Variables

	// The Ogre::Root object for this application
	Root* mRoot;
	// The Ogre::RenderWindow for this application
	RenderWindow* mWindow;

	// The current RunMode of the application
	RunMode* runMode;

	// Member Functions

	/** Loads all configuration files for the application.
	 
	This function creates the Ogre::Root object, initializes the resource
	groups, and sets up the RenderSystem.
	 
	@return Bool indicating success of loading.
	*/
	bool loadConfiguration(void);

protected:
	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const FrameEvent& evt);

public:
	/** Private default constructor to prevent instances of the
	NimbusApplication class.
	
	(This is clearly not private... demonstrating
	how SelfInitializingSingleton is broken. I think I might try an alternative.)
	*/
	NimbusApplication(void);

	virtual ~NimbusApplication(void);

	/** Starts the application running.
	
	The application will continue running according to Ogre::FrameListener
	triggers until internally terminated. (Specifically renderFrameQueued.)
	*/
	static void begin(void);
};

#endif
