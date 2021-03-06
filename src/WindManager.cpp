#include <list>
#include <sstream>
#include <OgreEntity.h>
#include <OISMouse.h>
#include <OgreMeshManager.h>
#include <OgreRay.h>
#include <OgreSceneManager.h>
#include <OgreVector2.h>

#include "WindManager.h"
#include "EventSystem/EventSystem.h"
#include "WindMap.h"
#include "VectorMap.h"
#include "WindCurrent.h"
#include "World.h"

using namespace Nimbus;

enum ArrowType {RED, BLUE};

WindManager::WindManager(Ogre::SceneManager* sceneManager, World* world, Ogre::Real beta) :
	mWindPlane(Ogre::Vector3::UNIT_Y, -12),
	mWorld(world),
	mEssentiallyZero(beta)
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

void createArrow(Ogre::Vector3 origin, Ogre::Vector3 facing, ArrowType arrow)
{
	// Create a representative arrow mesh
	payloadmap createArrowPayload;

	// Choose what kind of arrow we're creating
	std::string type;
	switch(arrow) {
	case ArrowType::BLUE:
		type = "BlueArrow";
		break;
	case ArrowType::RED:
		type = "RedArrow";
		break;
	}

	createArrowPayload["EntityType"] = &type;
	createArrowPayload["PositionVector"] = &origin;
	createArrowPayload["FacingVector"] = &facing;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createArrowPayload);}

void createArrow(Ogre::Vector2 origin, Ogre::Vector2 facing, ArrowType arrow)
{
	Ogre::Vector3 origin3d = Ogre::Vector3(origin.x, (arrow == ArrowType::RED) ? -12.0f : -10.0f, origin.y);
	Ogre::Vector3 facing3d = Ogre::Vector3(facing.x, 0, facing.y);

	createArrow(origin3d, facing3d, arrow);
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
	// Sanitize the point to world border if it's out of bounds.
	if (newPosition.x > mWorld->getWorldBounds().x - mWorld->getWindMap()->getOffset().x) {
		newPosition.x = mWorld->getWorldBounds().x - mWorld->getWindMap()->getOffset().x;
	} if (newPosition.x < mWorld->getWindMap()->getOffset().x - mWorld->getWorldBounds().x) {
		newPosition.x = mWorld->getWindMap()->getOffset().x - mWorld->getWorldBounds().x;
	} if (newPosition.y > mWorld->getWorldBounds().y - mWorld->getWindMap()->getOffset().y) {
		newPosition.y = mWorld->getWorldBounds().y - mWorld->getWindMap()->getOffset().y;
	} if(newPosition.y < mWorld->getWindMap()->getOffset().y - mWorld->getWorldBounds().y) {
		newPosition.y = mWorld->getWindMap()->getOffset().y - mWorld->getWorldBounds().y;
	}

	// If we've already started creating the wind current
	if(mCurrentPosition != Ogre::Vector2::ZERO)
	{
		// Calculate the delta between the new position and the previous position
		Ogre::Vector2 deltaVector = newPosition - mCurrentPosition;

		// Only add the new position if the delta isn't zero
		if(deltaVector.length() > mEssentiallyZero.length())
		{
			// Create the arrow facing in the direction of the created current
			createArrow(mCurrentPosition, deltaVector, ArrowType::RED);

			// Add the new point to the wind current
			this->mWindCurrent->addPoint(mCurrentPosition, deltaVector);
		} else {
			return;
		}
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

	// Add the final point of the wind current
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

		return Ogre::Vector2(collisionVector.x, collisionVector.z);
	}
}

WindCurrent WindManager::generateCurrent(void)
{
	WindCurrent w = WindCurrent(Ogre::Real(rand() % 10));
	int numVectors = rand() % 8 + 3;

	/*Ogre::Vector2 point = Ogre::Vector2(Ogre::Real(rand() % mWorld->getWindMap()->mOffset), Ogre::real)

	while (numVectors > 0) {

	}*/

	return w;
}

