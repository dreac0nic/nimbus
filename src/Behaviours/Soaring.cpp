#include "Soaring.h"
#include "../EventSystem.h"
#include "../WindMap.h"
#include <OgreVector3.h>

using namespace Nimbus;
using namespace Ogre;

Soaring::Soaring(BehaviourType type, World* world):
	Behaviour(type, world)
{
}

Soaring::Soaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
}

Soaring::Soaring(Soaring* other, World* world, int id) :
	Behaviour(other, other->mWorld, id)
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

Behaviour* Soaring::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Soaring(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Soaring::clone(int id)
{
	return new Soaring(this, this->mWorld, id);
}