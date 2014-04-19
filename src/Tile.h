#ifndef NIMBUS_TILE_H
#define NIMBUS_TILE_H

#include <vector>

#include <boost/polygon/voronoi.hpp>

#include <OGRE/OgreMesh.h>
#include <OGRE/OgreVector2.h>


namespace Nimbus
{
	// Typedef for voronoi celltype.
	typedef boost::polygon::voronoi_diagram_traits<double>::cell_type cellType;

	//Temporary; fill or replace
	enum Biome { MOUNTAIN, DESERT, COAST, OCEAN };

	// Class forward-declarations! Whoo!
	class Corner;

	/**  */
	class Tile
	{
	private:
		void _generateSubMesh(Ogre::MeshPtr& mesh);

	protected:
		/// A list of all neighbouring tiles that we can traverse.
		std::vector<Tile*> mNeighbours;

		/// The cell that this tile is represented by.
		cellType* mCell;

		/// A vector containing all the corners defining the tile.
		std::vector<Corner*> mCorners;

		/// The position of this tile's center.
		Ogre::Vector2 mPosition;

		/// The elevation of this cell.
		double elevation;

		/// Contains the biome of this tile.
		Biome biome;

		/** The ground saturation limit for this tile.
		    When breached, the tile is flooded.
		*/
		double grndSatLimit;

		/// Holds the change to this tile's humidity to be applied after update.
		double nextHumidity;

		/// Holds the change to this tile's temperature to be applied after update.
		double nextTemperature;

		/// Holds the change to this tile's ground saturation to be applied after update.
		double nextGrndSat;

		/// Holds this tile's current humidity.
		double humidity;

		/// Holds this tile's current temperature.
		double temperature;

		/// Holds this tile's current ground saturation.
		double grndSat;

		/// Tells whether or not this tile is burning.
		bool burning;

		/// Tells whether or not this tile is being rained on.
		bool raining;

		/// Wind power for calculation.
		Ogre::Real windPower;

	public:
		// CONSTRUCTORS --
		/** Blank constructor! */
		Tile(void);

		/** Constructor given the tile's location.
		  @param loc The location of the Tile.
		*/
		Tile(Ogre::Real position);

		/** Constructor given the tile's associated cell.
		  @param cell The associated cell for this tile.
		*/
		Tile(const cellType cell);

		/* Destructor! */
		virtual ~Tile(void);
		
		// MEMBER FUNCTIONS --
		// -- ACCESSORS
		/// Returns a pointer to the cell that identifies this tile.
		cellType* getCell(void) { return this->mCell; }

		/// Sets the center position of this tile.
		void setPosition(Ogre::Real x, Ogre::Real y) { this->mPosition = Ogre::Vector2(x, y); }

		/// Returns this tile's center position.
		Ogre::Vector2 getPosition(void) { return this->mPosition; }

		/// Sets this tile's center elevation.
		void setElevation(double elevation) { this->elevation = elevation; }

		/// Returns this tile's center elevation.
		double getElevation(void) { return this->elevation; }

		/// Sets the tile's current humidity to humi.
		void setHumidity(double humi) { this->humidity = humi; }

		/// Returns the tile's current humidity.
		double getHumidity(void) { return this->humidity; }

		/// Sets the tile's current temperature to temp.
		void setTemp(double temp) { this->temperature = temp; }

		/// Returns the tile's current temperature.
		double getTemp(void) { return this->temperature; }

		/// Sets the tile's current ground saturation to grnd.
		void setSat(double grnd) { this->grndSat = grnd; }

		/// Returns the tile's current ground saturation.
		double getSat(void) { return this->grndSat; }

		/// Sets the tile's current biome type to type.
		void setType(Biome type) { this->biome = type; }

		/// Returns the tile's current tile type.
		Biome getType(void) { return this->biome; }

		/** A fire-once function to set the neighbours of a tile.
		    In normal operation, this should only be called once after generation.

		    @param neighbours A vector of Tiles that neighbour this tile.
		*/
		void setNeighbours(std::vector<Tile*> neighbours) { this->mNeighbours = neighbours; }

		/// Returns a pointer to the list of neighbouring tiles.
		std::vector<Tile*>* getNeigbours(void) { return &(this->mNeighbours); }

		/** A all-or-nothing accessor for setting all of the corners in this tile.

		    @param corners A vector of pointers to Corners forming this tile.
		*/
		void setCorners(std::vector<Corner*> corners) { this->mCorners = corners; }

		// -- TILE MANAGEMENT
		/// Updates the tile based its internal values.
		void updateTile(void);

		/// Rolls over the next values to the tile's new values.
		void updateVar(void);

		// -- CONVERSIONS
		/* Convert the tile to string format! */
		void toString(void);

		/* Convert the tile into a OGRE mesh. */
		Ogre::MeshPtr getMesh(std::string);

		/* Add this tile as a sub-mesh to the current OGRE mesh. */
		bool addSubMesh(Ogre::MeshPtr& mesh);
	};
}

#endif