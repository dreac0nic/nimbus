#ifndef NIMBUS_VERTEX_H
#define NIMBUS_VERTEX_H

#include <stack>

#include "Voronoi.h"

namespace Nimbus
{
	namespace Voronoi
	{
		/** Provides a way to create points at the intersection of two edges.
			Tracks the total number of vertices and stores them by index.
		*/
		class Vertex
		{
		private:
			static std::stack<Vertex*> *_pool;
			static int _numVertices;
			Point *_coord;
			int _vertIndex;

			static Vertex *create(double x, double y);
			Vertex(double x, double y);
			Vertex();
		public:
			static Vertex *VERTEX_AT_INFINITY;
			~Vertex();

			Vertex *init(double x, double y);
			Point *getCoord();
			int getVertexIndex();
			double getX();
			double getY();
			void setIndex();

			//This is the only way to make a vertex
			static Vertex *intersect(Halfedge *halfedge0, Halfedge *halfedge1);
		};
	}
}

#endif