#include <list>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreRay.h>
#include <OgreSceneManager.h>
#include <OgreVector2.h>
#include "WindManager.h"
#include "EventSystem.h"
#include "WindMap.h"
#include "VectorMap.h"
#include "WindCurrent.h"
#include "World.h"

using namespace Nimbus;

WindManager::WindManager(Ogre::SceneManager* sceneManager, World* world) :
	mWindPlane(Ogre::Vector3::UNIT_Y, -12),
	mWorld(world)
{
	this->mSceneManager = sceneManager;

	createClickPlane();

	this->mMouseWindUpdateListener = new MouseWindUpdateListener(this);
	EventSystem::getSingleton()->registerListener(mMouseWindUpdateListener,
		EventSystem::EventType::MOUSE_POSITION_UPDATE);

	this->mMouseWindStartListener = new MouseWindStartListener(this);
	EventSystem::getSingleton()->registerListener(mMouseWindStartListener,
		EventSystem::EventType::MOUSE_POSITION_START);

	this->mMouseWindEndListener = new MouseWindEndListener(this);
	EventSystem::getSingleton()->registerListener(mMouseWindEndListener,
		EventSystem::EventType::MOUSE_POSITION_END);

	this->mTickListener = new TickListener(this);
	EventSystem::getSingleton()->registerListener(mTickListener,
		EventSystem::EventType::TICK);
}

WindManager::~WindManager(void)
{
	delete this->mMouseWindUpdateListener;
	delete this->mMouseWindStartListener;
	delete this->mMouseWindEndListener;
	delete this->mTickListener;
}

void WindManager::createClickPlane()
{
	// Debug material for wind plane
	Ogre::MaterialPtr debugWindPlaneMat = Ogre::MaterialManager::getSingleton().create("DebugWindPlane", "General");
	debugWindPlaneMat->getTechnique(0)->getPass(0)->setAmbient(0,0,1);

	// Creating the actual plane
	Ogre::MeshManager::getSingleton().createPlane("WindPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		mWindPlane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entWindPlane = mSceneManager->createEntity("WindPlaneEntity", "WindPlane");
	entWindPlane->setMaterialName("DebugWindPlane");
	//mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(entWindPlane);
}

bool WindManager::update(void)
{
	// Yay! Stub!
}

void WindManager::addPoint(Ogre::Vector2& newPosition)
{
	// If we havent't started making a wind current yet
	if(mCurrentPosition != NULL)
	{
		// Calculate the delta between the new position and the previous position
		Ogre::Vector2 deltaVector = newPosition - *mCurrentPosition;

		// Add the new point to the wind current
		this->mWindCurrent->addPoint(*mCurrentPosition, deltaVector);

		// Keep the memory clean... hopefully this doesn't have unexpected consequences (jinx!)
		delete mCurrentPosition;
	}

	// Update the current position, because we're dynamic
	mCurrentPosition = &newPosition;
}

void WindManager::MouseWindStartListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// In here, we create a new currentPosition and set it to null. We also call our first
	// addPoint. Each time this is called, we create a new WindCurrent.
}

void WindManager::MouseWindEndListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// In this function, we call our final addPoint and then send off our WindCurrent
	// to the WindMap to handle. We also delete mCurrentPosition and set it to NULL.
}

void WindManager::MouseWindUpdateListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// The only thing this function needs to do is call addPoint.
}

void WindManager::TickListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Stub!
	// This will be calling the windmap update in the future.
	// It may also be creating random wind currents in the future.
}

void WindManager::generateCurrent(void)
{
	// Stub!
}

void WindManager::generateCurrent(int numVectors)
{
	// Stub!
}

void WindManager::generateCurrent(int numVectors, Ogre::Vector2 startingPosition)
{
	// Stub!
}
