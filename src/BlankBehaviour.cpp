#include "BlankBehaviour.h"

using namespace Ogre;
using namespace Nimbus;

BlankBehaviour::BlankBehaviour(void):
	Behaviour()
{
	// Empty constructor
}

BlankBehaviour::BlankBehaviour(ConfigFile::SettingsMultiMap* initializingSettings)
{
	// (Pretty munch) Empty settings constructor

	this->mBehaviourType = (*(initializingSettings->find("name"))).second;
}

BlankBehaviour::~BlankBehaviour(void)
{
	// Empty destructor
}

void BlankBehaviour::startup(void)
{
	// IT IS BLANK YOU DO NOT STARTUP NOTHING
}

void BlankBehaviour::update(void)
{
	// IT IS BLANK YOU DO NOT UPDATE NOTHING
}

void BlankBehaviour::shutdown(void)
{
	// IT IS BLANK YOU DO NOT SHUTDOWN NOTHING
}

Behaviour* BlankBehaviour::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	// IT IS (Pretty munch) BLANK YOU DO CLONE NOTHING

	return new BlankBehaviour(initializingSettings);
}