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

void createArrow(Ogre::Vector3 origin, Ogre::Vector3 facing)
{
	// Create a representative arrow mesh
	payloadmap createArrowPayload;
	std::string type = "Arrow";
	createArrowPayload["EntityType"] = &type;
	createArrowPayload["PositionVector"] = &origin;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createArrowPayload);
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
	// If we've already started creating the wind current
	if(mCurrentPosition != NULL)
	{
		// Calculate the delta between the new position and the previous position
		Ogre::Vector2 deltaVector = newPosition - *mCurrentPosition;

		// Create the arrow facing in the direction of the created current
		createArrow(
			Ogre::Vector3(mCurrentPosition->x, 12, mCurrentPosition->y),
			Ogre::Vector3(deltaVector.x, 12, deltaVector.y));

		// Add the new point to the wind current
		this->mWindCurrent->addPoint(*mCurrentPosition, deltaVector);

		// Keep the memory clean... hopefully this doesn't have unexpected consequences (jinx!)
		delete mCurrentPosition;
	}

	// Update the current position, because we're dynamic
	mCurrentPosition = &newPosition;
}

std::list<Ogre::Vector2> WindManager::subdivideCurrent(Ogre::Vector2 origin, Ogre::Vector2 current)
{
	// The list for storing the subdivided vector
	std::list<Ogre::Vector2> subdivideList;

	Ogre::Vector2 subVector;

	subdivideList.push_back(origin);

	// While the wind current vector is too long
	while(current.length() > this->mWorld->getWindMap()->getAlphaVector().length())
	{
		// Calculate the subvector to the size of the alpha vector
		subVector = current.normalisedCopy() * this->mWorld->getWindMap()->getAlphaVector().length();

		// Move the origin forward to the end of the next subvector
		origin += subVector;

		// Store the found subvector in the subdivision list
		subdivideList.push_back(origin + subVector);

		// Cut off the subVector
		current = current - subVector;
	}

	// Return the subdivided wind current vector
	return subdivideList;
}

Ogre::Vector2 WindManager::getCollisionPoint(Ogre::Ray* collisionRay)
{
	std::pair<bool, Ogre::Real> collision = collisionRay->intersects(this->mWindPlane);

	if (!collision.first) {
		return Ogre::Vector2::ZERO;
	} else {
		Ogre::Vector3 collisionVector = collisionRay->getPoint(collision.second);
		return Ogre::Vector2(collisionVector.x, collisionVector.z);
	}
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

void WindManager::MouseWindStartListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// In here, we create a new currentPosition and set it to click position.
	// We also call our first addPoint. Each time this is called, we create a new WindCurrent.
	if (payload.find("Context") != payload.end()) {
		if (*(static_cast<std::string*>(payload["Context"])) != std::string("Wind")) {
			return;
		}
	}

	if (payload.find("WorldRay") != payload.end()) {
		Ogre::Ray* worldRay = (static_cast<Ogre::Ray*>(payload["WorldRay"]));

		// These two constants are DEBUG VALUES. These need to change! They
		// will be determined by user input.
		const int STRENGTH = 10;
		const bool TEMPORARY = true;

		mParent->mWindCurrent = new WindCurrent(STRENGTH, TEMPORARY);
		mParent->addPoint(this->mParent->getCollisionPoint(worldRay));
	}
}

void WindManager::MouseWindEndListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// In this function, we call our final addPoint and then send off our WindCurrent
	// to the WindMap to handle. We also delete mCurrentPosition and set it to NULL.
	if (payload.find("Context") != payload.end()) {
		if (*(static_cast<std::string*>(payload["Context"])) != std::string("Wind")) {
			return;
		}
	}

	if (payload.find("WorldRay") != payload.end()) {
		Ogre::Ray* worldRay = (static_cast<Ogre::Ray*>(payload["WorldRay"]));

		// These two constants are DEBUG VALUES. These need to change! They
		// will be determined by user input.
		const int STRENGTH = 10;
		const bool TEMPORARY = true;

		mParent->addPoint(this->mParent->getCollisionPoint(worldRay));
	}

	// Send off our wind current!
	mParent->mWorld->getWindMap()->addWindCurrent(mParent->mWindCurrent);

	// Clean up mCurrentPosition
	delete mParent->mCurrentPosition;
	mParent->mCurrentPosition = NULL;
}

void WindManager::MouseWindUpdateListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// The only thing this function needs to do is call addPoint.
	if (payload.find("Context") != payload.end()) {
		if (*(static_cast<std::string*>(payload["Context"])) != std::string("Wind")) {
			return;
		}
	}

	if (payload.find("WorldRay") != payload.end()) {
		Ogre::Ray* worldRay = (static_cast<Ogre::Ray*>(payload["WorldRay"]));

		// These two constants are DEBUG VALUES. These need to change! They
		// will be determined by user input.
		const int STRENGTH = 10;
		const bool TEMPORARY = true;

		mParent->addPoint(this->mParent->getCollisionPoint(worldRay));
	}
}

void WindManager::TickListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Arbitrary value to decide if we should add random currents... tends to be negative, ergo not generating currents
	int randomCurrents = rand() % 15 - 10;

	// This will be calling the windmap update in the future.
	this->mParent->mWorld->getWindMap()->update();

	// It may also be creating random wind currents in the future.
	while(randomCurrents > 0)
	{
		mParent->generateCurrent();
	}
}
