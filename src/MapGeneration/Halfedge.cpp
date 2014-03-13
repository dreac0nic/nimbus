#include "Voronoi.h"

using namespace Nimbus::Voronoi;

std::stack<Halfedge*> *Halfedge::_pool = new std::stack<Halfedge*>();
std::vector<Halfedge*> *Halfedge::_hash = new std::vector<Halfedge*>();
int Halfedge::_count = 0;
int Halfedge::_minBucket = 0;
int Halfedge::_hashSize = 0;
float Halfedge::_ymin = 0;
float Halfedge::_deltay = 0;

Halfedge *Halfedge::init(Edge *edge, LR lr){
	Halfedge::edge = edge;
	leftRight = lr;
	nextInPriorityQueue = NULL;
	vertex = NULL;
	return this;
}

Halfedge::Halfedge(Edge *edge, LR lr){
	init(edge, lr);
}
Halfedge::~Halfedge(){}

Halfedge *Halfedge::create(Edge *edge, LR lr){
	if (_pool->size() > 0) {
		Halfedge *temp = _pool->top()->init(edge, lr);
		_pool->pop();
		return temp;
	} else {
		return new Halfedge(edge, lr);
	}
}

Halfedge *Halfedge::createDummy(){
	return create(NULL, LR_LEFT);
}

bool Halfedge::isLeftOf(Point *p){
	Site *topSite;
	bool rightOfSite, above, fast;
	double dxp, dyp, dxs, t1, t2, t3, yl;

	topSite = edge->getRightSite();
	rightOfSite = p->x > topSite->getX();
	if (rightOfSite && leftRight == LR_LEFT) {
		return true;
	}
	if (!rightOfSite && leftRight == LR_RIGHT) {
		return false;
	}

	if (edge->a == 1.0) {
		dyp = p->y - topSite->getY();
		dxp = p->x - topSite->getX();
		fast = false;
		if ((!rightOfSite && edge->b < 0.0) || (rightOfSite && edge->b >= 0.0)) {
			above = dyp >= edge->b * dxp;
			fast = above;
		} else {
			above = p->x + p->y * edge->b > edge->c;
			if (edge->b < 0.0) {
				above = !above;
			}
			if (!above) {
				fast = true;
			}
		}
		if (!fast) {
			dxs = topSite->getX() - edge->getLeftSite()->getX();
			above = edge->b * (dxp * dxp - dyp * dyp)
				< dxs * dyp * (1.0 + 2.0 * dxp / dxs + edge->b * edge->b);
			if (edge->b < 0.0) {
				above = !above;
			}
		}
	} else /* edge->b == 1.0 */ {
		yl = edge->c - edge->a * p->x;
		t1 = p->y - yl;
		t2 = p->x - topSite->getX();
		t3 = yl - topSite->getY();
		above = t1 * t1 > t2 * t2 + t3 * t3;
	}
	return leftRight == LR_LEFT ? above : !above;
}

//////////////////Priority Queue Functions//////////////////

bool Halfedge::isEmpty(int bucket){
	return (_hash->at(bucket)->nextInPriorityQueue == NULL);
}

void Halfedge::adjustMinBucket(){
	while (_minBucket < _hashSize - 1 && isEmpty(_minBucket)) {
		++_minBucket;
	}
}

void Halfedge::initQueue(float ymin, float deltay, int sqrt_nsites){
	_ymin = ymin;
	_deltay = deltay;
	_hashSize = 4 * sqrt_nsites;
	int i;

	_count = 0;
	_minBucket = 0;
	_hash = new std::vector<Halfedge*>(_hashSize, Halfedge::createDummy());

	for (i = 0; i < _hashSize; ++i) {
		_hash->at(i)->nextInPriorityQueue = NULL;
	}
}

void Halfedge::disposeQueue(){
	_hash = NULL;
}

void Halfedge::insert(Halfedge *halfEdge){
	Halfedge *previous;
	Halfedge *next;
	int insertionBucket = bucket(halfEdge);
	if (insertionBucket < _minBucket) {
		_minBucket = insertionBucket;
	}
	previous = _hash->at(insertionBucket);
	while ((next = previous->nextInPriorityQueue) != NULL && (halfEdge->ystar > next->ystar || (halfEdge->ystar == next->ystar && halfEdge->vertex->getX() > next->vertex->getX()))) {
		previous = next;
	}
	halfEdge->nextInPriorityQueue = previous->nextInPriorityQueue;
	previous->nextInPriorityQueue = halfEdge;
	++_count;
}

void Halfedge::remove(Halfedge *halfEdge){
	Halfedge *previous;
	int removalBucket = bucket(halfEdge);

	if (halfEdge->vertex != NULL) {
		previous = _hash->at(removalBucket);
		while (previous->nextInPriorityQueue != halfEdge) {
			previous = previous->nextInPriorityQueue;
		}
		previous->nextInPriorityQueue = halfEdge->nextInPriorityQueue;
		_count--;
		halfEdge->vertex = NULL;
		halfEdge->nextInPriorityQueue = NULL;
	}
}

int Halfedge::bucket(Halfedge *halfEdge){
	int theBucket = (int) ((halfEdge->ystar - _ymin) / _deltay * _hashSize);
	if (theBucket < 0) {
		theBucket = 0;
	}
	if (theBucket >= _hashSize) {
		theBucket = _hashSize - 1;
	}
	return theBucket;
}

bool Halfedge::empty(){
	return _count == 0;
}

Point *Halfedge::min(){
	adjustMinBucket();
	Halfedge *answer = _hash->at(_minBucket)->nextInPriorityQueue;
	return new Point(answer->vertex->getX(), answer->ystar);
}

Halfedge *Halfedge::extractMin(){
	Halfedge *answer;

	// get the first real Halfedge in _minBucket
	answer = _hash->at(_minBucket)->nextInPriorityQueue;

	_hash->at(_minBucket)->nextInPriorityQueue = answer->nextInPriorityQueue;
	_count--;
	answer->nextInPriorityQueue = NULL;

	return answer;
}
