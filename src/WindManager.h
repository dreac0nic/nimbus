#ifndef NIMBUS_WINDMANAGER_H
#define NIMBUS_WINDMANAGER_H

#include <string>
#include "WindMap.h"
#include "manager.h"

namespace Nimbus
{
	static const double STRENGTHTOSUBTRACT = 1;
	static const double ORIGININFLUENCE = 0.5;
	static const double CORNERINFLUENCE = .10355339059327;
	static const double SIDEINFLUENCE = .1464466094073;

	/** Takes input and Updates WindMap
	*/
	class WindManager :
		public Manager
	{
	public:
		WindManager(int sizex, int sizey);
		virtual ~WindManager(void);

		// Stores the game's WindMap
		WindMap* windMap;

		// From Nimbus::Manager
		virtual bool update(void);
	};
}
#endif
