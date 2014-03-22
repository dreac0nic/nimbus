#include <list>
#include <OgreRay.h>
#include <OgreVector2.h>

#include "WindManager.h"
#include "EventSystem.h"

using namespace Nimbus;

WindManager::WindManager(void)
{
	EventSystem::getSingleton()->registerListener(new PathListener(), EventSystem::EventType::MOUSE_PATH);
}

WindManager::~WindManager(void)
{
}

bool WindManager::update(void)
{
	return true;
}

void WindManager::PathListener::handleEvent(payloadmap payload)
{
	std::list<Ogre::Ray>* rays = (std::list<Ogre::Ray>*)payload["Rays"];
	std::list<Ogre::Vector2>* points = (std::list<Ogre::Vector2>*)payload["Points"];
}
