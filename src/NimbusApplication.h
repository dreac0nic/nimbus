#ifndef NIMBUSAPPLICATION_H
#define NIMBUSAPPLICATION_H

#include <OgreRoot.h>
#include <OgreFrameListener.h>

using namespace Ogre;

/** The driving application base for Nimbus.

This application current handles much of the interface with Ogre.
Eventually, a lot of this should eventually be delegated to appropriate
subclasses. (e.g. input management should be given to InputManager)
 
One possible consideration we should take is making a main loop that drives
the application independent from Ogre. This would make our game more
independent rather than depending on Ogre for it's vital running
functionality.
*/
class NimbusApplication : public FrameListener
{
private:
	// The singleton variable (this application overuses the Singleton design)
	static NimbusApplication app;

	// Member Variables

	// The Ogre::Root object for this application
	Root* mRoot;
	// The Ogre::RenderWindow for this application
	RenderWindow* mWindow;

	// Member Functions

	/** Loads all configuration files for the application.
	 
	This function creates the Ogre::Root object, initializes the resource
	groups, and sets up the RenderSystem.
	 
	@return Bool indicating success of loading.
	*/
	bool loadConfiguration(void);

	/** A test function which will be used to show that Ogre has been loaded
	properly. Requires that the mRoot has been initialized.
	
	DO NOT use this for final application. Instead, create the SceneManager
	in the appropriate RunMode. Each RunMode should probably have its own
	SceneManager and scene construction method.
	
	@return Bool indicating success of creating the scene.
	*/
	bool createScene(void);

	/** Private default constructor to prevent instances of the
	NimbusApplication class.
	*/
	NimbusApplication(void);

protected:
	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const FrameEvent& evt);

public:
	virtual ~NimbusApplication(void);

	/** Starts the application running.
	
	The application will continue running according to Ogre::FrameListener
	triggers until internally terminated. (Specifically renderFrameQueued.)
	*/
	static void begin(void);
};

#endif
