#include <list>
#include <math.h>
#include <OgreRoot.h>
#include "WindCurrent.h"
#include "WindMap.h"

using namespace Nimbus;

WindMap::WindMap(Ogre::Real worldSize, Ogre::Real resolution) :
	mResolution(resolution),
	mCurrents()
{
}

WindMap::~WindMap(void)
{
}

Ogre::Vector2 WindMap::getWindVector(Ogre::Real x, Ogre::Real y)
{
	return this->mVectorMap->get(x, y);
}

Ogre::Vector2 WindMap::getWindVector(Ogre::Vector2 position)
{
	return this->mVectorMap->get(position.x, position.y);
}

Ogre::Vector2 getAverageWindVector(Ogre::Real topLeftx, Ogre::Real topLefty, Ogre::Real botRightx, Ogre::Real botRighty)
{
}

Ogre::Vector2 getAverageWindVector(Ogre::Vector2 topLeft, Ogre::Vector2 botRight)
{
}
