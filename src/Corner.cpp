#include "Corner.h"

#include <OGRE\OgreVector3.h>

using namespace Nimbus;

Corner::Corner(void)
{
}

Corner::~Corner(void)
{
}

void Corner::toString()
{
	std::cout << "Corner, x:" << loc->x << ", y:" << loc->y;
}

Ogre::Vector3 Corner::getVector3(void)
{
	Ogre::Vector3 pointVector3((Ogre::Real)this->loc->x, (Ogre::Real)this->elevation, (Ogre::Real)this->loc->y);

	return pointVector3;
}