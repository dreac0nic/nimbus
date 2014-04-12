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

	// Rotate the camera around the tile to get a good look at it.
	this->mCameraParent->rotate(Ogre::Quaternion(Ogre::Degree(0.25f), Ogre::Vector3::UNIT_Y), Ogre::Node::TransformSpace::TS_WORLD);

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
	this->mCameraParent = this->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	SceneNode* cameraNode = this->mCameraParent->createChildSceneNode();

	cameraNode->attachObject(this->mCamera);
	cameraNode->translate(Vector3(0, 50, 100));

	mCamera->setNearClipDistance(5);

	// Add a viewport for the camera
	mViewport = NimbusApplication::getRenderWindow()->addViewport(mCamera);

	mViewport->setBackgroundColour(ColourValue(0.5f, 0.2f, 0.3f, 1.0f));

	//////////
	// Set up the appropriate models

	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

	mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

	// Load the dragon
	dragon = mSceneMgr->createEntity("Dragon", "dragon.mesh");
	dragonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	// dragonNode->attachObject(dragon);

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

	// Generate some fake map information and throw it into the scene!
	const int pointCount = 6;
	const float lPI = 3.14159f;
	const float tileScalar = 10.0f;
	Tile testTile;
	
	int vertRes = 5;
	float radius = 2.0f;

	for(int i = 0; i < vertRes; ++i) {
		Corner* temp = new Corner();

		temp->loc = new Ogre::Vector2(radius*cos(i*2*3.14/vertRes), radius*sin(i*2*3.14/vertRes));
		temp->elevation = 0.0f;

		testTile.corners.push_back(temp);

		std::cout << "Vert: " << temp->loc->x << ", " << temp->loc->y << std::endl;
	}

	testTile.loc = Ogre::Vector2(0.0f, 0.0f);
	testTile.elevation = 1.0f;

	// Create material
	MaterialPtr material = MaterialManager::getSingleton().create(
		"Tiles/Default",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	material->getTechnique(0)->getPass(0)->setVertexColourTracking(TVC_AMBIENT);
	material->setCullingMode(CULL_NONE);

	// Create entity and add to the scene.
	Entity* tileEntity = mSceneMgr->createEntity("testTile", testTile.getMesh("tileMesh"));
	SceneNode* tileNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	tileEntity->setMaterialName("Tiles/Default");

	// tileNode->attachObject(tileEntity);

	tileNode->setPosition(0, 0, -80);
	tileNode->setScale(10.0f, 10.0f, 10.0f);

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

	// Target the tile for tracking.
	this->mCamera->setAutoTracking(true, terrainNode);

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
