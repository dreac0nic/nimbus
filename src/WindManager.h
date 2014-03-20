#ifndef NIMBUS_WINDMANAGER_H
#define NIMBUS_WINDMANAGER_H

#include <string>

#include "manager.h"

namespace Nimbus
{
	/** Takes input and Updates WindMap
	 */
	class WindManager :
		public Manager
	{
	public:
		WindManager(void);
		virtual ~WindManager(void);

		// From Nimbus::Manager
		virtual bool update(void);
	};
}
#endif