WindCurrent WindManager::generateCurrent(int numVectors)
{
	// Stub!

	return WindCurrent(0);
}

WindCurrent WindManager::generateCurrent(int numVectors, Ogre::Vector2 startingPosition)
{
	// Stub!

	return WindCurrent(0);
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
		OIS::MouseButtonID* id = static_cast<OIS::MouseButtonID*>(payload["ButtonPressed"]);
		Ogre::Ray* worldRay = (static_cast<Ogre::Ray*>(payload["WorldRay"]));

		bool tempCurrent = true;
		if (*id == OIS::MouseButtonID::MB_Left)
		{
			tempCurrent = true;
		}
		else if (*id == OIS::MouseButtonID::MB_Right)
		{
			//tempCurrent = false;
		}

		// These two constants are DEBUG VALUES. These need to change! They
		// will be determined by user input.
		const int STRENGTH = 10;

		mParent->mWindCurrent = new WindCurrent(Ogre::Real(STRENGTH), tempCurrent);

		// Create the first mouseDown point and add it to our current.
		Ogre::Vector2 collisionPoint = this->mParent->getCollisionPoint(worldRay);
		mParent->addPoint(collisionPoint);
	}
}

std::string WindManager::MouseWindStartListener::str()
{
    std::stringstream output;
    output << "Mouse Wind Start Listener (Listens for the start of a wind current) [WindManager]";
    return output.str();
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

	if (payload.find("WorldRay") != payload.end() && mParent->mWindCurrent != NULL) {
		Ogre::Ray* worldRay = (static_cast<Ogre::Ray*>(payload["WorldRay"]));
		Ogre::Vector2 collisionPoint = this->mParent->getCollisionPoint(worldRay);
		Ogre::Vector2 clickDelta = collisionPoint - mParent->mCurrentPosition;

		std::list<Ogre::Vector2> currentVectorList = mParent->subdivideCurrent(mParent->mCurrentPosition, clickDelta);
		for (std::list<Ogre::Vector2>::iterator itr = currentVectorList.begin(); itr != currentVectorList.end(); ++itr)
		{
			// Add each point along our path to the wind current in construction
			mParent->addPoint(*itr);
		}
	}

	// Send off our wind currnt!
	mParent->mWorld->getWindMap()->addWindCurrent(mParent->mWindCurrent);

	mParent->mCurrentPosition = Ogre::Vector2::ZERO;
}

std::string WindManager::MouseWindUpdateListener::str()
{
    std::stringstream output;
    output << "Mouse Wind Update Listener (Listens for the update for a wind current) [WindManager]";
    return output.str();
}

void WindManager::MouseWindUpdateListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// The only thing this function needs to do is call addPoint.
	if (payload.find("Context") != payload.end()) {
		if (*(static_cast<std::string*>(payload["Context"])) != std::string("Wind")) {
			return;
		}
	}

	if (payload.find("WorldRay") != payload.end() && mParent->mWindCurrent != NULL) {
		Ogre::Ray* worldRay = (static_cast<Ogre::Ray*>(payload["WorldRay"]));
		Ogre::Vector2 collisionPoint = this->mParent->getCollisionPoint(worldRay);
		Ogre::Vector2 clickDelta = collisionPoint - mParent->mCurrentPosition;

		std::list<Ogre::Vector2> currentVectorList = mParent->subdivideCurrent(mParent->mCurrentPosition, clickDelta);
		for (std::list<Ogre::Vector2>::iterator itr = currentVectorList.begin(); itr != currentVectorList.end(); ++itr)
		{
			// Add each point along our path to the wind current in construction
			mParent->addPoint(*itr);
		}
	}
}

std::string WindManager::MouseWindEndListener::str()
{
    std::stringstream output;
    output << "Mouse Wind End Listener (Listens for the end of a wind current) [WindManager]";
    return output.str();
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

std::string WindManager::TickListener::str()
{
    std::stringstream output;
    output << "Tick Listener (Updates the wind map) [WindManager]";
    return output.str();
}
