#include <list>
#include <utility>
#include <OgreRoot.h>
#include "WindCurrent.h"

using namespace Nimbus;
using namespace Ogre;

WindCurrent::WindCurrent(Real strength, bool temporary) :
	mStrength(strength),
	mTemporary(temporary),
	mPath(),
	mTimeAlive(0)
{
}

WindCurrent::~WindCurrent(void)
{
}

void WindCurrent::addPoint(Vector2 position, Vector2 influence)
{
	// Add the pair to the path list
	this->mPath.push_back(std::pair<Ogre::Vector2, Ogre::Vector2>(position, influence));
}

void WindCurrent::incrementTimer()
{
	this->mTimeAlive++;
}

pathList WindCurrent::getPath()
{
	return this->mPath;
}

Real WindCurrent::getStrength()
{
	return this->mStrength;
}

bool WindCurrent::isTemporary()
{
	return this->mTemporary;
}

int WindCurrent::getTimeAlive()
{
	return this->mTimeAlive;
}

bool WindCurrent::operator==(const WindCurrent &other) const
{
	return other.mPath == mPath && other.mStrength == mStrength && other.mTemporary == mTemporary;
}

bool WindCurrent::operator!=(const WindCurrent &other) const
{
	return !(other == *this);
}
