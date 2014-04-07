#ifndef NIMBUS_HALFEDGE_H
#define NIMBUS_HALFEDGE_H

#include <stack>
#include <vector>

#include "Edge.h"
#include "Voronoi.h"

namespace Nimbus
{
	namespace Voronoi
	{
		/*	Each Edge has two Halfedges (right and left).
			Halfedges are useful in creating Corners where Edges meet.
		*/
		class Halfedge
		{
		private:
			static std::stack<Halfedge*> _pool;
			static std::vector<Halfedge*> _hash;
			static int _count;
			static int _minBucket;
			static int _hashSize;
			static double _ymin;
			static double _deltay;
			static bool isEmpty(int bucket);
			static void adjustMinBucket();

			Halfedge *init(Edge *edge, LR lr);

		public:
			static void initQueue(double ymin, double deltay, int sqrt_nsites);
			static void disposeQueue();
			static void insert(Halfedge *halfEdge);
			static void remove(Halfedge *halfEdge);
			static int bucket(Halfedge *halfEdge);
			static bool empty();
			static Point *min();
			static Halfedge *extractMin();

			Halfedge *edgeListLeftNeighbor;
			Halfedge *edgeListRightNeighbor;
			Halfedge *nextInPriorityQueue;
			Edge *edge;
			Vertex *vertex;
			LR leftRight;
			double ystar;
			Halfedge(Edge *edge, LR lr);
			~Halfedge();
			static Halfedge *create(Edge *edge, LR lr);
			static Halfedge *createDummy();
			bool isLeftOf(Point *p);
		};
	}
}

#endif