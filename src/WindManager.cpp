#include <list>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreRay.h>
#include <OgreSceneManager.h>
#include <OgreVector2.h>

#include "WindManager.h"
#include "EventSystem.h"

using namespace Nimbus;

WindManager::WindManager(Ogre::SceneManager* sceneManager)
	: mWindPlane(Ogre::Vector3::UNIT_Y, -12)
{
	this->mSceneManager = sceneManager;

	createClickPlane();

	this->mMouseWindListener = new MouseWindListener(this, sceneManager);
	EventSystem::getSingleton()->registerListener(mMouseWindListener,
		EventSystem::EventType::MOUSE_POSITION);
}

WindManager::~WindManager(void)
{
	delete this->mMouseWindListener;
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
	return true;
}

void WindManager::MouseWindListener::handleEvent(payloadmap payload, EventListener* responder)
{
	std::string context = *(static_cast<std::string*>(payload["Context"]));

	if (context.compare("Wind") == 0)
	{
		Ogre::Ray* ray = static_cast<Ogre::Ray*>(payload["WorldRay"]);
		Ogre::Vector2* screenPos = static_cast<Ogre::Vector2*>(payload["ScreenPosition"]);

		// Debug material for collision markers
		Ogre::MaterialPtr debugMarkerMat = Ogre::MaterialManager::getSingleton().create("DebugMarkers", "General");
		debugMarkerMat->getTechnique(0)->getPass(0)->setAmbient(1,0,0);
		
		// Testing the ray for collision with the wind plane
		std::pair<bool, Ogre::Real> result = ray->intersects(this->mContainingManager->mWindPlane);
 
		if(result.first)
		{
			Ogre::Vector3 point = ray->getPoint(result.second);
			std::stringstream message;
			message << "Hit at " << point.x << ", " << point.y << ", " << point.z;
			Ogre::LogManager::getSingleton().logMessage(message.str());

			// Creating the plane to show the succesful clicking test
			std::stringstream planeName, entityName;
			Ogre::Plane testPlane(Ogre::Vector3::UNIT_Y, 0);
			planeName << "TestPlane" << mCounter;
			Ogre::MeshManager::getSingleton().createPlane(planeName.str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					testPlane, 2, 2, 2, 2, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
			entityName << "TestPlaneEntity" << mCounter++;
			Ogre::Entity* entTestPlane = mSceneManager->createEntity(entityName.str(), planeName.str());
			entTestPlane->setMaterialName("DebugMarkers");
			Ogre::SceneNode* testPlaneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
			testPlaneNode->attachObject(entTestPlane);
			testPlaneNode->setPosition(point);
		}
	}
}