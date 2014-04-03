#include <OgreRoot.h>
#include "VectorMap.h"
#include "EventSystem.h"

namespace Nimbus
{
	VectorMap::VectorMap(int xLength, int yLength)
	{
		// Temporary solution
		if(xLength < 1){
			xLength = 1;
		}
		if(yLength < 1){
			yLength = 1;
		}

		map = std::vector<Ogre::Vector2>();
		for(int i = 0; i < xLength; i++)
		{
			for(int j = 0; j < yLength; j++)
			{
				map.push_back(Ogre::Vector2(0,0));
				// Create a representative arrow mesh
				payloadmap createArrowPayload;
				std::string type = "Arrow";
				createArrowPayload["EntityType"] = &type;
				createArrowPayload["PositionVector"] = new Ogre::Vector3((Ogre::Real)i, (Ogre::Real)-12, (Ogre::Real)j);
				EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createArrowPayload);
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
		map[length * length + posy] = Ogre::Vector2((Ogre::Real)strx, (Ogre::Real)stry);
	}
}