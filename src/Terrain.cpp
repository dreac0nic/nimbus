#include "Terrain.h"
#include "World.h"

using namespace boost::polygon;
using namespace Nimbus;

template <>
struct geometry_concept<Point> { typedef point_concept type; };
   
template <>
struct point_traits<Point> {
  typedef int coordinate_type;
    
  static inline coordinate_type get(const Point& point, orientation_2d orient) {
    return (orient == HORIZONTAL) ? point.x : point.y;
  }
};

Terrain::Terrain(World* world) : mWorld(world)
{
	// We generate our pointlist using 35^2-2 (1223) or 36^2-5 (1291) points -- good primes
	for (int i = 0; i < 1291; i++) {
		this->pointList.push_back(Point(rand() % (int)mWorld->getWorldBounds().x,
										rand() % (int)mWorld->getWorldBounds().y));
	}

	construct_voronoi(pointList.begin(), pointList.end(), &diagram);
}

Terrain::~Terrain(void)
{

}
