#include "GameMode.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "NimbusApplication.h"

using namespace Nimbus;
using namespace Ogre;

GameMode::GameMode(void)
{
}

GameMode::~GameMode(void)
{
	// Delete dynamic member variables... I think this is the right place for this.
	delete this->mEntityMan;
	delete this->mEnvironmentMan;
	delete this->mWorld;
}

void GameMode::initialize()
{	
	// Create the scene manager
	mSceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager");
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Hardcoding some things for now
	// Position the camera
	mCamera->setPosition(Vector3(0, 50, 80));
	mCamera->lookAt(Vector3(0, 0, -100));
	mCamera->setNearClipDistance(5);
	// Add a viewport for the camera
	mViewport = NimbusApplication::getRenderWindow()->addViewport(mCamera);
	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
	// Set the ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	
	// Construct the game world (truthfully this should be done by the menu probably...
	//     this would allow for game configuration and loading... of maybe we
	//     should have a loading game mode.
	this->mWorld = new World(this->mSceneMgr);

	// Construct the world managers
	this->mEnvironmentMan = new EnvironmentManager();
	this->mEntityMan = new EntityManager(this->mWorld);

	// Configure entity types
	this->mEntityMan->configureEntityTypes("../../assets/scripts/ConfigFiles.ini", this->mWorld);

	// Initializing the managers
	this->mEnvironmentMan->initialize();
	this->mEntityMan->initialize();

	// Adding the world root node to the actual scene
	this->mSceneMgr->getRootSceneNode()->addChild(this->mWorld->getWorldNode());
}

RunMode* GameMode::run(const FrameEvent& evt)
{
	// Updating all of the entities through the manager
	this->mEntityMan->update();

	// Continue to run this runmode
	return this;
}

void GameMode::pause(void)
{
}

void GameMode::stop(void)
{
}
