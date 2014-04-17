#include <list>
#include <math.h>
#include <algorithm>
#include <OgreRoot.h>
#include "WindMap.h"

using namespace Nimbus;

/** Arbitrary Corner enum used to make corner ordering more intuitive.
	Explicitly starts at zero for those wanting to understand the actual values.
*/
enum Corner {
	TOP_LEFT = 0, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT
};

float round(float x) {
	return (float)floor(x + 0.5);
}

WindMap::WindMap(Ogre::Real worldSize, Ogre::Real resolution, Ogre::Vector2 offset, Ogre::Real minimumCurrentLength) :
	mResolution(resolution),
	mAlphaVector(Ogre::Vector2(resolution, resolution)),
	mPersistenceFactor(Ogre::Real(.7)),
	mCurrents(),
	mOffset(offset),
	mMinimumCurrentLength(minimumCurrentLength)
{
	// Calculate vector map size
	int vectorMapWidth = (int)round(worldSize / resolution) + 1;
	int vectorMapHeight = (int)round(worldSize / resolution) + 1;

	// Initialize the vector map to the correct size and to zero vectors
	this->mVectorMap = new Grid<Ogre::Vector2>(vectorMapWidth, vectorMapHeight);
	this->mVectorMap->initialize(Ogre::Vector2::ZERO);

	// Create the arrow catcher listener
	this->mArrowCatcher = new ArrowCatcher();

	this->mArrowGrid = NULL;
}

WindMap::~WindMap(void)
{
	delete this->mVectorMap;
	delete this->mArrowCatcher;
}

void WindMap::update(void)
{
	// Grid to store influence vectors
	Grid<std::vector<Ogre::Vector2>*> pendingInfluenceVectors(this->mVectorMap->getXDimension(), this->mVectorMap->getYDimension());

	// Coordinates of the corners
	std::vector<Ogre::Vector2> cornerCoordinates;

	// List of distance to corner vectors ordered (topleft, topright, bottomright, bottomleft)
	std::vector<Ogre::Vector2> cornerDistance;

	// Used to find the vector map distance of specific wind influence vectors
	Ogre::Vector2 tempDistance;

	// Used to calculate the average wind vector
	Ogre::Vector2 averageWindVector;

	// Initialize the pending influence vectors grid
	for (int x = 0; x < pendingInfluenceVectors.getXDimension(); ++x)
	{
		for (int y = 0; y < pendingInfluenceVectors.getYDimension(); ++y)
		{
			pendingInfluenceVectors.set(x, y, new std::vector<Ogre::Vector2>());
			pendingInfluenceVectors.get(x, y)->push_back(Ogre::Vector2(0, 0));
		}
	}

	// Go through each wind current
	for (std::list<WindCurrent*>::iterator current = this->mCurrents.begin();
		current != this->mCurrents.end();
		++current)
	{
		// Go through each wind influence vector (stored as a position vector paired with an influence vector)
		for (pathList::iterator windInfluenceVector = (*current)->getPath()->begin();
			windInfluenceVector != (*current)->getPath()->end();
			++windInfluenceVector)
		{
			// Name the values for clarity
			Ogre::Vector2 positionVector = windInfluenceVector->first;
			Ogre::Vector2 deltaVector = windInfluenceVector->second;

			// Clear the previous corner coordinates and distances using zero vectors
			cornerCoordinates.assign(4, Ogre::Vector2::ZERO);
			cornerDistance.assign(4, Ogre::Vector2::ZERO);

			// Get the approximate position of the wind influence vector in terms of the vector map (rather than in world coordinates)
			tempDistance = (positionVector + this->mOffset) / this->mResolution;

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
					(int)cornerCoordinates[corner].y)->push_back(
						(1 - cornerDistance[corner].length() / this->mAlphaVector.length()) * deltaVector);
			}
		}

		// Increment the life timer for the wind current
		(*current)->incrementTimer();
	}

	// For the each of the pending influence vectors
	for (int x = 0; x < pendingInfluenceVectors.getXDimension(); ++x)
	{
		for (int y = 0; y < pendingInfluenceVectors.getYDimension(); ++y)
		{
			// Reset the average wind vector to zero
			averageWindVector = Ogre::Vector2::ZERO;

			std::vector<Ogre::Vector2>* tempList = pendingInfluenceVectors.get(x,y);

			// Sum all scaled delta vectors
			for (std::vector<Ogre::Vector2>::iterator vectorList = tempList->begin();
				vectorList != tempList->end();
				++vectorList)
			{
				averageWindVector += *vectorList;
			}

			if (tempList->size() != 0) {
				// Calculate the average scaled delta vector
				averageWindVector /= Ogre::Real(tempList->size());

				// Average the scaled delta vector average and the previous value, storing it back into the vector map
				this->mVectorMap->set(x, y,
					(1- this->mPersistenceFactor) * averageWindVector +
					(this->mPersistenceFactor) * this->mVectorMap->get(x,y));

				// Essentially zero
				if (this->mVectorMap->get(x, y).length() < 0.1)
				{
					this->mVectorMap->set(x, y, Ogre::Vector2(0, 0));
				}
			}
		}
	}

	// Delete any expired wind currents
	std::list<WindCurrent*>::iterator current = this->mCurrents.begin();
	while (current != this->mCurrents.end())
	{
		// Temporary currents last 10 seconds (as per GDD... hey it was actually useful guys!)
		if ((*current)->isTemporary() && (*current)->getTimeAlive() > 5)
		{
			// Delete the wind current
			delete *current;

			current = this->mCurrents.erase(current);

			if (current == this->mCurrents.end())
			{
				break;
			}
		}
		else
		{
			current++;
		}
	}

	// Create the arrow grid if necessary
	if(this->mArrowGrid == NULL)
	{
		createArrowGrid();
	}
	// Otherwise, update the arrows
	else
	{
		updateArrowGrid();
	}

	// Erase all allocated lists
	pendingInfluenceVectors.erase();
}

