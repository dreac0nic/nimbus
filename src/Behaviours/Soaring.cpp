#include "Soaring.h"
#include "../EventSystem/EventSystem.h"
#include "../WindMap.h"
#include <OgreVector3.h>

using namespace Nimbus;
using namespace Ogre;

Soaring::Soaring(BehaviourType type, World* world, EventSystem* eventSystem):
	Behaviour(type, world, eventSystem)
{
}

Soaring::Soaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem):
	Behaviour(type, world, initializingSettings, eventSystem)
{
}

Soaring::Soaring(Soaring* other, World* world, int id, EventSystem* eventSystem) :
	Behaviour(other, other->mWorld, id, eventSystem)
{
}

Soaring::~Soaring(void)
{
	// DESTROY ALL OF THE THINGS
}

void Soaring::init(void)
{
}

void Soaring::startup(void)
{
	// STARTUP FOR SOARING
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Soaring::update(void)
{
	// UPDATE THE FLOCK SOARING
	payloadmap payload;

	// Get the wind vector and convert it to a 3d vector
	Vector2 windVector = this->mWorld->getWindMap()->getWindVector(Vector2(0,0));
	Vector3 windVector3d = Vector3(windVector.x, 0, windVector.y);

	// Fire off the event to let the group know this cloud's contribution
	payload["EntityId"] = &this->mParentId;
	payload["PositionDelta"] = &windVector3d;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::SOAR_ENTITY, payload);
}

void Soaring::shutdown(void)
{
	// SUT DOWN THE SOARING
	/*
		LOLWUT
	*/
}

Behaviour* Soaring::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem)
{
	return new Soaring(this->mBehaviourType, this->mWorld, initializingSettings, eventSystem);
}

Behaviour* Soaring::clone(int id, EventSystem* eventSystem)
{
	return new Soaring(this, this->mWorld, id, eventSystem);
}