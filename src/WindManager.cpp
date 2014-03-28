#include "WindMap.h"
#include "VectorMap.h"
#include "WindCurrent.h"
#include "WindManager.h"

using namespace Nimbus;

WindManager::WindManager(int sizex, int sizey)
{
	windMap = new WindMap(sizex, sizey);
}

WindManager::~WindManager(void)
{
}

bool WindManager::update(void)
{
	for(int i = 0; i < windMap->sizeX; i++)
	{
		for(int j = 0; i < windMap->sizeY; j++)
		{
			Ogre::Vector2 currentVector = windMap->getVector(i, j);
			Ogre::Vector2 temp;
			double totalWindX = currentVector.x;
			double totalWindY = currentVector.y;

			if(i != 0)
			{
				if(j != 0)
				{
					temp = windMap->getVector(i-1, j-1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}

				temp = windMap->getVector(i-1, j);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;

				if(j != windMap->sizeY-1)
				{
					temp = windMap->getVector(i-1, j+1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}
			}

			if(j != 0)
			{
				temp = windMap->getVector(i, j-1);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;
			}

			if(j != windMap->sizeY-1)
			{
				temp = windMap->getVector(i, j+1);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;
			}

			if(i != windMap->sizeX-1)
			{
				if(j != 0)
				{
					temp = windMap->getVector(i+1, j-1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}

				temp = windMap->getVector(i+1, j);
				totalWindX += (temp.x - currentVector.x) * SIDEINFLUENCE;
				totalWindY += (temp.y - currentVector.y) * SIDEINFLUENCE;

				if(j != windMap->sizeY-1)
				{
					temp = windMap->getVector(i+1, j+1);
					totalWindX += (temp.x - currentVector.x) * CORNERINFLUENCE;
					totalWindY += (temp.y - currentVector.y) * CORNERINFLUENCE;
				}
			}
			windMap->setVector(i, j, totalWindX * ORIGININFLUENCE, totalWindY * ORIGININFLUENCE);
		}
	}

	for (WindCurrent current : windMap->currents)
	{
		double strength = current.strength;

		if(current.strength > 0){
			Ogre::Vector2 firstPos = Ogre::Vector2::NEGATIVE_UNIT_X;

			for(Ogre::Vector2 secondPos : current.path)
			{
				Ogre::Vector2 secondVec = windMap->getVector(secondPos);

				if(firstPos != Ogre::Vector2::NEGATIVE_UNIT_X){
					Ogre::Vector2 firstVec = windMap->getVector(firstPos);
					Ogre::Vector2 temp = Ogre::Vector2((Ogre::Real)((secondVec.x - firstVec.x) * strength), (Ogre::Real)((secondVec.y - firstVec.y) * strength));

					windMap->setVector(firstPos, temp);
				}

				firstPos = secondPos;
			}
			if(current.temp) {
				current.strength -= STRENGTHTOSUBTRACT;
			}
		} else 
		{
			windMap->currents.remove(current);
		}
	}
	return true;
}
