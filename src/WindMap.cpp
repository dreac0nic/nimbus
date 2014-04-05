#include <list>
#include <math.h>
#include <algorithm>
#include <OgreRoot.h>
#include "WindCurrent.h"
#include "WindMap.h"

using namespace Nimbus;

WindMap::WindMap(Ogre::Real worldSize, Ogre::Real resolution) :
	mResolution(resolution),
	mAlphaVector(Ogre::Vector2(resolution, resolution)),
	mCurrents()
{
	// Calculate vector map size
	int vectorMapWidth = (int)floor(worldSize / resolution + .5);
	int vectorMapHeight = (int)floor(worldSize / resolution + .5);

	// Initialize the vector map
	this->mVectorMap = new Grid<Ogre::Vector2>(vectorMapWidth, vectorMapHeight);
}

WindMap::~WindMap(void)
{
}

void WindMap::update(void)
{
}

void WindMap::addWindCurrent(WindCurrent* windCurrent)
{
	this->mCurrents.push_back(windCurrent);
}

void WindMap::removeWindCurrent(WindCurrent* windCurrent)
{
	// Get an iterator for the given wind current
	std::list<WindCurrent*>::iterator toDelete =
		find(this->mCurrents.begin(), this->mCurrents.end(), windCurrent);

	// If we've found te wind current
	if(toDelete != this->mCurrents.end())
	{
		// Delete the wind current
		this->mCurrents.erase(toDelete);
	}
}

Ogre::Vector2 WindMap::getWindVector(Ogre::Real x, Ogre::Real y)
{
	int xPos, yPos;

	// Rounding, we do it (there's no round function...)
	xPos = (int) floor(x / mResolution + .5);
	yPos = (int) floor(y / mResolution + .5);

	return this->mVectorMap->get(xPos, yPos);
}

Ogre::Vector2 WindMap::getWindVector(Ogre::Vector2 position)
{
	return getWindVector(position.x, position.y);
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