void WindMap::addWindCurrent(WindCurrent* windCurrent)
{
	// Don't add the wind current if it's too short.
	if (windCurrent->length() <= this->mMinimumCurrentLength)
	{
		std::stringstream truncatedCurrent;
		truncatedCurrent << "[NIMBUS]: " << "Wind current too short! Truncated current.\n"
			<< "(detail) " << "length: " << windCurrent->length() << "\n"
			<< "(detail) " << "current data: ";
		for (pathList::iterator itr = windCurrent->getPath()->begin(); itr != windCurrent->getPath()->end(); ++itr)
		{
			truncatedCurrent << "(" << itr->second.x << ", " << itr->second.y << ") ";
		}
		truncatedCurrent << "\n";
		Ogre::LogManager::getSingleton().logMessage(truncatedCurrent.str());
		return;
	}

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
	xPos = (int) round((x + mOffset.x) / mResolution);
	yPos = (int) round((y + mOffset.y) / mResolution);

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
	for (int x = (int)(ceil(topLeftx / mResolution) * mResolution); x < (int)(floor(botRightx / mResolution) * mResolution); x+=(int)mResolution) {
		for (int y = (int)(floor(topLefty / mResolution) * mResolution); y < (int)(ceil(botRighty / mResolution) * mResolution); y+=(int)mResolution) {
			vectorList.push_back(getWindVector(Ogre::Real(x), Ogre::Real(y)));
		}
	}

	for (size_t i = 0; i < vectorList.size(); i++)
	{
		averageVector += vectorList.at(i);
	}

	averageVector /= Ogre::Real(vectorList.size());

	return averageVector;
}

Ogre::Vector2 WindMap::getAverageWindVector(Ogre::Vector2 topLeft, Ogre::Vector2 botRight)
{
	std::vector<Ogre::Vector2> vectorList;
	Ogre::Vector2 averageVector = Ogre::Vector2::ZERO;

	// Get all vectors on the windmap between topLeft and botRight
	for (int x = (int)(ceil(topLeft.x / mResolution) * mResolution); x < (int)(floor(botRight.x / mResolution) * mResolution); x+=(int)mResolution) {
		for (int y = (int)(floor(topLeft.y / mResolution) * mResolution); y < (int)(ceil(botRight.y / mResolution) * mResolution); y+=(int)mResolution) {
			vectorList.push_back(getWindVector(Ogre::Real(x), Ogre::Real(y)));
		}
	}

	for (size_t i = 0; i < vectorList.size(); i++)
	{
		averageVector += vectorList.at(i);
	}

	averageVector /= Ogre::Real(vectorList.size());

	return averageVector;
}

std::list<WindCurrent*>* WindMap::getWindCurrents()
{
	return &this->mCurrents;
}

void WindMap::createArrowGrid()
{
	payloadmap createArrowPayload;
	std::string type = "BlueArrow";
	Ogre::Vector3 position;

	createArrowPayload["EntityType"] = &type;
	createArrowPayload["PositionVector"] = &position;

	// Create the arrow grid
	mArrowGrid = new Grid<GameEntityId>(this->mVectorMap->getXDimension(), this->mVectorMap->getYDimension());

	// Initialize the grid to entity ids of 0
	mArrowGrid->initialize(0);

	// Create the arrows
	for(int x = 0; x < mArrowGrid->getXDimension(); ++x)
	{
		for(int y = 0; y < mArrowGrid->getYDimension(); ++y)
		{
			// Calculate the position of the arrow
			position = this->mResolution * Ogre::Vector3(
				Ogre::Real(x), Ogre::Real(0), Ogre::Real(y)) - Ogre::Vector3(mOffset.x , 0, mOffset.y);

			// Create an arrow and capture the arrow id
			EventSystem::getSingleton()->fireEvent(
				EventSystem::EventType::CREATE_ENTITY,
				createArrowPayload,
				this->mArrowCatcher);

			// Store the entity id in the correct location
			mArrowGrid->set(x, y, this->mArrowCatcher->getEntityId());
		}
	}

	// Update the arrow grid facing vectors
	updateArrowGrid();
}

void WindMap::updateArrowGrid()
{
	// The update payload
	payloadmap updateArrowPayload;
	GameEntityId entityId;
	Ogre::Vector2 faceVector;
	Ogre::Vector3 face3dVector;

	// Preload the update payload
	updateArrowPayload["EntityId"] = &entityId;
	updateArrowPayload["FacingVector"] = &face3dVector;

	// Update the facing position of each arrow
	for(int x = 0; x < this->mArrowGrid->getXDimension(); ++x)
	{
		for(int y = 0; y < this->mArrowGrid->getYDimension(); ++y)
		{
			// Get the id of the arrow to update
			entityId = this->mArrowGrid->get(x, y);

			// Get the corresponding wind direction of the arrow
			faceVector = this->mVectorMap->get(x, y);

			// Convert the wind direction to 3d
			face3dVector = Ogre::Vector3(faceVector.x, 0, faceVector.y);

			// Send the position update event
			EventSystem::getSingleton()->fireEvent(EventSystem::EventType::POSITION_ENTITY, updateArrowPayload);
		}
	}
}

void WindMap::ArrowCatcher::handleEvent(payloadmap payload, EventListener* responder)
{
	if(payload.find("EntityId") != payload.end())
	{
		this->entityId = *static_cast<GameEntityId*>(payload["EntityId"]);
	}
}
