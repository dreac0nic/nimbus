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
	this->mCurrentPosition = Ogre::Vector2::ZERO;

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

void createRedArrow(Ogre::Vector3 origin, Ogre::Vector3 facing)
{
	// Debug output
	/*std::cerr << "red origin(" << origin.x << ", " << origin.z << "), "
		<< "facing(" << facing.x << ", " << facing.z << ")\n";//*/

	// Create a representative arrow mesh
	payloadmap createArrowPayload;
	std::string type = "Arrow";
	createArrowPayload["EntityType"] = &type;
	createArrowPayload["PositionVector"] = &origin;
	createArrowPayload["FacingVector"] = &facing;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createArrowPayload);
}

void createBlueArrow(Ogre::Vector3 origin, Ogre::Vector3 facing)
{
	// Debug output
	/*std::cerr << "blue origin(" << origin.x << ", " << origin.z << "), "
		<< "facing(" << facing.x << ", " << facing.z << ")\n";//*/

	// Create a representative arrow mesh
	payloadmap createArrowPayload;
	std::string type = "BlueArrow";
	createArrowPayload["EntityType"] = &type;
	createArrowPayload["PositionVector"] = &origin;
	createArrowPayload["FacingVector"] = &facing;
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
	return true;
}

void WindManager::addPoint(Ogre::Vector2& newPosition)
{
	std::cerr << "newPos: (" << newPosition.x << ", " << newPosition.y << ")\n";

	// If we've already started creating the wind current
	if(mCurrentPosition != Ogre::Vector2::ZERO)
	{
		// Calculate the delta between the new position and the previous position
		Ogre::Vector2 deltaVector = newPosition - mCurrentPosition;

		// Create the arrow facing in the direction of the created current
		/*createRedArrow(
			Ogre::Vector3(mCurrentPosition.x, 0, mCurrentPosition.y),
			Ogre::Vector3(deltaVector.x, 0, deltaVector.y));//*/

		// Add the new point to the wind current
		this->mWindCurrent->addPoint(mCurrentPosition, deltaVector);
	}

	// Update the current position, because we're dynamic
	mCurrentPosition = newPosition;
}

std::list<Ogre::Vector2> WindManager::subdivideCurrent(Ogre::Vector2 origin, Ogre::Vector2 current)
{
	// The list for storing the subdivided vector
	std::list<Ogre::Vector2> subdivideList;

	Ogre::Vector2 subVector;

	// While the wind current vector is too long
	while(current.length() > this->mWorld->getWindMap()->getAlphaVector().length())
	{
		// Calculate the subvector to the size of the alpha vector
		subVector = current.normalisedCopy() * this->mWorld->getWindMap()->getAlphaVector().length();

		// Move the origin forward to the end of the next subvector
		origin += subVector;

		// Store the found subvector in the subdivision list
		subdivideList.push_back(origin);

		// Cut off the subVector
		current -= subVector;
	}

	subdivideList.push_back(origin + current);

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

		createBlueArrow(collisionVector, Ogre::Vector3::ZERO);

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

		mParent->mWindCurrent = new WindCurrent(Ogre::Real(STRENGTH), TEMPORARY);
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
		Ogre::Vector2 clickDelta = mParent->getCollisionPoint(worldRay) - mParent->mCurrentPosition;

		// These two constants are DEBUG VALUES. These need to change! They
		// will be determined by user input.
		const int STRENGTH = 10;
		const bool TEMPORARY = true;

		std::list<Ogre::Vector2> currentVectorList = mParent->subdivideCurrent(mParent->mCurrentPosition, clickDelta);
		for (std::list<Ogre::Vector2>::iterator itr = currentVectorList.begin(); itr != currentVectorList.end(); ++itr)
		{
			createRedArrow(Ogre::Vector3(mParent->mCurrentPosition.x, -10, mParent->mCurrentPosition.y), Ogre::Vector3::ZERO);
			std::cerr << "Add point at (" << itr->x << ", " << itr->y << ")\n";
			mParent->addPoint(*itr);
		}
	}

	// Send off our wind currnt!
	mParent->mWorld->getWindMap()->addWindCurrent(mParent->mWindCurrent);

	mParent->mCurrentPosition = Ogre::Vector2::ZERO;
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
		Ogre::Vector2 clickDelta = mParent->getCollisionPoint(worldRay) - mParent->mCurrentPosition;

		// These two constants are DEBUG VALUES. These need to change! They
		// will be determined by user input.
		const int STRENGTH = 10;
		const bool TEMPORARY = true;

		std::list<Ogre::Vector2> currentVectorList = mParent->subdivideCurrent(mParent->mCurrentPosition, clickDelta);
		for (std::list<Ogre::Vector2>::iterator itr = currentVectorList.begin(); itr != currentVectorList.end(); ++itr)
		{
			createRedArrow(Ogre::Vector3(mParent->mCurrentPosition.x, -10, mParent->mCurrentPosition.y), Ogre::Vector3::ZERO);
			std::cerr << "Add point at (" << itr->x << ", " << itr->y << ")\n";
			mParent->addPoint(*itr);
		}
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
		// Generate a random current
		mParent->generateCurrent();

		// Decrement the number of random currents needed
		randomCurrents--;
	}
}
