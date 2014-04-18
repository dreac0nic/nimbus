#ifndef NIMBUS_CORNER_H
#define NIMBUS_CORNER_H

#include <vector>

#include <OGRE/OgreVector2.h>

namespace Nimbus
{
	// Class forward-declarations.
	/* Tile holds the information about the tile on the map/terrain. */
	class Tile;
	
	/* The TileEdge class holds the information for the edge of a tile. */
	class TileEdge;

	/* Typedef Vector2 to Point, to improve readability. */
	typedef Ogre::Vector2 Point;
	
	/* Corner holds a corner or the meeting of edges of the tile. */
	class Corner
	{
	public:
		/* The location of the point. */
		Point *loc;

		/* The elevation of this corner. */
		double elevation;

		// CONSTRUCTORS --
		/* Blank default constructor. */
		Corner(void);

		/* Destructor */
		virtual ~Corner(void);

		// MEMBER FUNCTIONS --
		/* Convert the corner to a string. */
		void toString(void);

		/* Convert the corner to a OGRE Vector3. */
		Ogre::Vector3 getVector3(void);
	};
}

#endif