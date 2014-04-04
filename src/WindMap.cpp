#include <list>
#include <math.h>
#include <algorithm>
#include <OgreRoot.h>
#include "WindCurrent.h"
#include "WindMap.h"

using namespace Nimbus;

WindMap::WindMap(Ogre::Real worldSize, Ogre::Real resolution) :
	mResolution(resolution),
	mCurrents()
{
	// Calculate vector map size
	int vectorMapWidth = (int)floor(worldSize / resolution);
	int vectorMapHeight = (int)floor(worldSize / resolution);

	// Initialize the vector map
	this->mVectorMap = new Grid<Ogre::Vector2>(vectorMapWidth, vectorMapHeight);
}

WindMap::~WindMap(void)
{
}

void WindMap::addWindCurrent(WindCurrent* windCurrent)
{
	this->mCurrents.push_back(windCurrent);
}

void WindMap::removeWindCurrent(WindCurrent* windCurrent)
{
	std::list<WindCurrent*>::iterator toDelete =
		find(this->mCurrents.begin(), this->mCurrents.end(), windCurrent);

	this->mCurrents.erase(toDelete);
}

Ogre::Vector2 WindMap::getWindVector(Ogre::Real x, Ogre::Real y)
{
	return this->mVectorMap->get(x, y);
}

Ogre::Vector2 WindMap::getWindVector(Ogre::Vector2 position)
{
	return this->mVectorMap->get(position.x, position.y);
}

Ogre::Vector2 WindMap::getAverageWindVector(Ogre::Real topLeftx, Ogre::Real topLefty, Ogre::Real botRightx, Ogre::Real botRighty)
{

}

Ogre::Vector2 WindMap::getAverageWindVector(Ogre::Vector2 topLeft, Ogre::Vector2 botRight)
{

}

std::list<WindCurrent*>* WindMap::getWindCurrents()
{
	return &this->mCurrents;
}
