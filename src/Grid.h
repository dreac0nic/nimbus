#ifndef NIMBUS_GRID_H
#define NIMBUS_GRID_H

namespace Nimbus
{	
	/** Stores a grid of objects of type T.
		Provides basic 2d array niceties... etc.
	 */
	template <class T>
	class Grid
	{
	private:
		// Stores the grid of objects
		std::vector<T> grid;

		// Stores size of one dimension of map
		int xLength;
		int yLength;

	public:
		/** Constructs a grid of the given x and y dimensions.
			@param xLength The X dimension of the grid
			@param yLength The Y dimension of the grid
		*/
		Grid(int xLength, int yLength)
		{
			grid.resize(xLength * yLength);
			this->xLength = xLength;
			this->yLength = yLength;
		}

		virtual ~Grid(void)
		{
		}

		/** Initializes the entire grid to the given value.
			@param value The value to which the entire grid should be initialized.
		*/
		void initialize(T value)
		{
			grid.assign(xLength * yLength, value);
		}

		/** Erases all the elements in the grid
			(though it may not delete them if they're pointers)
		*/
		void erase()
		{
			grid.clear();
		}

		/** Gets element from location (x, y)
			@param posx The x position of the element
			@param posy The y position of the element
			@return The value of the given element
		*/
		T get(int posx, int posy)
		{
			return grid[posx * xLength + posy];
		}

		/** Sets element to value at position (x, y)
			@param posx The x position of the element
			@param posy The y position of the element
			@param value The value to store in the given element
		*/
		void set(int posx, int posy, T value)
		{
			grid[posx * xLength + posy] = value;
		}

		/** Gets the x dimension of the grid.
			@return The size of the x dimension
		*/
		int getXDimension()
		{
			return xLength;
		}

		/** Gets the y dimension of the grid
			@return The size of the y dimension
		*/
		int getYDimension()
		{
			return yLength;
		}
	};
}
#endif
