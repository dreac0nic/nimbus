#include "Voronoi.h"

using namespace Nimbus::Voronoi;

std::stack<Site*> *Site::_pool = new std::stack<Site*>;
double const Site::EPSILON = .005;
int Site::_currentIndex=0;
bool Site::_sorted=false;
std::vector<Site*> *Site::_sites;

Site::Site(Point *p, int index, double weight){
	init(p, index, weight);
}

Site::~Site(){

}

bool Site::operator< (const Site &other) const{
	if (_coord.y < other._coord.y) {
		return -1;
	}
	if (_coord.y > other._coord.y) {
		return 1;
	}
	if (_coord.x < other._coord.x) {
		return -1;
	}
	if (_coord.x > other._coord.x) {
		return 1;
	}
	return 0;
}

Point *Site::getCoord(){
	return &_coord;
}

void Site::addEdge(Edge *edge){
	_edges->insert(_edges->end(), edge);
}

int Site::comp(const void *a, const void*b){
	return (int)Edge::compareSitesDistances((Edge*)a, (Edge*)b);
}

Edge *Site::nearestEdge(){
	std::sort(_edges->begin(),_edges->end());
	return _edges->at(0);
}


std::vector<Site*> *Site::neighborSites(){
	if (_edges == NULL || _edges->empty()) {
		return new std::vector<Site*>;
	}
	if (_edgeOrientations == NULL) {
		reorderEdges();
	}
	std::vector<Site*> *list = new std::vector<Site*>;
	for (int i = 0; i < _edges->size(); i++) {
		list->insert(list->end(), neighborSite(_edges->at(i)));
	}
	return list;
}

Site *Site::neighborSite(Edge *edge){
	if (this == edge->getLeftSite()) {
		return edge->getRightSite();
	}
	if (this == edge->getRightSite()) {
		return edge->getLeftSite();
	}
	return NULL;
}

std::vector<Point*> *Site::region(Rectangle *clippingbounds){
	if (_edges == NULL || _edges->empty()) {
		return new std::vector<Point*>();
	}
	if (_edgeOrientations == NULL) {
		reorderEdges();
		_region = clipToBounds(clippingbounds);
		if ((new Polygon(_region))->winding() == CLOCKWISE) {
			std::reverse(_region->begin(),_region->end());
		}
	}
	return _region;
}

double Site::getX(){
	return _coord.x;
}

double Site::getY(){
	return _coord.y;
}

double Site::dist(Point *p){
	return _coord.distance(*p);
}

Site *Site::create(Point *p, int index, double weight){
	if (_pool->size() > 0) {
		Site * temp= _pool->top()->init(p, index, weight);
		_pool->pop();
		return temp;
	} else {
		return new Site(p, index, weight);
	}
}

bool Site::closeEnough(Point *p0, Point *p1){
	return p0->distance(*p1) < EPSILON;
}

int Site::check(Point *point, Rectangle *bounds){
	int value = 0;
	if (point->x == bounds->left) {
		value |= LEFT;
	}
	if (point->x == bounds->right) {
		value |= RIGHT;
	}
	if (point->y == bounds->top) {
		value |= TOP;
	}
	if (point->y == bounds->bottom) {
		value |= BOTTOM;
	}
	return value;
}

void Site::clear(){
	if (_edges != NULL) {
		_edges->clear();
		_edges = NULL;
	}
	if (_edgeOrientations != NULL) {
		_edgeOrientations->clear();
		_edgeOrientations = NULL;
	}
	if (_region != NULL) {
		_region->clear();
		_region = NULL;
	}
}

void Site::reorderEdges(){
	Edge::reorderByVertex(_edges);
	_edges = Edge::getEdges();
	_edgeOrientations = Edge::getEdgeOrientations();
	//reorderer.dispose();
}

std::vector<Point*> *Site::clipToBounds(Rectangle *bounds){
	std::vector<Point*> *points = new std::vector<Point*>();
	int n = _edges->size();
	int i = 0;
	Edge *edge;
	while (i < n && (_edges->at(i)->getVisible() == false)) {
		++i;
	}

	if (i == n) {
		// no edges visible
		return points;
	}
	edge = _edges->at(i);
	LR orientation = _edgeOrientations->at(i);
	points->insert(points->end(), edge->getClippedEnds()->at(orientation));
	points->insert(points->end(), edge->getClippedEnds()->at((LR)((orientation+1)%2)));

	for (int j = i + 1; j < n; ++j) {
		edge = _edges->at(j);
		if (edge->getVisible() == false) {
			continue;
		}
		connect(points, j, bounds, false);
	}
	// close up the polygon by adding another corner point of the bounds if needed:
	connect(points, i, bounds, true);

	return points;
}

