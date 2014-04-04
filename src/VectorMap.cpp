#include <OgreRoot.h>

#include "VectorMap.h"
#include "EventSystem.h"

namespace Nimbus
{
	VectorMap::VectorMap(int xLength, int yLength)
	{
		this->mCatchEntityListener = new CatchEntityListener();

		this->arrows = new Grid<GameEntityId>(xLength, yLength);

		map = std::vector<Ogre::Vector2>();
		for(int i = 0; i < xLength; i++)
		{
			for(int j = 0; j < yLength; j++)
			{
				map.push_back(Ogre::Vector2(0, 0));

				// Create a representative arrow mesh
				payloadmap createArrowPayload;
				std::string type = "Arrow";
				createArrowPayload["EntityType"] = &type;
				Ogre::Vector3 pos = Ogre::Vector3(Ogre::Real(i * 250), Ogre::Real(-12), Ogre::Real(j * 250));
				createArrowPayload["PositionVector"] = &pos;
				EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createArrowPayload, this->mCatchEntityListener);

				// Store the arrow id that we just created in the corresponding grid location
				arrows->set(i, j, this->mCatchEntityListener->getEntityId());
			}
		}
		
		length = xLength;
	}

	VectorMap::~VectorMap(void)
	{
	}

	Ogre::Vector2 VectorMap::getVector(int posx, int posy)
	{
		return map.at(posx * length + posy);
	}

	void VectorMap::setVector(int posx, int posy, double strx, double stry)
	{
		map[posx * length + posy] = Ogre::Vector2(Ogre::Real(strx), Ogre::Real(stry));
	}

	GameEntityId VectorMap::getArrowId(int posx, int posy)
	{
		return this->arrows->get(posx, posy);
	}

	void VectorMap::CatchEntityListener::handleEvent(payloadmap payload, EventListener* responder)
	{

		if(payload.find("EntityId") != payload.end())
		{
			this->mEntityId = *static_cast<GameEntityId*>(payload["EntityId"]);
		}
	} 
}
