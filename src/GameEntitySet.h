#ifndef NIMBUS_GAMEENTITYSET_H
#define NIMBUS_GAMEENTITYSET_H

#include <iterator>
#include <map>

#include "GameEntity.h"

namespace Nimbus
{
	typedef std::map<GameEntityType, GameEntity> entitysetmap;
	/* THE ULTIMATE STUBBINESS */
	class GameEntitySet
	{
	private:
		std::map<GameEntityType, GameEntity> entities;

	public:
		GameEntitySet(void);
		~GameEntitySet(void);

		std::iterator<std::input_iterator_tag, GameEntity>* getIterator(GameEntityType);
	};
}

#endif