#include "TestMode.h"

#include <math.h>

#include <OgreRoot.h>
#include <OgreVector2.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>

#include "NimbusApplication.h"

#include "Map.h"
#include "Corner.h"
#include "Tile.h"

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

	//////////
	// Set up the appropriate models

	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

	mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

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

	// Generate some fake map information and throw it into the scene!
	const int pointCount = 6;
	const float lPI = 3.14159f;
	const float tileScalar = 10.0f;
	Tile testTile;

	Corner* corn1 = new Corner();
	Corner* corn2 = new Corner();
	Corner* corn3 = new Corner();
	Corner* corn4 = new Corner();
	Corner* corn5 = new Corner();

	corn1->loc = new Ogre::Vector2(0.0f, 1.0f);
	corn2->loc = new Ogre::Vector2(-1.0f, 1.0f);
	corn3->loc = new Ogre::Vector2(-1.0f, 0.0f);
	corn4->loc = new Ogre::Vector2(0.0f, -1.0f);
	corn5->loc = new Ogre::Vector2(1.0f, -1.0f);

	testTile.corners.push_back(corn1);
	testTile.corners.push_back(corn2);
	testTile.corners.push_back(corn3);
	testTile.corners.push_back(corn4);
	testTile.corners.push_back(corn5);

	/*
	for(int i = 0; i < pointCount; ++i) {
		Corner* tempCorner = new Corner();
		tempCorner->loc = new Ogre::Vector2(tileScalar*cos(2*lPI*(i/pointCount)), tileScalar*sin(2*lPI*(i/pointCount)));

		testTile.corners.push_back(tempCorner);
	}
	*/

	Entity* tileEntity = mSceneMgr->createEntity("testTile", testTile.getMesh());
	SceneNode* tileNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	tileNode->attachObject(tileEntity);

	tileNode->setPosition(0, 0, -150);
	tileNode->setScale(40.0f, 40.0f, 40.0f);

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
