#ifndef NIMBUS_GAMEENTITY_H
#define NIMBUS_GAMEENTITY_H

#include <map>

#include "./Behaviours/Behaviour.h"

namespace Nimbus
{
	typedef std::string GameEntityType;
	typedef std::map<std::string, Behaviour*> behaviourmap;

	class GameEntity
	{
	private:
		/** Map of behaviours used by the entity. */
		behaviourmap mBehaviours;

		/** The type name of the game entity. */
		GameEntityType mEntityType;

		/** The id of the entity. */
		GameEntityId mId;

		/** The event system for the entity. */
		EventSystem* mEventSystem;

	public:
		/** Standard blank constructor. */
		GameEntity(GameEntityId id, GameEntityType type);

		/** Prototype constructor. */
		GameEntity(GameEntityId id, GameEntity* other);

		/** Standard blank destructor. */
		~GameEntity(void);

		/** Configure the entity according to the general settings map.

		This settings map is obtained from the general section in the Entity Type
		ini file. It contains all general information needed to construct the entity.
		*/
		void configure(Ogre::ConfigFile::SettingsMultiMap* configurationSettings);

		// OBJECT METHODS --
		void startup(void);
		bool addBehaviour(Behaviour* behaviour, std::string name);
		void update(void);
		void removeBehaviour(Behaviour* behaviour);

		// ACCESSOR METHODS --
		/** Return a reference to the behaviours used by this entity. */
		behaviourmap* getBehaviours(void) { return &(this->mBehaviours); }

		/** Return the entity type name. */
		GameEntityType getEntityType() { return this->mEntityType; }

		/** Gets the id of the entity. */
		int getEntityId() { return this->mId; }
	};
}

#endif