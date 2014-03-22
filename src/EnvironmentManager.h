#ifndef NIMBUS_ENVIRONMENTMANAGER_H
#define NIMBUS_ENVIRONMENTMANAGER_H

#include "Manager.h"

namespace Nimbus
{
	/** Manages all elements of the game world environment.

	 The environment consists of all properties of the game world which affect
	 the entities which exist dependent on the conditions of world.
	 */
	class EnvironmentManager :
		public Manager
	{
	public:
		EnvironmentManager(void);
		virtual ~EnvironmentManager(void);

		// From Nimbus::Manager
		virtual void initialize(void);
		virtual bool update(void);
	};

}

#endif
