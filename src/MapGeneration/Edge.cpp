#include "Voronoi.h"

using namespace Nimbus::Voronoi;

Edge *Edge::DELETED = new Edge();
int Edge::_numEdges = 0;
std::stack<Edge*> *Edge::_pool = new std::stack<Edge*>();
std::vector<Edge*> *Edge::_edges = new std::vector<Edge*>();
std::vector<LR> *Edge::_edgeOrientations = new std::vector<LR>();
std::vector<Halfedge*> *Edge::_hash = new std::vector<Halfedge*>();
Halfedge *Edge::_leftEnd;
Halfedge *Edge::_rightEnd;
float Edge::_deltax = 0;
float Edge::_xmin = 0;
int Edge::_hashSize = 0;

///////////////Edge functions///////////////

Edge *Edge::create(){
	Edge *edge;
	if (_pool->size() > 0) {
		edge = _pool->top();
		_pool->pop();
		edge->init();
	} else {
		edge = new Edge();
	}
	return edge;
}

Edge *Edge::createBisectingEdge(Site *site0, Site *site1){
	float dx, dy, absdx, absdy;
	float a, b, c;
	Edge *edge;

	dx = site1->getX() - site0->getX();
	dy = site1->getY() - site0->getY();
	absdx = dx > 0 ? dx : -dx;
	absdy = dy > 0 ? dy : -dy;
	c = site0->getX() * dx + site0->getY() * dy + (dx * dx + dy * dy) * 0.5;
	if (absdx > absdy) {
		a = 1.0;
		b = dy / dx;
		c /= dx;
	} else {
		b = 1.0;
		a = dx / dy;
		c /= dy;
	}//end else

	edge = create();

	edge->setLeftSite(site0);
	edge->setRightSite(site1);
	site0->addEdge(edge);
	site1->addEdge(edge);

	edge->a = a;
	edge->b = b;
	edge->c = c;

	return edge;
}//end createBisectingEdge

void Edge::init(){
	_sites = new std::map<LR, Site*>();
}

Edge::Edge(){
	_edgeIndex = _numEdges++;
	init();
}

Edge::~Edge(){

}

float Edge::compareSitesDistances_MAX(Edge *edge0, Edge *edge1){
	double length0 = edge0->sitesDistance();
	double length1 = edge1->sitesDistance();
	if (length0 < length1) {
		return 1;
	}
	if (length0 > length1) {
		return -1;
	}
	return 0;
}

float Edge::compareSitesDistances(Edge *edge0, Edge *edge1){
	return -compareSitesDistances_MAX(edge0, edge1);
}

void Edge::clipVertices(Rectangle *bounds){
	float xmin = bounds->x;
	float ymin = bounds->y;
	float xmax = bounds->right;
	float ymax = bounds->bottom;

	Vertex *vertex0, *vertex1;
	float x0, x1, y0, y1;

	if (a == 1.0 && b >= 0.0) {
		vertex0 = _rightVertex;
		vertex1 = _leftVertex;
	} else {
		vertex0 = _leftVertex;
		vertex1 = _rightVertex;
	}

	if (a == 1.0) {
		y0 = ymin;
		if (vertex0 != NULL && vertex0->getY() > ymin) {
			y0 = vertex0->getY();
		}
		if (y0 > ymax) {
			return;
		}
		x0 = c - b * y0;

		y1 = ymax;
		if (vertex1 != NULL && vertex1->getY() < ymax) {
			y1 = vertex1->getY();
		}
		if (y1 < ymin) {
			return;
		}
		x1 = c - b * y1;

		if ((x0 > xmax && x1 > xmax) || (x0 < xmin && x1 < xmin)) {
			return;
		}

		if (x0 > xmax) {
			x0 = xmax;
			y0 = (c - x0) / b;
		} else if (x0 < xmin) {
			x0 = xmin;
			y0 = (c - x0) / b;
		}

		if (x1 > xmax) {
			x1 = xmax;
			y1 = (c - x1) / b;
		} else if (x1 < xmin) {
			x1 = xmin;
			y1 = (c - x1) / b;
		}
	} else {
		x0 = xmin;
		if (vertex0 != NULL && vertex0->getX() > xmin) {
			x0 = vertex0->getX();
		}
		if (x0 > xmax) {
			return;
		}
		y0 = c - a * x0;

		x1 = xmax;
		if (vertex1 != NULL && vertex1->getX() < xmax) {
			x1 = vertex1->getX();
		}
		if (x1 < xmin) {
			return;
		}
		y1 = c - a * x1;

		if ((y0 > ymax && y1 > ymax) || (y0 < ymin && y1 < ymin)) {
			return;
		}

		if (y0 > ymax) {
			y0 = ymax;
			x0 = (c - y0) / a;
		} else if (y0 < ymin) {
			y0 = ymin;
			x0 = (c - y0) / a;
		}

		if (y1 > ymax) {
			y1 = ymax;
			x1 = (c - y1) / a;
		} else if (y1 < ymin) {
			y1 = ymin;
			x1 = (c - y1) / a;
		}
	}

	_clippedVertices = new std::map<LR, Point*>();
	if (vertex0 == _leftVertex) {
		_clippedVertices->insert(std::pair<LR, Point*>(LR_LEFT, new Point(x0, y0)));
		_clippedVertices->insert(std::pair<LR, Point*>(LR_RIGHT, new Point(x1, y1)));
	} else {
		_clippedVertices->insert(std::pair<LR, Point*>(LR_RIGHT, new Point(x0, y0)));
		_clippedVertices->insert(std::pair<LR, Point*>(LR_LEFT, new Point(x1, y1)));
	}
}

