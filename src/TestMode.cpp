#include "TestMode.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>

TestMode::TestMode(RenderWindow* window):
	mWindow(window)
{
}

TestMode::~TestMode(void)
{
}

RunMode* TestMode::run(const FrameEvent& evt)
{
	// Attempt to initialize the run mode
	if(!this->initialized && !this->initialize())
	{
		LogManager::getSingletonPtr()->logMessage("(Nimbus) Failed to initialize TestMode");

		// Terminate the application if we fail to initialize
		return 0;
	}

	// Continue to run this runmode
	return this;
}

bool TestMode::initialize()
{
	Entity* dragon;
	SceneNode* dragonNode;

	Light* light;

	// Create the scene manager
	mSceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager");

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	//////////
	// Set up the camera

	// Position the camera
	mCamera->setPosition(Vector3(0,50,80));
	mCamera->lookAt(Vector3(0,0,-100));
	mCamera->setNearClipDistance(5);

	// Add a viewport for the camera
	mViewport = mWindow->addViewport(mCamera);

	//////////
	// Set up the appropriate models

	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

	// Load the dragon
	dragon = mSceneMgr->createEntity("Dragon", "dragon.mesh");
	dragonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	dragonNode->attachObject(dragon);

	dragonNode->setPosition(0,0,-100);
	dragonNode->setScale(40.0, 40.0, 40.0);
	dragonNode->pitch(Degree(90));

	//////////
	// Set up light sources

	// Set the ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));

	// Create a point light for main light
	light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
