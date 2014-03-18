#include "Voronoi.h"

using namespace Nimbus::Voronoi;

Rectangle::Rectangle(){
	width = height = left = top = right = bottom = 0;
}

Rectangle::Rectangle(double x1, double y1, double width1, double height1){
	width = width1;
	height = height1;
	left = x = x1;
	top = y = y1;
	right = x + width;
	bottom = y + height;
}

bool Rectangle::liesOnAxes(Point *p){
	return Util::closeEnough(p->x, x, 1) || Util::closeEnough(p->y, y, 1) || Util::closeEnough(p->x, right, 1) || Util::closeEnough(p->y, bottom, 1);
}

bool Rectangle::inBounds(Point *p){
	return inBounds(p->x, p->y);
}

bool Rectangle::inBounds(double x0, double y0){
	if (x0 < x || x0 > right || y0 < y || y0 > bottom) {
		return false;
	}
	return true;
}