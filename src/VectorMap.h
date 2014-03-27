#ifndef VECTOR_MAP_H
#define VECTOR_MAP_H

#include <OgreRoot.h>

namespace Nimbus
{	
	/** Stores an array of 2D-Vectors
	 Used by WindMap
	 */
	class VectorMap
	{
	private:
		// Stores Vector2s in vector
		std::vector<Ogre::Vector2> map;
		// Stores size of one dimension of map
		int length;
	public:
		VectorMap(int xLength, int yLength);
		virtual ~VectorMap(void);

		// Gets Vector2 from location (x, y)
		Ogre::Vector2 getVector(int posx, int posy);

		// Sets Vector2 to strength (x, y) at position (x, y)
		void setVector(int posx, int posy, int strx, int stry);
	};
}
#endif