LineSegment *Edge::delaunayLine(){
	return new LineSegment(getLeftSite()->getCoord(), getRightSite()->getCoord());
}

std::map<LR, Point*> *Edge::getClippedEnds(){
	return _clippedVertices;
}

Site *Edge::getLeftSite(){
	return _sites->at(LR_LEFT);
}

Vertex *Edge::getLeftVertex(){
	return _leftVertex;
}

Site *Edge::getRightSite(){
	return _sites->at(LR_RIGHT);
}

Vertex *Edge::getRightVertex(){
	return _rightVertex;
}

bool Edge::getVisible(){
	return _clippedVertices != NULL;
}

bool Edge::isPartOfConvexHull(){
	return (_leftVertex == NULL || _rightVertex == NULL);
}

void Edge::setVertex(LR leftRight, Vertex *v){
	if (leftRight == LR_LEFT) {
		_leftVertex = v;
	} else {
		_rightVertex = v;
	}
}

void Edge::setLeftSite(Site *s){
	_sites->insert(std::pair<LR, Site*>(LR_LEFT, s));
}

void Edge::setRightSite(Site *s){
	_sites->insert(std::pair<LR, Site*>(LR_RIGHT, s));
}

Site *Edge::site(LR leftRight){
	return _sites->at(leftRight);
}

float Edge::sitesDistance(){
	return(getLeftSite()->getCoord()->distance(*(getRightSite()->getCoord())));
}

Vertex *Edge::vertex(LR leftRight){
	return (leftRight == LR_LEFT) ? _leftVertex : _rightVertex;
}

LineSegment *Edge::voronoiEdge(){
	if (!getVisible()) {
		return new LineSegment(NULL, NULL);
	}
	return new LineSegment(_clippedVertices->at(LR_LEFT),
		_clippedVertices->at(LR_RIGHT));
}

///////////////Edge list functions///////////////

Halfedge *Edge::getHash(int b){
	Halfedge *halfEdge;

	if (b < 0 || b >= _hashSize) {
		return NULL;
	}
	halfEdge = _hash->at(b);
	if (halfEdge != NULL && halfEdge->edge == Edge::DELETED) {
		/* Hash table points to deleted halfedge.  Patch as necessary. */
		_hash->at(b) = NULL;
		// still can't dispose halfEdge yet!
		return NULL;
	} else {
		return halfEdge;
	}
}

