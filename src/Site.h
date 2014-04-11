#ifndef VORONOI_SITE_H
#define VORONOI_SITE_H

#include <OgreRoot.h>

namespace Nimbus
{
	namespace Voronoi
	{
		class Site
		{
		public:
			Site(Ogre::Real x, Ogre::Real y);
			~Site(void);

		private:
			Ogre::Real x;
			Ogre::Real y;

		public:
			Ogre::Real getX(void) { return this->x; }
			Ogre::Real getY(void) { return this->y; }
		};
	}
}

#endif