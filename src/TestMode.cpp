#include "TestMode.h"

#include <math.h>

#include <OgreRoot.h>
#include <OgreVector2.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreColourValue.h>

#include <OGRE\OgreMesh.h>
#include <OGRE\OgreSubMesh.h>
#include <OGRE\OgreMeshManager.h>
#include <OGRE\OgreHardwareBufferManager.h>
#include <OGRE\OgreResourceGroupManager.h>

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
	mCamera->setPosition(0.0f, 20.0f, 0.0f);
	mCamera->setNearClipDistance(5);

	// Add a viewport for the camera
	mViewport = NimbusApplication::getRenderWindow()->addViewport(mCamera);

	mViewport->setBackgroundColour(ColourValue(0.5f, 0.2f, 0.3f, 1.0f));

	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

	mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

	//////////
	// Set up the appropriate models

	// Load the dragon
	dragon = mSceneMgr->createEntity("Dragon", "dragon.mesh");
	dragonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	dragonNode->attachObject(dragon);

	dragonNode->setPosition(0, 0, -100);
	dragonNode->setScale(40.0f, 40.0f, 40.0f);
	dragonNode->pitch(Degree(90));

	//////////
	// Set up light sources

	// Set the ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	// Create a point light for main light
	light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	// Create material
	MaterialPtr material = MaterialManager::getSingleton().create(
		"Tiles/Default",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	material->getTechnique(0)->getPass(0)->setVertexColourTracking(TVC_AMBIENT);

	// Create entity and add to the scene.
	MeshPtr tileMesh = MeshManager::getSingleton().createManual("tileMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	// Create a map.
	std::cout << "Test map." << std::endl;
	Map testMap;

	// Get a mesh from the terrain and attach it to an entity.
	Entity* terrainEntity = mSceneMgr->createEntity("testTerrain", testMap.getMesh("terrainMesh"));
	SceneNode* terrainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	terrainEntity->setMaterialName("Tiles/Default");

	terrainNode->attachObject(terrainEntity);

	terrainNode->setPosition(0, 0, -100);
	terrainNode->setScale(10.0f, 10.0f, 10.0f);

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
