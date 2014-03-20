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
		/* Vector containing all the tiles that this corner touches.
		  (All the tiles that use this corner to define its borders)
		*/
		std::vector<Tile*> touches;

		/* Vector containing all the corners touched by this corner via its edges. */
		std::vector<Corner*> adjacent;

		/* The edges extending from the corner.*/
		std::vector<TileEdge*> protrudes;

		/* The location of the point. */
		Point *loc;
		
		/* The index of this corner. */
		int index;

		/* Contains whether or not this corner is the border corner. */
		bool border;

		/* The elevation of this corner. */
		double elevation;

		/* Weather or not this corner is a water tile. */
		bool water;

		/* Weather or not this corner is a water ocean. */
		bool ocean;

		/* Weather or not this corner is a coast. */
		bool coast;

		/* Has a pointer to the corner that is the heaviest downslope. */
		Corner *downslope;

		/* River? */
		int river;

		/* The moisture of this corner. */
		double moisture;

		// CONSTRUCTORS --
		/* Blank default constructor. */
		Corner(void);

		/* Destructor */
		virtual ~Corner(void);

		// MEMBER FUNCTIONS --
		/* Less-then comparison operator for Corner, comparing to other corners. */
		bool operator< (const Corner &other) const;

		/* Convert the corner to a string. */
		void toString(void);
	};
}

#endif