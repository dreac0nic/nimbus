#include "Voronoi.h"

using namespace Nimbus::Voronoi;

Polygon::Polygon(std::vector<Point*> *vertices){
	_vertices = vertices;
}

Polygon::~Polygon(){

}

float Polygon::signedDoubleArea(){
	int index, nextIndex;
	int n = _vertices->size();
	Point *point, *next;
	float signedDoubleArea = 0;
	for (index = 0; index < n; ++index) {
		nextIndex = (index + 1) % n;
		point = _vertices->at(index);
		next = _vertices->at(nextIndex);
		signedDoubleArea += point->x * next->y - next->x * point->y;
	}
	return signedDoubleArea;
}

float Polygon::area(){
	return (float)std::abs(signedDoubleArea() * 0.5);
}

Winding Polygon::winding(){
	float sda = signedDoubleArea();
	if (sda < 0) {
		return CLOCKWISE;
	}
	if (sda > 0) {
		return COUNTERCLOCKWISE;
	}
	return NONE;
}
