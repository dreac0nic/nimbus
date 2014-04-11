#ifndef VORONOI_H
#define VORONOI_H

namespace Nimbus
{
	namespace Voronoi
	{
		class Voronoi
		{
		public:
			Voronoi(std::vector<Site> *pointx);
		private:
			std::vector<Site> points;
			std::priority_queue<Event> events;
		};
	}
}

#endif VORONOI_H