void Edge::initList(float xmin, float deltax, int sqrt_nsites){
	_xmin = xmin;
	_deltax = deltax;
	_hashSize = 2 * sqrt_nsites;

	_hash = new std::vector<Halfedge*>();

	// two dummy Halfedges:
	_leftEnd = Halfedge::createDummy();
	_rightEnd = Halfedge::createDummy();
	_leftEnd->edgeListLeftNeighbor = NULL;
	_leftEnd->edgeListRightNeighbor = _rightEnd;
	_rightEnd->edgeListLeftNeighbor = _leftEnd;
	_rightEnd->edgeListRightNeighbor = NULL;

	_hash->at(0) = _leftEnd;
	_hash->at(_hashSize - 1) = _rightEnd;
}

void Edge::insert(Halfedge *lb, Halfedge *newHalfedge){
	newHalfedge->edgeListLeftNeighbor = lb;
	newHalfedge->edgeListRightNeighbor = lb->edgeListRightNeighbor;
	lb->edgeListRightNeighbor->edgeListLeftNeighbor = newHalfedge;
	lb->edgeListRightNeighbor = newHalfedge;
}

void Edge::remove(Halfedge *halfEdge){
	halfEdge->edgeListLeftNeighbor->edgeListRightNeighbor = halfEdge->edgeListRightNeighbor;
	halfEdge->edgeListRightNeighbor->edgeListLeftNeighbor = halfEdge->edgeListLeftNeighbor;
	halfEdge->edge = Edge::DELETED;
	halfEdge->edgeListLeftNeighbor = halfEdge->edgeListRightNeighbor = NULL;
}

Halfedge *Edge::edgeListLeftNeighbor(Point *p){
	int i, bucket;
	Halfedge *halfEdge;

	/* Use hash table to get close to desired halfedge */
	bucket = (int) ((p->x - _xmin) / _deltax * _hashSize);
	if (bucket < 0) {
		bucket = 0;
	}
	if (bucket >= _hashSize) {
		bucket = _hashSize - 1;
	}
	halfEdge = getHash(bucket);
	if (halfEdge == NULL) {
		for (i = 1; true; ++i) {
			if ((halfEdge = getHash(bucket - i)) != NULL) {
				break;
			}
			if ((halfEdge = getHash(bucket + i)) != NULL) {
				break;
			}
		}
	}
	/* Now search linear list of halfedges for the correct one */
	if (halfEdge == _leftEnd || (halfEdge != _rightEnd && halfEdge->isLeftOf(p))) {
		do {
			halfEdge = halfEdge->edgeListRightNeighbor;
		} while (halfEdge != _rightEnd && halfEdge->isLeftOf(p));
		halfEdge = halfEdge->edgeListLeftNeighbor;
	} else {
		do {
			halfEdge = halfEdge->edgeListLeftNeighbor;
		} while (halfEdge != _leftEnd && !halfEdge->isLeftOf(p));
	}

	/* Update hash table and reference counts */
	if (bucket > 0 && bucket < _hashSize - 1) {
		_hash->at(bucket) = halfEdge;
	}
	return halfEdge;
}

//////////////Edge sorting functions//////////////


void Edge::initQueue(){
	_edges = new std::vector<Edge*>();
	_edgeOrientations = new std::vector<LR>();
}

std::vector<Edge*> *Edge::getEdges(){
	return _edges;
}

std::vector<LR> *Edge::getEdgeOrientations(){
	return _edgeOrientations;
}

