#ifndef NIMBUS_TILE_H
#define NIMBUS_TILE_H

#include <vector>

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
	public:
		// CLASS VARIABLES --
		/* The delta X of the Tile class. */
		static int deltaX;

		/* The delta Y of the Tile class. */
		static int deltaY;

		// CONSTRUCTORS --
		/* Blank constructor! */
		Tile(void);

		/* Constructor given the tile's humidity,temperature,
			ground saturation, andlocation.
		  @param loc The location of the Tile.
		  @param humid Tile Humidity
		  @param temper Tile Temperature
		  @param grnd Tile Ground Saturation
		*/
		Tile(double humid, double temper, double grnd, Point *loc );

		/* Destructor! */
		virtual ~Tile(void);
		
		// MEMBER FUNCTIONS --
		/* Calculate box of the tile. */
		void calculateBox(void);

		/* Convert the tile to string format! */
		void toString(void);
		
		/*Return humidity of a tile*/
		double getHumidity();
		
		/*Returns Temperature of a tile*/
		double getTemp();
		
		/*returns Ground Saturation of a tile*/
		double getSat();

		/*updates a tile's Ground Saturation,Humidity and Temperature*/
		void updateTile();
		
		/*Changes type of a tile into a string and returns that string*/
		String getType();
		
	private:
	
		
		void setHumidity();
		void setTemp();
		void setSat();
		void setType();		
		//to include: neighbor memory of some kind
		
	protected:
	
				// MEMBER VARIABLES --
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
		
		/*Holds this tile's biome.*/
		int biome;
		
		/*Holds this tile's humidity*/
		double humidity;
		
		/*Holds this tile's temperature*/
		double temp;
		
		/*Holds this tile's ground Saturation*/
		double grndSat;
				
		/*Holds whether or not this tile is burning*/		
		bool burning;
		
		/*Holds the limit of ground saturation before flooding*/
		double grndSatLmt;
		
		
		int tileId;
		
		//MEMBER FUNCTIONS
		/*Spawning function for animals*/
		void spawn();
	};
	
	
}

#endif