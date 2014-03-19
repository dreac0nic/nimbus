#include "Positional.h"

using namespace Nimbus;
using namespace Ogre;

Nimbus::Positional::Positional(BehaviourType type, World* world):
	Behaviour(type, world)
{
}

Nimbus::Positional::Positional(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
}

Nimbus::Positional::~Positional(void)
{
	// DESTROY ALL OF THE THINGS
}

void Nimbus::Positional::startup(void)
{
	// STARTUP FOR POSITIONAL
	this->mDisplacementVector = Vector3::ZERO;

	// Register event listeners.
}

void Nimbus::Positional::update(void)
{
	// UPDATE THE POSITIONAL
	if(this->mDisplacementVector != Vector3::ZERO) {
		this->mPosition += this->mDisplacementVector;

		this->mDisplacementVector = Vector3::ZERO;
	}
}

void Nimbus::Positional::shutdown(void)
{
	// SUT DOWN THE POSITIONAL STUFF
	// Deregister event listeners.
}

Behaviour* Nimbus::Positional::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Positional(this->mWorld, initializingSettings);
}