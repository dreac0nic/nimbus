#ifndef NIMBUS_GAMEENTITY_H
#define NIMBUS_GAMEENTITY_H

#include <map>

#include "Behaviour.h"

namespace Nimbus
{
	typedef std::string GameEntityType;
	typedef std::map<std::string, Behaviour*> behaviourmap;

	class GameEntity
	{
	private:
		/* Map of behaviours used by the entity. */
		behaviourmap mBehaviours;

		/* The type name of the game entity. */
		GameEntityType mEntityType;

		/* Test variable to prove that loading entities does indeed work. */
		int mAge;

	public:
		/* Standard blank constructor. */
		GameEntity(void);

		/* Settings based constructor. */
		GameEntity(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/* Prototype constructor. */
		GameEntity(GameEntity* other);

		/* Standard blank destructor. */
		~GameEntity(void);

		// OBJECT METHODS --
		void initialize(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		bool add(Behaviour* behaviour);
		void update(void);
		void remove(Behaviour* behaviour);

		// ACCESSOR METHODS --
		/* Return a reference to the behaviours used by this entity. */
		behaviourmap* getBehaviours(void) { return &(this->mBehaviours); }

		/* Return the entity type name. */
		GameEntityType getEntityType() { return this->mEntityType; }
	};
}

#endif