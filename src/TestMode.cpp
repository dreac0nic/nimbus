#include "TestMode.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "NimbusApplication.h"
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>

using namespace Nimbus;
using namespace Ogre;

TestMode::TestMode(void)
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

	// Overlay stuffz
	Ogre::OverlaySystem* overlaySystem = new Ogre::OverlaySystem;
	mSceneMgr->addRenderQueueListener(overlaySystem);

	Ogre::OverlayManager* mOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

	Ogre::Overlay* theOverlay = mOverlayMgr->create("theOverlay");

	Ogre::OverlayElement* theOneElementToRuleThemAll = mOverlayMgr->createOverlayElement("TextArea", "cowtype");
	theOneElementToRuleThemAll->setMetricsMode(Ogre::GMM_PIXELS);
	theOneElementToRuleThemAll->setPosition(20,20);
	theOneElementToRuleThemAll->setDimensions(100, 100);

	theOneElementToRuleThemAll->setCaption("This is not text");

	Ogre::OverlayContainer* theContainer = static_cast<Ogre::OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "TheContainer"));
	theContainer->setMetricsMode(Ogre::GMM_PIXELS);
	theContainer->setPosition(10,10);

	theContainer->addChild(theOneElementToRuleThemAll);

	theOverlay->add2D(theContainer);

	theOverlay->show();

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	//////////
	// Set up the camera

	// Position the camera
	mCamera->setPosition(Vector3(0, 50, 80));
	mCamera->lookAt(Vector3(0, 0, -100));
	mCamera->setNearClipDistance(5);

	// Add a viewport for the camera
	mViewport = NimbusApplication::getRenderWindow()->addViewport(mCamera);

	mViewport->setBackgroundColour(Ogre::ColourValue(1.0f, .4f, .4f));

	//////////
	// Set up the appropriate models

	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

	// Load the dragon
	dragon = mSceneMgr->createEntity("Dragon", "dragon.mesh");
	dragonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	dragonNode->attachObject(dragon);

	dragonNode->setPosition(0, 0, -100);
	dragonNode->setScale(40.0, 40.0, 40.0);
	dragonNode->pitch(Degree(90));

	//////////
	// Set up light sources

	// Set the ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	// Create a point light for main light
	light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