void Site::connect(std::vector<Point*> *points, int j, Rectangle *bounds, bool closingUp){
	Point *rightPoint = points->at(points->size() - 1);
	Edge *newEdge = _edges->at(j);
	LR newOrientation = _edgeOrientations->at(j);
	// the point that  must be connected to rightPoint:
	Point *newPoint = newEdge->getClippedEnds()->at(newOrientation);
	if (!closeEnough(rightPoint, newPoint)) {
		// The points do not coincide, so they must have been clipped at the bounds;
		// see if they are on the same border of the bounds:
		if (rightPoint->x != newPoint->x
			&& rightPoint->y != newPoint->y) {
				// They are on different borders of the bounds;
				// insert one or two corners of bounds as needed to hook them up:
				// (NOTE this will not be correct if the region should take up more than
				// half of the bounds rect, for then we will have gone the wrong way
				// around the bounds and included the smaller part rather than the larger)
				int rightCheck = check(rightPoint, bounds);
				int newCheck = check(newPoint, bounds);
				double px, py;
				if ((rightCheck & RIGHT) != 0) {
					px = bounds->right;
					if ((newCheck & BOTTOM) != 0) {
						py = bounds->bottom;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & TOP) != 0) {
						py = bounds->top;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & LEFT) != 0) {
						if (rightPoint->y - bounds->y + newPoint->y - bounds->y < bounds->height) {
							py = bounds->top;
						} else {
							py = bounds->bottom;
						}
						points->insert(points->end(), new Point(px, py));
						points->insert(points->end(), new Point(bounds->left, py));
					}
				} else if ((rightCheck & LEFT) != 0) {
					px = bounds->left;
					if ((newCheck & BOTTOM) != 0) {
						py = bounds->bottom;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & TOP) != 0) {
						py = bounds->top;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & RIGHT) != 0) {
						if (rightPoint->y - bounds->y + newPoint->y - bounds->y < bounds->height) {
							py = bounds->top;
						} else {
							py = bounds->bottom;
						}
						points->insert(points->end(), new Point(px, py));
						points->insert(points->end(), new Point(bounds->right, py));
					}
				} else if ((rightCheck & TOP) != 0) {
					py = bounds->top;
					if ((newCheck & RIGHT) != 0) {
						px = bounds->right;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & LEFT) != 0) {
						px = bounds->left;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & BOTTOM) != 0) {
						if (rightPoint->x - bounds->x + newPoint->x - bounds->x < bounds->width) {
							px = bounds->left;
						} else {
							px = bounds->right;
						}
						points->insert(points->end(), new Point(px, py));
						points->insert(points->end(), new Point(px, bounds->bottom));
					}
				} else if ((rightCheck & BOTTOM) != 0) {
					py = bounds->bottom;
					if ((newCheck & RIGHT) != 0) {
						px = bounds->right;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & LEFT) != 0) {
						px = bounds->left;
						points->insert(points->end(), new Point(px, py));
					} else if ((newCheck & TOP) != 0) {
						if (rightPoint->x - bounds->x + newPoint->x - bounds->x < bounds->width) {
							px = bounds->left;
						} else {
							px = bounds->right;
						}
						points->insert(points->end(), new Point(px, py));
						points->insert(points->end(), new Point(px, bounds->top));
					}
				}
		}
		if (closingUp) {
			// newEdge's ends have already been added
			return;
		}
		points->insert(points->end(), newPoint);
	}
	Point *newRightPoint = newEdge->getClippedEnds()->at((LR)((newOrientation+1)%2));
	if (!closeEnough(points->at(0), newRightPoint)) {
		points->insert(points->end(), newRightPoint);
	}
}

Site *Site::init(Point *p, int index, double weight){
	_coord = *p;
	_siteIndex = index;
	Site::weight = weight;
	_edges = new std::vector<Edge*>();
	_region = NULL;
	return this;
}

void Site::initList(){
	_sites = new std::vector<Site *>();
	_sorted = false;
}

void Site::disposeList(){
	if (_sites != NULL){
		_sites->clear();
		_sites = NULL;
	}
}

int Site::push(Site *site){
	_sorted = false;
	_sites->push_back(site);
	return _sites->size();
}

int Site::getLength(){
	return _sites->size();
}

Site *Site::next(){
	if (_sorted == false) {
		return NULL;
	}
	if (_currentIndex < _sites->size()) {
		return _sites->at(_currentIndex++);
	} else {
		return NULL;
	}
}

Rectangle *Site::getSitesBounds(){
	if (_sorted == false) {
		std::sort(_sites->begin(), _sites->end());
		_currentIndex = 0;
		_sorted = true;
	}
	double xmin, xmax, ymin, ymax;
	if (_sites->empty()) {
		return new Rectangle(0, 0, 0, 0);
	}
	xmin = DBL_MAX;
	xmax = FLT_MIN;
	for (int i = 0; i < _sites->size(); i++) {
		if (_sites->at(i)->getX() < xmin) {
			xmin = _sites->at(i)->getX();
		}
		if (_sites->at(0)->getX() > xmax) {
			xmax = _sites->at(0)->getX();
		}
	}
	// here's where we assume that the sites have been sorted on y:
	ymin = _sites->at(0)->getY();
	ymax = _sites->at(_sites->size() - 1)->getY();

	return new Rectangle(xmin, ymin, xmax - xmin, ymax - ymin);
}

std::vector<Point *> *Site::siteCoords(){
	std::vector<Point*> *coords = new std::vector<Point*>();
	for (int i = 0; i < _sites->size(); i++) {
		coords->insert(coords->end(), _sites->at(i)->getCoord());
	}
	return coords;
}

std::vector< std::vector<Point *> *> *Site::regions(Rectangle * plotBounds){
	std::vector< std::vector<Point *> *> *regions = new std::vector< std::vector<Point *> *>();
	for (int i = 0; i < _sites->size(); i++) {
		regions->insert(regions->end(), _sites->at(i)->region(plotBounds));
	}
	return regions;
}
