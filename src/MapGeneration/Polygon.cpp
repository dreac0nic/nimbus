#include "Polygon.h"

using namespace Nimbus::Voronoi;

Polygon::Polygon(std::vector<Point*> *vertices){
	_vertices = *vertices;
}

Polygon::~Polygon(){

}

double Polygon::signedDoubleArea(){
	int index, nextIndex;
	int n = _vertices.size();
	Point *point, *next;
	double signedDoubleArea = 0;
	for (index = 0; index < n; ++index) {
		nextIndex = (index + 1) % n;
		point = _vertices.at(index);
		next = _vertices.at(nextIndex);
		signedDoubleArea += point->x * next->y - next->x * point->y;
	}
	return signedDoubleArea;
}

Winding Polygon::winding(){
	double sda = signedDoubleArea();
	if (sda < 0) {
		return CLOCKWISE;
	}
	if (sda > 0) {
		return COUNTERCLOCKWISE;
	}
	return NONE;
}
