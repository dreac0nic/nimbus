#include <list>
#include <OgreRay.h>
#include <OgreVector2.h>
#include <OgreManualObject.h>

#include "WindManager.h"
#include "EventSystem.h"

using namespace Nimbus;

WindManager::WindManager(Ogre::SceneManager* sceneManager)
{
	this->mSceneManager = sceneManager;
	this->mRaySceneQuery = mSceneManager->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	this->mRaySceneQuery->setSortByDistance(true);

	createClickPlane();

	EventSystem::getSingleton()->registerListener(new PathListener(this), EventSystem::EventType::MOUSE_PATH);
}

WindManager::~WindManager(void)
{
}

// From: https://www.youtube.com/watch?v=QquD8qFUKeM
void WindManager::createClickPlane()
{
	Ogre::Real planeHeight = -12;
	Ogre::ManualObject* planeXY = mSceneManager->createManualObject("PlaneXY");
	planeXY->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_FAN);
	planeXY->colour(0, 0, 1);
	planeXY->position(Ogre::Vector3(-100, planeHeight, 0)); // Close Left
	planeXY->position(Ogre::Vector3(100, planeHeight, 0)); // Close Right
	planeXY->position(Ogre::Vector3(100, planeHeight, -200)); // Far Right
	planeXY->position(Ogre::Vector3(-100, planeHeight, -200)); // Far Left
	planeXY->end();
	//planeXY->setVisible(false);

	mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(planeXY);
}

Ogre::RaySceneQuery* WindManager::getRaySceneQuery()
{
	return this->mRaySceneQuery;
}

bool WindManager::update(void)
{
	return true;
}

void WindManager::PathListener::handleEvent(payloadmap payload)
{
	std::list<Ogre::Ray>* rays = (std::list<Ogre::Ray>*)payload["Rays"];
	std::list<Ogre::Vector2>* points = (std::list<Ogre::Vector2>*)payload["Points"];
	
	// From: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Raycasting+to+the+polygon+level
	if (rays->size() > 0)
	{
		this->mContainingManager->getRaySceneQuery()->setRay(rays->front());
		if (this->mContainingManager->getRaySceneQuery()->execute().size() <= 0)
		{
			Ogre::LogManager::getSingleton().logMessage("Missed everything");
			return;
		}
	}
    Ogre::RaySceneQueryResult &rayQueryResult = this->mContainingManager->getRaySceneQuery()->getLastResults();
    for (unsigned int x = 0; x < rayQueryResult.size(); x++)
    {
        if ((rayQueryResult[x].movable != NULL) &&
            (rayQueryResult[x].movable->getMovableType().compare("ManualObject") == 0))
        {
            Ogre::ManualObject* windPlane = static_cast<Ogre::ManualObject*>(rayQueryResult[x].movable);
			Ogre::LogManager::getSingleton().logMessage("Hit " + windPlane->getName());
        }
		else
		{
			Ogre::LogManager::getSingleton().logMessage("Hit " + rayQueryResult[x].movable->getName());
		}
    }
}
