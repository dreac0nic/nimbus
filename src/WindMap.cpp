#include <list>
#include <math.h>
#include <algorithm>
#include <OgreRoot.h>
#include "WindCurrent.h"
#include "WindMap.h"

using namespace Nimbus;

/** Arbitrary Corner enum used to make corner ordering more intuitive.
	Explicitly starts at zero for those wanting to understand the actual values.
*/
enum Corner {
	TOP_LEFT = 0, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT
};

WindMap::WindMap(Ogre::Real worldSize, Ogre::Real resolution) :
	mResolution(resolution),
	mAlphaVector(Ogre::Vector2(resolution, resolution)),
	mPersistenceFactor(.5),
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
	// Grid to store influence vectors
	Grid<std::vector<Ogre::Vector2> > pendingInfluenceVectors(this->mVectorMap->getXDimension(), this->mVectorMap->getYDimension());

	// Coordinates of the corners
	std::vector<Ogre::Vector2> cornerCoordinates;

	// List of distance to corner vectors ordered (topleft, topright, bottomright, bottomleft)
	std::vector<Ogre::Vector2> cornerDistance;

	// Used to find the vector map distance of specific wind influence vectors
	Ogre::Vector2 tempDistance;

	// Used to calculate the average wind vector
	Ogre::Vector2 averageWindVector;

	// Initialize the pending influence vectors grid
	pendingInfluenceVectors.initialize(std::vector<Ogre::Vector2>());

	// Go through each wind current
	for(std::list<WindCurrent*>::iterator current = this->mCurrents.begin(); current != this->mCurrents.end(); ++current)
	{
		// Go through each wind influence vector (stored as a position vector paired with an influence vector)
		for(pathList::iterator windInfluenceVector = (*current)->getPath()->begin(); windInfluenceVector != (*current)->getPath()->end(); ++windInfluenceVector)
		{
			// Name the values for clarity
			Ogre::Vector2 positionVector = windInfluenceVector->first;
			Ogre::Vector2 deltaVector = windInfluenceVector->second;

			// Clear the previous corner coordinates and distances using zero vectors
			cornerCoordinates.assign(4, Ogre::Vector2::ZERO);
			cornerDistance.assign(4, Ogre::Vector2::ZERO);

			// Get the approximate position of the wind influence vector in terms of the vector map (rather than in world coordinates)
			tempDistance = positionVector / this->mResolution;

			// Calculate the corner coordinates
			cornerCoordinates[Corner::TOP_LEFT] = Ogre::Vector2(floor(tempDistance.x), floor(tempDistance.y));
			cornerCoordinates[Corner::TOP_RIGHT] = Ogre::Vector2(ceil(tempDistance.x), floor(tempDistance.y));
			cornerCoordinates[Corner::BOTTOM_RIGHT] = Ogre::Vector2(ceil(tempDistance.x), ceil(tempDistance.y));
			cornerCoordinates[Corner::BOTTOM_LEFT] = Ogre::Vector2(floor(tempDistance.x), ceil(tempDistance.y));

			// For each corner around the wind influence vector
			for(int corner = Corner::TOP_LEFT; corner <= Corner::BOTTOM_LEFT; ++corner)
			{
				// Get distance vectors to each adjacent grid vector
				cornerDistance[corner] = tempDistance - cornerCoordinates[corner];

				// Store the scaled delta vector (excuse my [gl]OOP and math)
				pendingInfluenceVectors.get(
				(int)cornerCoordinates[corner].x,
				(int)cornerCoordinates[corner].y).push_back(
					(1 - cornerDistance[corner].length() / this->mAlphaVector.length()) * deltaVector);
			}
		}

		// Increment the life timer for the wind current
		(*current)->incrementTimer();
	}

	// For the each of the pending influence vectors
	for(int x = 0; x < pendingInfluenceVectors.getXDimension(); ++x)
	{
		for(int y = 0; y < pendingInfluenceVectors.getYDimension(); ++y)
		{
			// Reset the average wind vector to zero
			averageWindVector = Ogre::Vector2::ZERO;

			// Sum all scaled delta vectors
			for(std::vector<Ogre::Vector2>::iterator vectorList = pendingInfluenceVectors.get(x,y).begin();
				vectorList != pendingInfluenceVectors.get(x,y).end();
				++vectorList)
			{
				averageWindVector += *vectorList;
			}

			// Calculate the average scaled delta vector
			averageWindVector /= Ogre::Real(pendingInfluenceVectors.get(x,y).size());

			// Average the scaled delta vector average and the previous value, storing it back into the vector map
			this->mVectorMap->set(x,y,
				(1- this->mPersistenceFactor) * averageWindVector +
				(this->mPersistenceFactor) * this->mVectorMap->get(x,y));
		}
	}

	// Delete any expired wind currents
	for(std::list<WindCurrent*>::iterator current = this->mCurrents.begin(); current != this->mCurrents.end(); ++current)
	{
		// Temporary currents last 10 seconds (as per GDD... hey it was actually useful guys!)
		if((*current)->isTemporary() && (*current)->getTimeAlive() > 10)
		{
			// Delete the wind current
			delete *current;

			current = this->mCurrents.erase(current);
		}
	}
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
		delete *toDelete;
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
