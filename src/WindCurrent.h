#ifndef WIND_CURRENT_H
#define WIND_CURRENT_H

#include <list>
#include <OgreRoot.h>

namespace Nimbus
{
	/** Stores a path representing wind input
	 Used by WindMap
	 */
	class WindCurrent
	{
	public:
		WindCurrent(void);
		virtual ~WindCurrent(void);

		// Stores WindCurrent strength
		int strength;

		// Stores path's positions
		std::list<Ogre::Vector2> path;
	};
}
#endif
