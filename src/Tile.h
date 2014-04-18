#ifndef NIMBUS_TILE_H
#define NIMBUS_TILE_H

#include <vector>

#include <OGRE/OgreMesh.h>
#include <OGRE/OgreVector2.h>


namespace Nimbus
{
	//Temporary; fill or replace
	enum Biome { MOUNTAIN, DESERT, COAST, OCEAN };

	// Class forward-declarations! Whoo!
	class Corner;
	class TileEdge;

	/* Typedef Vector2 to Point, to improve readability. */
	typedef Ogre::Vector2 Point;

	/* This holds the Tile class, originally named "Center." */
	class Tile
	{
	private:
		void _generateSubMesh(Ogre::MeshPtr& mesh);

	public:
		// CLASS VARIABLES --
		/* The delta X of the Tile class. */
		static int deltaX;

		/* The delta Y of the Tile class. */
		static int deltaY;

		// MEMEBER VARIABLES --
		/* The index of this tile. */
		int index;

		/* The location of this tile. */
		Point loc;

		/* A vector containing all the corners defining the tile. */
		std::vector<Corner*> corners;

		/* A vector containing all the neighboring tiles. */
		std::vector<Tile*> neighbors;

		/* A vector containing the borders of the tile. */
		std::vector<TileEdge*> borders;

		/* Whether or not this is a border tile. */
		bool border;

		/* Holds whether not this tile is an ocean. */
		bool ocean;

		/* Holds whether not this tile is a water tile. */
		bool water;

		/* Holds whether not this tile is a coast. */
		bool coast;

		/* Holds this tile's elevation. */
		double elevation;

		/* Holds this tile's moisture level. */
		double moisture;

		/* Holds this tile's area. */
		double area;

		/* Holds this tile's biome. */
		Biome biome;

		// CONSTRUCTORS --
		/* Blank constructor! */
		Tile(void);

		/* Constructor given the tile's location.
		  @param loc The location of the Tile.
		*/
		Tile(Point *loc);

		/* Destructor! */
		virtual ~Tile(void);
		
		// MEMBER FUNCTIONS --
		/* Convert the tile to string format! */
		void toString(void);

		/* Convert the tile into a OGRE mesh. */
		Ogre::MeshPtr getMesh(std::string);

		/* Add this tile as a sub-mesh to the current OGRE mesh. */
		bool addSubMesh(Ogre::MeshPtr& mesh);
	};
}

#endif