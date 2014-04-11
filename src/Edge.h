#ifndef VORONOI_EDGE_H
#define VORONOI_EDGE_H

#include "Site.h"

namespace Nimbus
{
	namespace Voronoi
	{
		class Edge
		{
		public:
			Edge(Site startSite);
			~Edge(void);

		private:
			Site startSite;
			Site endSite;

		public:
			Site* getStartSite(void) { return &this->startSite; }
			Site* getEndSite(void) { return &this->endSite; }
			void setEndSite(Site endSite) { this->endSite = endSite; }
		};
	}
}

#endif VORONOI_EDGE_H