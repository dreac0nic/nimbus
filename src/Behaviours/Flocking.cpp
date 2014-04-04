#include "Flocking.h"
#include <algorithm>

using namespace Nimbus;
using namespace Ogre;

Flocking::Flocking(BehaviourType type, World* world) :
	Behaviour(type, world)
{
	this->init(Real(1.0), Real(0.1));
}

Flocking::Flocking(BehaviourType type, World* world, ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
	stringstream optionsParser;

	Real influenceFactor = Real(1.0);
	Real overrideFactor = Real(0.1);

	// Load the influence factor for the flocking group
	if(initializingSettings->find("influence") != initializingSettings->end())
	{
		optionsParser = stringstream(initializingSettings->find("influence")->second);
		optionsParser >> influenceFactor;
	}

	// Load the override factor for the flocking group
	if(initializingSettings->find("override") != initializingSettings->end())
	{
		optionsParser = stringstream(initializingSettings->find("override")->second);
		optionsParser >> overrideFactor;
	}

	this->init(influenceFactor, overrideFactor);
}

Flocking::Flocking(Flocking* other, World* world, int id) :
	Behaviour(other, world, id)
{
	this->init(other->mComponentInfluenceFactor, other->mComponentOverrideFactor);
}

Flocking::~Flocking(void)
{
	// DESTROY ALL OF THE THINGS
	delete this->mTickListener;
	delete this->mSoarListener;
}

void Flocking::init(Real influenceFactor, Real overrideFactor)
{
	// Initialize constants
	mComponentInfluenceFactor = influenceFactor;
	mComponentOverrideFactor = overrideFactor;

	// Initial delta is zero
	this->mPositionDelta = Vector3::ZERO;

	// Create the listeners
	this->mSoarListener = new SoarListener(this);
	this->mTickListener = new TickListener(this);
}

void Flocking::startup(void)
{
	// Register the tick listener
	EventSystem::getSingleton()->registerListener(mTickListener, EventSystem::EventType::TICK);
}

void Flocking::update(void)
{
}

void Flocking::shutdown(void)
{
	// SUT DOWN THE FLOCK Flocking
	/*
		LOLWUT
	*/
}

Behaviour* Flocking::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Flocking(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Flocking::clone(int id)
{
	return new Flocking(this, this->mWorld, id);
}

void Flocking::SoarListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Get the entity id
	GameEntityId entityId = *static_cast<GameEntityId*>(payload["EntityId"]);

	// Return if we do not have this entity id in our entity list
	if(mParent->mEntities.find(entityId) == mParent->mEntities.end())
	{
		return;
	}

	// Default to delta of zero
	Vector3 positionDelta = Vector3::ZERO;
	Real componentFactor = Real(mParent->mComponentInfluenceFactor / mParent->mEntities.size());

	// Get the position delta
	if(payload.find("PositionDelta") != payload.end())
	{
		// Get the component entity's delta
		positionDelta = *static_cast<Ogre::Vector3*>(payload["PositionDelta"]);

		// Store the component delta
		mParent->mEntities[entityId] = positionDelta;

		// Initialize the delta vector if it is zero
		if(mParent->mPositionDelta == Vector3::ZERO)
		{
			mParent->mPositionDelta = positionDelta;
		}

		// Scale the position delta relative to the number of components
		positionDelta *= componentFactor;
	}

	// Aggregate the position delta of this component
	mParent->mPositionDelta *= 1 - componentFactor;
	mParent->mPositionDelta += positionDelta;
}

void Flocking::TickListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Create the translation event payload
	payloadmap translatePayload;

	// Send off a translation event for each entity in the group
	for(std::map<GameEntityId, Vector3>::iterator it = mParent->mEntities.begin(); it != mParent->mEntities.end(); ++it)
	{
		GameEntityId entityId = it->first;

		// Calculate a delta vector based on the group delta and the individual delta of the entity
		Vector3 delta = Real(1 - mParent->mComponentOverrideFactor) * mParent->mPositionDelta;
		delta += mParent->mComponentOverrideFactor * mParent->mEntities[entityId];

		// Change the translation vector of the entity based on the new direction
		translatePayload["EntityId"] = &entityId;
		translatePayload["PositionDelta"] = &delta;
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::BEGIN_TRANSLATE_ENTITY, translatePayload);
	}
}