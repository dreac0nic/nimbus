#ifndef VORONOI_PARABOLA_H
#define VORONOI_PARABOLA_H

#include "Site.h"
#include "Event.h"
#include "Edge.h"

namespace Nimbus
{
	namespace Voronoi
	{
		class Parabola
		{
		public:
			Parabola(void);
			Parabola(Site* focalPoint);

		private:
			Site* focalPoint;
			Edge* containedEdge;
			Event* circleEvent;

			Parabola* parent;
			Parabola* leftChild;
			Parabola* rightChild;

		public:
			bool isLeafNode(void) { return (leftChild != NULL && rightChild != NULL) ? true : false; }
		};
	}
}

#endif VORONOI_PARABOLA_H