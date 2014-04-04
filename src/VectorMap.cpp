#include <OgreRoot.h>
#include "VectorMap.h"
#include "EventSystem.h"

namespace Nimbus
{
	VectorMap::VectorMap(int xLength, int yLength)
	{
		this->mCatchEntityListener = new CatchEntityListener();

		this->arrows = new Grid<GameEntityId>(xLength, yLength);

		int tempx = xLength;// / VECTORMAPTOWORLDRATIO;
		int tempy = yLength;// / VECTORMAPTOWORLDRATIO;

		map = std::vector<Ogre::Vector2>();
		for(int i = 0; i < tempx; i++)
		{
			for(int j = 0; j < tempy; j++)
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
		
		length = tempx;
	}

	VectorMap::~VectorMap(void)
	{
	}

	Ogre::Vector2 VectorMap::getVector(int posx, int posy)
	{
		int tempx = posx / 250;// / VECTORMAPTOWORLDRATIO;
		int tempy = posy / 250;// / VECTORMAPTOWORLDRATIO;
		return map.at(tempx * length + tempy);
	}

	void VectorMap::setVector(int posx, int posy, double strx, double stry)
	{
		int tempx = posx / 250;// / VECTORMAPTOWORLDRATIO;
		int tempy = posy / 250;// / VECTORMAPTOWORLDRATIO;
		map[tempx * length + tempy] = Ogre::Vector2(Ogre::Real(strx), Ogre::Real(stry));
	}

	void VectorMap::updateArrows()
	{
		// The payload variables
		payloadmap positionPayload;
		GameEntityId entityId;

		// Preload the pointer to the entity id
		positionPayload["EntityId"] = &entityId;

		for(int x = 0; x < this->length; ++x)
		{
			for(int y = 0; y < this->length; ++y)
			{
				// Update the entity id
				entityId = arrows->get(x, y);

				// Get and load the corresponding wind vector
				positionPayload["FacingVector"] = &this->map[x*length + y];

				// Fire off the event
				EventSystem::getSingleton()->fireEvent(EventSystem::EventType::POSITION_ENTITY, positionPayload);
			}
		}
	}

	GameEntityId VectorMap::getArrowId(int posx, int posy)
	{
		return this->arrows->get(posx / 250, posy / 250);
	}

	void VectorMap::CatchEntityListener::handleEvent(payloadmap payload, EventListener* responder)
	{

		if(payload.find("EntityId") != payload.end())
		{
			this->mEntityId = *static_cast<GameEntityId*>(payload["EntityId"]);
		}
	} 
}
