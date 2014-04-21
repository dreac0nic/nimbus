#include "Corner.h"

#include <sstream>

#include <OGRE\OgreVector3.h>

using namespace Nimbus;

Corner::Corner(void)
{
}

Corner::~Corner(void)
{
}

std::string Corner::str(void)
{
	// Create string stream for storing the information from the tile.
	std::stringstream buffer;

	// Fill the buffer.
	buffer << "Corner; [" << loc->x << ", " << loc->y << ": e" << elevation;

	return buffer.str();
}

Ogre::Vector3 Corner::vec3(void)
{
	Ogre::Vector3 pointVector3((Ogre::Real)this->loc->x, (Ogre::Real)this->elevation, (Ogre::Real)this->loc->y);

	return pointVector3;
}