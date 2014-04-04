#include <list>
#include <OgreRoot.h>
#include "WindCurrent.h"

using namespace Nimbus;

WindCurrent::WindCurrent(Ogre::Real strength, bool temporary) :
	mStrength(strength),
	mTemporary(temporary)
{
}

WindCurrent::~WindCurrent(void)
{
}

bool WindCurrent::operator==(const WindCurrent &other) const
{
	return other.mPath == mPath && other.mStrength == mStrength && other.mTemporary == mTemporary;
}

bool WindCurrent::operator!=(const WindCurrent &other) const
{
	return !(other == *this);
}
