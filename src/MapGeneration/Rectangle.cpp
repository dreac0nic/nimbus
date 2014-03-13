#include "Voronoi.h"

using namespace Nimbus::Voronoi;

Rectangle::Rectangle(float x, float y, float width, float height){
	Rectangle::width = width;
	Rectangle::height = height;
	left = Rectangle::x = x;
	top = Rectangle::y = y;
	right = x + width;
	bottom = y + height;
}

bool Rectangle::liesOnAxes(Point *p){
	return Util::closeEnough(p->x, x, 1) || Util::closeEnough(p->y, y, 1) || Util::closeEnough(p->x, right, 1) || Util::closeEnough(p->y, bottom, 1);
}

bool Rectangle::inBounds(Point *p){
	return inBounds(p->x, p->y);
}

bool Rectangle::inBounds(float x0, float y0){
	if (x0 < x || x0 > right || y0 < y || y0 > bottom) {
		return false;
	}
	return true;
}