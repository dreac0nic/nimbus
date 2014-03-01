#ifndef NIMBUS_BLANKBEHAVIOUR_H
#define NIMBUS_BLANKBEHAVIOUR_H

#include "Behaviour.h"

namespace Nimbus
{
	class BlankBehaviour:
		public Behaviour
	{
	public:
		BlankBehaviour(void);
		~BlankBehaviour(void);

		// From: Behaviour
		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
	};
}

#endif