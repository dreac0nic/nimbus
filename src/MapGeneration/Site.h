#ifndef NIMBUS_SITE_H
#define NIMBUS_SITE_H

#include <stack>
#include <vector>

#include "Voronoi.h"

namespace Nimbus
{
	namespace Voronoi
	{
		/** Sites equate to the contents of a tile.
			The center of a site is a random point, and the interior contains all points closer to that vertex
			than any other.  The edges of each site are formed along the perpendicular bisector between two
			neighboring vertices.
		*/
		class Site
		{
		private:
			static std::stack<Site*> _pool;
			static const double EPSILON;
			static double compare(Site *s1, Site *s2);
			static bool closeEnough(Point *p0, Point *p1);
			static int check(Point *point, Rectangle *bounds);
			static std::vector<Site*> _sites;
			static int _currentIndex;
			static bool _sorted;

			Point _coord;
			int _siteIndex;
			std::vector<LR> *_edgeOrientations;
			std::vector<Point*> *_region;
			void clear();
			void reorderEdges();
			std::vector<Point*> *clipToBounds(Rectangle *bounds);
			void connect(std::vector<Point*> *points, int j, Rectangle *bounds, bool closingUp);
			Site *init(Point *p, int index, double weight);

		public:
			static Site *create(Point *p, int index, double weight);
			static void initList();
			static void disposeList();
			static int push(Site *site);
			static int getLength();
			static Site *next();
			static Rectangle *getSitesBounds();
			static std::vector<Point *> *siteCoords();
			static std::vector< std::vector<Point *> *> *regions(Rectangle * plotBounds);

			bool operator< (const Site &other) const;

			double weight;
			std::vector<Edge*> *_edges;
			Site(Point *p, int index, double weight);
			~Site();
			int comp(const void *a,const void *b);
			Point *getCoord();
			void addEdge(Edge *edge);
			Edge *nearestEdge();
			std::vector<Site*> *neighborSites();
			Site *neighborSite(Edge *edge);
			std::vector<Point*> *region(Rectangle *clippingbounds);
			double getX();
			double getY();
			double dist(Point *p);
		};
	}
}

#endif