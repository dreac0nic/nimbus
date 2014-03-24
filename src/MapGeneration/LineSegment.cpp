#include "Voronoi.h"

using namespace Nimbus::Voronoi;


LineSegment::~LineSegment(){
}

LineSegment::LineSegment(std::pair<Point*, Point*> *points){
	this->points = *points;
}

LineSegment::LineSegment(Point *point0, Point *point1){
	points.first = point0;
	points.second = point1;
}

double LineSegment::compareLengthsMax(LineSegment *segment0, LineSegment *segment1){
	double length0 = segment0->points.first->distance(*(segment0->points.second));
	double length1 = segment1->points.first->distance(*(segment1->points.second));
	if (length0 < length1) {
		return 1;
	}
	if (length0 > length1) {
		return -1;
	}
	return 0;
}

double LineSegment::compareLengths(LineSegment *edge0, LineSegment *edge1){
	return -compareLengthsMax(edge0, edge1);
}
