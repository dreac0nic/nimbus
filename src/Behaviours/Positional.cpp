#include "Positional.h"

using namespace Nimbus;
using namespace Ogre;

Positional::Positional(BehaviourType type, World* world):
	Behaviour(type, world)
{
	this->init(Vector3::ZERO, Vector3::ZERO);
}

Positional::Positional(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
}

Positional::Positional(Positional* other, World* world, int id):
	Behaviour(other, world, id)
{
	this->init(other->mPosition, other->mFacingVector);
}

Positional::~Positional(void)
{
	// DESTROY ALL OF THE THINGS
}

void Positional::startup(void)
{
	// STARTUP FOR POSITIONAL
	this->mDisplacementVector = Vector3::ZERO;

	// Register event listeners.
}

void Positional::update(void)
{
	// UPDATE THE POSITIONAL
	if(this->mDisplacementVector != Vector3::ZERO) {
		this->mPosition += this->mDisplacementVector;

		this->mDisplacementVector = Vector3::ZERO;
	}
}

void Positional::shutdown(void)
{
	// SUT DOWN THE POSITIONAL STUFF
	// Deregister event listeners.
}

Behaviour* Positional::clone(int id)
{
	return new Positional(this, this->mWorld, id);
}

Behaviour* Nimbus::Positional::clone(void)
{
	return new Nimbus::Positional(this, this->mWorld);
}