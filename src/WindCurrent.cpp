#include <list>
#include <OgreRoot.h>
#include "WindCurrent.h"

namespace Nimbus
{
	WindCurrent::WindCurrent(void)
	{
	}

	WindCurrent::~WindCurrent(void)
	{
	}

	bool WindCurrent::operator==(const WindCurrent &other) const
	{
		return other.path == path && other.strength == strength && other.temp == temp;
	}

	bool WindCurrent::operator!=(const WindCurrent &other) const
	{
		return !(other == *this);
	}
}
