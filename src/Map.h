#ifndef NIMBUS_MAP_H
#define NIMBUS_MAP_H

#include <utility>

#include <OGRE/OgreVector2.h>

#include "Tile.h"

namespace Nimbus
{
	class Map;

	class Map
	{
	public:
		//////////
		// Member Variables

		// Map content objects

		/* List of all tile centers on the map. */
		std::vector<Tile*> tiles;

		//////////
		// Public Member Functions

		/** Default constructor taking no agruments.
		  Generates a simple set of tiles for debug use.
		*/
		Map(void);

		/** Convert the map into an OGRE mesh for rendering! */
		Ogre::MeshPtr getMesh(std::string meshName);
	};
}

#endif
