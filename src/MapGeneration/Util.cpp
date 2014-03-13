#include "Voronoi.h"

using namespace Nimbus::Voronoi;

bool Util::closeEnough(float d1, float d2, float diff){
	return abs(d1 - d2) <= diff;
}//end closeEnough