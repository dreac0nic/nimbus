#ifndef NIMBUS_TILEEDGE_H
#define NIMBUS_TILEEDGE_H

#include <OGRE\OgreVector2.h>

namespace nimbus
{
	// Class forward-declarations.
	/* Tile holds the Voronoi tile of the map. */
	class Tile;

	/* Corner holds a corner or the meeting of edges of the tile. */
	class Corner;

	/* Typedef Vector2 to Point, to improve readability. */
	typedef Ogre::Vector2 Point;

	/* The TileEdge class holds the information for the edge of a tile. */
	class TileEdge
	{
	public:
		int index;
		int river;

		Tile *d0, *d1;
		Corner *v0, *v1;
		Point *midpoint;

		/* Blank TileEdge constructor. */
		TileEdge(void);

		/* DESTRUCTOR */
		virtual ~TileEdge(void);

		void setVoronoi(Corner *c0, Corner *c1);
		void toString();
	};
}

#endif