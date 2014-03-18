#include "Voronoi.h"

using namespace Nimbus::Voronoi;

bool Util::closeEnough(double d1, double d2, double diff){
	return abs(d1 - d2) <= diff;
}//end closeEnough
