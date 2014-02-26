#ifndef NIMBUS_GAMEENTITY_H
#define NIMBUS_GAMEENTITY_H

#include <map>

#include "Behaviour.h"

namespace Nimbus
{
	typedef std::map<std::string, Behaviour> behaviourmap;

	class GameEntity
	{
	private:
		/* Map of behaviours used by the entity. */
		behaviourmap behaviours;

	public:
		/* Standard blank constructor. */
		GameEntity(void);

		/* Standard blank destructor. */
		~GameEntity(void);

		// OBJECT METHODS --
		bool add(Behaviour behaviour);
		void update(void);
		void remove(Behaviour behaviour);

		// ACCESSOR METHODS --
		/* Return a reference to the behaviours used by this entity. */
		behaviourmap* getBehaviours(void) { return &(this->behaviourmap); }
	};
}

#endif