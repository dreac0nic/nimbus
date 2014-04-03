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

using namespace Nimbus;

WindManager::WindManager(Ogre::SceneManager* sceneManager, WindMap windMap)
	: mWindPlane(Ogre::Vector3::UNIT_Y, -12), mWindMap(windMap)
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
}

WindManager::~WindManager(void)
{
	delete this->mMouseWindUpdateListener;
	delete this->mMouseWindStartListener;
	delete this->mMouseWindEndListener;
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
	for(int i = 0; i < mWindMap.sizeX; i++)
	{
		for(int j = 0; j < mWindMap.sizeY; j++)
		{
			Ogre::Vector2 currentVector = mWindMap.getVector(i, j);
			Ogre::Vector2 temp;
			double totalWindX = currentVector.x;
			double totalWindY = currentVector.y;

			if(i != 0)
			{
				if(j != 0)
				{
					temp = mWindMap.getVector(i-1, j-1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}

				temp = mWindMap.getVector(i-1, j);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;

				if(j != mWindMap.sizeY-1)
				{
					temp = mWindMap.getVector(i-1, j+1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}
			}

			if(j != 0)
			{
				temp = mWindMap.getVector(i, j-1);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;
			}

			if(j != mWindMap.sizeY-1)
			{
				temp = mWindMap.getVector(i, j+1);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;
			}

			if(i != mWindMap.sizeX-1)
			{
				if(j != 0)
				{
					temp = mWindMap.getVector(i+1, j-1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}

				temp = mWindMap.getVector(i+1, j);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;

				if(j != mWindMap.sizeY-1)
				{
					temp = mWindMap.getVector(i+1, j+1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}
			}
			mWindMap.setVector(i, j, totalWindX * ORIGININFLUENCE, totalWindY * ORIGININFLUENCE);
		}
	}

	std::list<WindCurrent>::iterator current = mWindMap.currents.begin();
	while (current != mWindMap.currents.end())
	{
		double strength = current->strength;

		if(current->strength > 0){
			Ogre::Vector2 firstPos = Ogre::Vector2::NEGATIVE_UNIT_X;

			std::list<Ogre::Vector2>::iterator secondPos = current->path.begin();
			while (secondPos != current->path.end())
			{
				Ogre::Vector2 secondVec = mWindMap.getVector(*secondPos);

				if(firstPos != Ogre::Vector2::NEGATIVE_UNIT_X){
					Ogre::Vector2 firstVec = mWindMap.getVector(firstPos);
					Ogre::Vector2 temp = Ogre::Vector2((Ogre::Real)((secondVec.x - firstVec.x) * strength), (Ogre::Real)((secondVec.y - firstVec.y) * strength));

					mWindMap.setVector(firstPos, temp);
				}

				firstPos = *secondPos;
				++current;
			}
			if(current->temp) {
				current->strength -= STRENGTHTOSUBTRACT;
			}
			++current;
		} else 
		{
			current = this->mWindMap.currents.erase(current);
		}
	}
	return true;
}

void WindManager::MouseWindStartListener::handleEvent(payloadmap payload, EventListener* responder)
{
	mContainingManager->tempCurrent = WindCurrent();
}

void WindManager::MouseWindEndListener::handleEvent(payloadmap payload, EventListener* responder)
{
	mContainingManager->mWindMap.currents.push_back(mContainingManager->tempCurrent);
}

void WindManager::MouseWindUpdateListener::handleEvent(payloadmap payload, EventListener* responder)
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

			mContainingManager->tempCurrent.path.push_back(Ogre::Vector2(point.x, point.z));

			std::stringstream message;
			message << "Hit at " << point.x << ", " << point.y << ", " << point.z;
			Ogre::LogManager::getSingleton().logMessage(message.str());

			// Create a representative arrow mesh
			payloadmap createArrowPayload;
			std::string type = "Arrow";
			createArrowPayload["EntityType"] = &type;
			createArrowPayload["PositionVector"] = &point;
			EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createArrowPayload);

			// Creating the plane to show the succesful clicking test
			/*std::stringstream planeName, entityName;
			Ogre::Plane testPlane(Ogre::Vector3::UNIT_Y, 0);
			planeName << "TestPlane" << mCounter;
			Ogre::MeshManager::getSingleton().createPlane(planeName.str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			testPlane, 15, 15, 2, 2, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
			entityName << "TestPlaneEntity" << mCounter++;
			Ogre::Entity* entTestPlane = mSceneManager->createEntity(entityName.str(), planeName.str());
			entTestPlane->setMaterialName("DebugMarkers");
			Ogre::SceneNode* testPlaneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
			testPlaneNode->attachObject(entTestPlane);
			testPlaneNode->setPosition(point);*/
		}
	}
}
