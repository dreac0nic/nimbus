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

	xPos = (int) floor(x / mResolution);
	yPos = (int) floor(y / mResolution);

	return this->mVectorMap->get(xPos, yPos);
}

Ogre::Vector2 WindMap::getWindVector(Ogre::Vector2 position)
{
	return getWindVector(position.x, position.y);
}

Ogre::Vector2 WindMap::getAverageWindVector(Ogre::Real topLeftx, Ogre::Real topLefty, Ogre::Real botRightx, Ogre::Real botRighty)
{
	std::vector<Ogre::Vector2> vectorList;
	Ogre::Vector2 averageVector = Ogre::Vector2::ZERO;

	// Get all vectors on the windmap between topLeft and botRight
	for (int x = ceil(topLeftx / mResolution) * mResolution; x < floor(botRightx / mResolution) * mResolution; x+=mResolution) {
		for (int y = floor(topLefty / mResolution) * mResolution; y < ceil(botRighty / mResolution) * mResolution; y+=mResolution) {
			vectorList.push_back(getWindVector(x, y));
		}
	}

	for (int i = 0; i < vectorList.size(); i++)
	{
		averageVector += vectorList.at(i);
	}

	averageVector /= vectorList.size();

	return averageVector;
}

Ogre::Vector2 WindMap::getAverageWindVector(Ogre::Vector2 topLeft, Ogre::Vector2 botRight)
{
	std::vector<Ogre::Vector2> vectorList;
	Ogre::Vector2 averageVector = Ogre::Vector2::ZERO;

	// Get all vectors on the windmap between topLeft and botRight
	for (int x = ceil(topLeft.x / mResolution) * mResolution; x < floor(botRight.x / mResolution) * mResolution; x+=mResolution) {
		for (int y = floor(topLeft.y / mResolution) * mResolution; y < ceil(botRight.y / mResolution) * mResolution; y+=mResolution) {
			vectorList.push_back(getWindVector(x, y));
		}
	}

	for (int i = 0; i < vectorList.size(); i++)
	{
		averageVector += vectorList.at(i);
	}

	averageVector /= vectorList.size();

	return averageVector;
}

std::list<WindCurrent*>* WindMap::getWindCurrents()
{
	return &this->mCurrents;
}

void WindMap::update(void)
{
}
