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

Ogre::Vector2 WindMap::getVector(double posx, double posy)
{
	double resultX;
	double resultY;

	int flox = (int)floor(posx);
	int floy = (int)floor(posy);
		
	Ogre::Vector2 topLeft = vectorMap->getVector(flox, floy);
	Ogre::Vector2 botLeft = Ogre::Vector2(0, 0);
	if (1 + floy < sizeY)
	{
		botLeft = vectorMap->getVector(flox, 1 + floy);
	}
	Ogre::Vector2 topRight = Ogre::Vector2(0, 0);
	if (1 + flox < sizeX)
	{
		topRight = vectorMap->getVector(1 + flox, floy);
	}
	Ogre::Vector2 botRight = Ogre::Vector2(0, 0);
	if (1 + flox < sizeX && 1 + floy < sizeY)
	{
		botRight = vectorMap->getVector(1 + flox, 1 + floy);
	}
		
	double left = flox - posx + 1;
	double top = floy - posy + 1;

	resultX = left * top * topLeft.x + left * (1-top) * botLeft.x + (1-left) * top * topRight.x + (1-left) * (1-top) * botRight.x;
	resultY = left * top * topLeft.y + left * (1-top) * botLeft.y + (1-left) * top * topRight.y + (1-left) * (1-top) * botRight.y;

	return Ogre::Vector2((Ogre::Real)resultX, (Ogre::Real)resultY);
}

Ogre::Vector2 WindMap::getVector(Ogre::Vector2 position)
{
	return getVector(position.x/250, position.y/250);
}

Ogre::Vector2 WindMap::getAreaAverage(double topLeftx, double topLefty, double botRightx, double botRighty)
{
	// Placeholder functionality... replace asap
	return getVector(topLeftx, topLefty);
}
Ogre::Vector2 WindMap::getAreaAverage(Ogre::Vector2 topLeft, Ogre::Vector2 botRight)
{
	return getAreaAverage(topLeft.x, topLeft.y, botRight.x, botRight.y);
}

void WindMap::setVector(double posx, double posy, double strx, double stry)
{
	double resultX1;
	double resultY1;
	double resultX2;
	double resultY2;
	double resultX3;
	double resultY3;
	double resultX4;
	double resultY4;

	int flox = (int)floor(posx);
	int floy = (int)floor(posy);

	Ogre::Vector2 topLeft = vectorMap->getVector(flox, floy);
	Ogre::Vector2 botLeft = Ogre::Vector2(0, 0);
	if (1 + floy < sizeY)
	{
		botLeft = vectorMap->getVector(flox, 1 + floy);
	}
	Ogre::Vector2 topRight = Ogre::Vector2(0, 0);
	if (1 + flox < sizeX)
	{
		topRight = vectorMap->getVector(1 + flox, floy);
	}
	Ogre::Vector2 botRight = Ogre::Vector2(0, 0);
	if (1 + flox < sizeX && 1 + floy < sizeY)
	{
		botRight = vectorMap->getVector(1 + flox, 1 + floy);
	}

	double left = flox - posx + 1;
	double top = floy - posy + 1;

	resultX1 = left * top * strx + topLeft.x;
	resultY1 = left * top * stry + topLeft.y;
	resultX2 = left * (1-top) * strx + botLeft.x;
	resultY2 = left * (1-top) * stry + botLeft.y;
	resultX3 = (1-left) * top * strx + topRight.x;
	resultY3 = (1-left) * top * stry + topRight.y;
	resultX4 = (1-left) * (1-top) * strx + botRight.x;
	resultY4 = (1-left) * (1-top) * stry + botRight.y;

	vectorMap->setVector(flox, floy, resultX1, resultY1);
	if (1 + floy < sizeY)
	{
		vectorMap->setVector(flox, 1 + floy, resultX2, resultY2);
	}
	if (1 + flox < sizeX)
	{
		vectorMap->setVector(1 + flox, floy, resultX3, resultY3);
	}
	if (1 + flox < sizeX && 1 + floy < sizeY)
	{
		vectorMap->setVector(1 + flox, 1 + floy, resultX4, resultY4);
	}
}

void WindMap::setVector(Ogre::Vector2 position, Ogre::Vector2 strength)
{
	setVector(position.x, position.y, strength.x, strength.y);
}

void WindMap::updateArrows()
{
	vectorMap->updateArrows();
}

std::list<WindCurrent*>* WindMap::getWindCurrents()
{
	return &this->mCurrents;
}