std::vector<Edge*> *Edge::reorderBySite(std::vector<Edge*> *origEdges){
	int i;
	int n = origEdges->size();
	Edge *edge;
	// we're going to reorder the edges in order of traversal
	std::vector<bool> *done = new std::vector<bool>;

	done->insert(done->begin(), n, false);

	int nDone = 0;

	std::vector<Edge*> *newEdges = new std::vector<Edge*>();
	std::vector<Edge*>::iterator newEdgesIt = newEdges->begin();

	std::vector<LR>::iterator edgeOrientIt = _edgeOrientations->begin();

	i = 0;
	edge = origEdges->at(i);
	newEdges->at(i) = edge;
	edgeOrientIt = _edgeOrientations->insert(edgeOrientIt, LR_LEFT);
	Site *firstPoint = edge->getLeftSite();
	Site *lastPoint = edge->getRightSite();

	done->at(i) = true;
	++nDone;

	while (nDone < n) {
		for (i = 1; i < n; ++i) {
			if (done->at(i)) {
				continue;
			}
			edge = origEdges->at(i);
			Site *leftPoint = edge->getLeftSite();
			Site *rightPoint = edge->getRightSite();

			if (leftPoint == lastPoint) {
				lastPoint = rightPoint;
				edgeOrientIt = _edgeOrientations->insert(edgeOrientIt, LR_LEFT);
				newEdgesIt = newEdges->insert(newEdgesIt, edge);
				done->at(i) = true;
			} else if (rightPoint == firstPoint) {
				firstPoint = leftPoint;
				edgeOrientIt = _edgeOrientations->insert(_edgeOrientations->begin(), LR_LEFT);
				newEdges->insert(newEdges->begin(), edge);
				done->at(i) = true;
			} else if (leftPoint == firstPoint) {
				firstPoint = rightPoint;
				edgeOrientIt = _edgeOrientations->insert(_edgeOrientations->begin(), LR_RIGHT);
				newEdgesIt = newEdges->insert(newEdges->begin(), edge);

				done->at(i) = true;
			} else if (rightPoint == lastPoint) {
				lastPoint = leftPoint;
				edgeOrientIt = _edgeOrientations->insert(edgeOrientIt, LR_RIGHT);
				newEdgesIt = newEdges->insert(newEdgesIt, edge);
				done->at(i) = true;
			}
			if (done->at(i)) {
				++nDone;
			}
		}
	}

	return newEdges;
}

std::vector<Edge*> *Edge::reorderByVertex(std::vector<Edge*> *origEdges){
	int i;
	int n = origEdges->size();
	Edge *edge;
	// we're going to reorder the edges in order of traversal
	std::vector<bool> *done = new std::vector<bool>();

	done->insert(done->begin(), n, false);

	int nDone = 0;

	std::vector<Edge*> *newEdges = new std::vector<Edge*>();
	std::vector<Edge*>::iterator newEdgesIt = newEdges->begin();

	std::vector<LR>::iterator edgeOrientIt = _edgeOrientations->begin();

	i = 0;
	edge = origEdges->at(i);
	newEdges->at(i) = edge;
	edgeOrientIt = _edgeOrientations->insert(edgeOrientIt, LR_LEFT);
	Vertex *firstPoint = edge->getLeftVertex();
	Vertex *lastPoint = edge->getRightVertex();

	if (firstPoint == Vertex::VERTEX_AT_INFINITY || lastPoint == Vertex::VERTEX_AT_INFINITY) {
		return new std::vector<Edge*>;
	}

	done->at(i) = true;
	++nDone;

	while (nDone < n) {
		for (i = 1; i < n; ++i) {
			if (done->at(i)) {
				continue;
			}
			edge = origEdges->at(i);
			Vertex *leftPoint = edge->getRightVertex();
			Vertex *rightPoint = edge->getRightVertex();
			if (firstPoint == Vertex::VERTEX_AT_INFINITY || lastPoint == Vertex::VERTEX_AT_INFINITY) {
				return new std::vector<Edge*>;
			}
			if (leftPoint == lastPoint) {
				lastPoint = rightPoint;
				edgeOrientIt = _edgeOrientations->insert(edgeOrientIt, LR_LEFT);
				newEdgesIt = newEdges->insert(newEdgesIt, edge);
				done->at(i) = true;
			} else if (rightPoint == firstPoint) {
				firstPoint = leftPoint;
				edgeOrientIt = _edgeOrientations->insert(_edgeOrientations->begin(), LR_LEFT);
				newEdges->insert(newEdges->begin(), edge);
				done->at(i) = true;
			} else if (leftPoint == firstPoint) {
				firstPoint = rightPoint;
				edgeOrientIt = _edgeOrientations->insert(_edgeOrientations->begin(), LR_RIGHT);
				newEdgesIt = newEdges->insert(newEdges->begin(), edge);

				done->at(i) = true;
			} else if (rightPoint == lastPoint) {
				lastPoint = leftPoint;
				edgeOrientIt = _edgeOrientations->insert(edgeOrientIt, LR_RIGHT);
				newEdgesIt = newEdges->insert(newEdgesIt, edge);
				done->at(i) = true;
			}
			if (done->at(i)) {
				++nDone;
			}
		}
	}

	return newEdges;
}