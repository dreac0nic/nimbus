#ifndef NIMBUS_WINDMANAGER_H
#define NIMBUS_WINDMANAGER_H

#include <string>

#include "EventListener.h"
#include "Manager.h"

namespace Nimbus
{
	/** Takes input and Updates WindMap
	 */
	class WindManager :
		public Manager
	{
	protected:
		// Event Listeners

		/** Listens for the mouse path event
		*/
		class PathListener : 
			public EventListener
		{
		public:
			PathListener() {}
			virtual ~PathListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload);
		};

	public:
		WindManager(void);
		virtual ~WindManager(void);

		// From Nimbus::Manager
		virtual bool update(void);
	};
}
#endif
