#include "Corner.h"

#include <OGRE\OgreVector3.h>

using namespace Nimbus;

Corner::Corner(void)
{
}

Corner::~Corner(void)
{
}

bool Corner::operator< (const Corner &other) const
{
	return (elevation < other.elevation);
}

void Corner::toString()
{
	std::cout << "Corner, x:" << loc->x << ", y:" << loc->y << " " << (water ? "w" : "-") << (ocean ? "o" : "-") << (coast ? "c" : "-");
}

Ogre::Vector3 Corner::getVector3(void)
{
	Ogre::Vector3 pointVector3();

	return Ogre::Vector3(this->loc->x, this->elevation, this->loc->y);
}