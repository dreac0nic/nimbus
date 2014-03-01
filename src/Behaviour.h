#ifndef NIMBUS_BEHAVIOUR_H
#define NIMBUS_BEHAVIOUR_H

#include <string>

namespace Nimbus
{
	typedef std::string BehaviourType;
	/* THE ULTIMATE STUBBINESS */
	class Behaviour
	{
	public:
		Behaviour(void);
		~Behaviour(void);

		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
	};
}

#endif