#include "Voronoi.h"

using namespace Nimbus::Voronoi;

std::pair<Point*, Point*> *points;

LineSegment::~LineSegment(){

}

LineSegment::LineSegment(std::pair<Point*, Point*> *points){
	LineSegment::points = points;
}

LineSegment::LineSegment(Point *point0, Point *point1){
	points->first = point0;
	points->second = point1;
}

float LineSegment::compareLengthsMax(LineSegment *segment0, LineSegment *segment1){
	float length0 = segment0->points->first->distance(*(segment0->points->second));
	float length1 = segment1->points->first->distance(*(segment1->points->second));
	if (length0 < length1) {
		return 1;
	}
	if (length0 > length1) {
		return -1;
	}
	return 0;
}

float LineSegment::compareLengths(LineSegment *edge0, LineSegment *edge1){
	return -compareLengthsMax(edge0, edge1);
}
