#ifndef NIMBUS_BEHAVIOUR_H
#define NIMBUS_BEHAVIOUR_H

namespace Nimbus
{
	/* THE ULTIMATE STUBBINESS */
	class Behaviour
	{
	public:
		Behaviour(void);
		~Behaviour(void);

		void startup(void);
		void update(void);
		void shutdown(void)
	};
}

#endif