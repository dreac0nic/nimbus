#include "WindMap.h"

using namespace Nimbus;
using namespace Ogre;

WindMap::WindMap(int x, int y)
{
}

WindMap::~WindMap()
{
}

/* Definitely here just as a placeholder... testing only. */
Vector2 WindMap::getVector(double x, double y)
{
	return Vector2::UNIT_X;
}

Vector2 WindMap::getVector(Vector2 position)
{
	return Vector2::UNIT_X;
}

Vector2 getAreaAverage(double topX, double topY, double botX, double botY)
{
	return Vector2::UNIT_X;
}

Vector2 getAreaAverage(Vector2 topLeft, Vector2 botRight)
{
	return Vector2::UNIT_X;
}