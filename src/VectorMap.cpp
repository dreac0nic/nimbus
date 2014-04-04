#include <OgreRoot.h>
#include "VectorMap.h"
#include "EventSystem.h"

namespace Nimbus
{
	VectorMap::VectorMap(int xLength, int yLength)
	{
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
				EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createArrowPayload);
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
}