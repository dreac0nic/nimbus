#ifndef NIMBUS_TILE_H
#define NIMBUS_TILE_H

#include <vector>

#include <OGRE/OgreVector2.h>


namespace Nimbus
{
	//Temporary; fill or replace
	enum Biome { MOUNTAIN, DESERT, COAST, OCEAN };


	/* This holds the Tile class, originally named "Center." */
	class Tile
	{
	public:
		// CLASS VARIABLES --

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
		Tile(double humid, double temper, double grnd, Point *loc, double elev);

		/* Destructor! */

		
		// MEMBER FUNCTIONS --

		
		/*Return humidity of a tile*/
		double getHumidity();
		
		/*Returns Temperature of a tile*/
		double getTemp();
		
		/*returns Ground Saturation of a tile*/
		double getSat();

		/*updates a tile's Ground Saturation,Humidity and Temperature and stores them in 'next' Variable*/
		void updateTile();
		
		/*Assigns values in 'next' variables to current variables */
		void updateVar();

		/*called by outside function to let tile know if */
		void rainCloud();

		/*Changes type of a tile into a string and returns that string*/
		std::string getType();

		/*Gives humidity to a tile based on wind*/
		void giveHumi(Tile reciever);


	private:
		//MEMBER VARIABLES


		/* Holds this tile's elevation. */
		double elevation;

		/*Holds this tile's biome.*/
		int biome;

		/*Voronoi cell pointer*/
		void voroCellPoin;
		
		void setHumidity(double humi);
		void setTemp(double temp);
		void setSat(double grnd);
		void setType();		
		//to include: neighbor memory of some kind
		
	protected:
	
				// MEMBER VARIABLES --
		/*Holds this tiles's Humidity to be after update*/
		double nextHumidity;
		/*Holds this tiles's Temperature to be after update*/
		double nextTemperature;
		/*Holds this tiles's Ground Saturation to be after update*/		
		double nextGrndSat;
		
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

		/*Holds whether or not this tile is being rained on*/
		bool raining;

		/*Holds the biomes in a 3D array*/
		static int biomeArray[][][];

		/*Holds the positions of  Biome array*/
		int biomeX; //Temperature
		int biomeY; //Ground Saturation
		int biomeZ; //Humidity
		
		int tileId;
		
		//MEMBER FUNCTIONS
		/*Spawning function for animals*/
		void spawn();

		
	};
	
	
}

#endif