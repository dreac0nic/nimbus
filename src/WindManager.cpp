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

WindManager::WindManager(Ogre::SceneManager* sceneManager, World* world)
	: mWindPlane(Ogre::Vector3::UNIT_Y, -12), mWorld(world)// mWindMap(*mWorld->getWindMap())
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

bool oldUpdate(void)
{
	WindMap mWindMap = *this->mWorld->getWindMap();

	// Smooth current wind vectors
	for(int i = 0; i < mWindMap.sizeX; i++)
	{
		std::stringstream message;
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
			
			message << "[(" << i << "," << j << "),(" << totalWindX * ORIGININFLUENCE << "," << totalWindY * ORIGININFLUENCE << ")]; ";

			mWindMap.setVector(i, j, totalWindX * ORIGININFLUENCE, totalWindY * ORIGININFLUENCE);
		}
		Ogre::LogManager::getSingleton().logMessage(message.str());
	}

	// Factor in wind currents
	std::list<WindCurrent>::iterator current = mWindMap.currents.begin();
	while (current != mWindMap.currents.end())
	{
		double strength = current->strength;

		if(current->strength > 0){
			Ogre::Vector2 firstPos = Ogre::Vector2::NEGATIVE_UNIT_X;

			std::list<Ogre::Vector2>::iterator secondPos = current->path.begin();
			while (secondPos != current->path.end())
			{
				if(firstPos != Ogre::Vector2::NEGATIVE_UNIT_X){
					Ogre::Vector2 temp = (*secondPos - firstPos) * Ogre::Real(strength);

					if(mWindMap.getVector(firstPos).length() < temp.length())
					{
						mWindMap.setVector(firstPos, temp);
					}
				}

				firstPos = *secondPos;
				++secondPos;
			}
			if(current->temp) {
				current->strength -= STRENGTHTOSUBTRACT;
			}
			++current;
		} else 
		{
			current = mWindMap.currents.erase(current);
		}
	}

	return true;
}

void WindManager::MouseWindStartListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Stub!
}

void WindManager::MouseWindEndListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Stub!
}

void WindManager::MouseWindUpdateListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Stub!
}

void WindManager::TickListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Stub!
	// This will be calling the windmap update in the future.
}
