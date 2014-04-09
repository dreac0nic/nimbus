#include "Flocking.h"
#include <algorithm>
#include <list>

using namespace Nimbus;
using namespace Ogre;

Flocking::Flocking(BehaviourType type, World* world, EventSystem* eventSystem) :
	Behaviour(type, world, eventSystem)
{
	this->init(Real(1.0), Real(0.1));
}

Flocking::Flocking(BehaviourType type, World* world, EventSystem* eventSystem, ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings, eventSystem)
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

Flocking::Flocking(Flocking* other, World* world, int id, EventSystem* eventSystem) :
	Behaviour(other, world, id, eventSystem)
{
	this->init(other->mComponentInfluenceFactor, other->mComponentOverrideFactor);
}

Flocking::~Flocking(void)
{
	// DESTROY ALL OF THE THINGS
	delete this->mTickListener;
	delete this->mSoarListener;
	delete this->mUpdateListener;
}

void Flocking::init(Real influenceFactor, Real overrideFactor)
{
	// Initialize constants
	mComponentInfluenceFactor = influenceFactor;
	mComponentOverrideFactor = overrideFactor;

	// Initial delta is zero
	this->mPositionDelta = Vector3::ZERO;

	// Create the listeners
	this->mUpdateListener = new UpdateListener(this);
	this->mSoarListener = new SoarListener(this);
	this->mTickListener = new TickListener(this);
}

void Flocking::startup(void)
{
	// Register the tick listener
	EventSystem::getSingleton()->registerListener(mUpdateListener, EventSystem::EventType::FLOCK_UPDATE);
	EventSystem::getSingleton()->registerListener(mSoarListener, EventSystem::EventType::SOAR_ENTITY);
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

Behaviour* Flocking::clone(ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem)
{
	return new Flocking(this->mBehaviourType, this->mWorld, initializingSettings, eventSystem);
}

Behaviour* Flocking::clone(int id, EventSystem* eventSystem)
{
	return new Flocking(this, this->mWorld, id, eventSystem);
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
		this->mEntityEventSystem->fireEvent(EventSystem::EventType::TRANSLATE_ENTITY, translatePayload);
	}
}

void Flocking::UpdateListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Make sure the event is for this entity
	if(mParent->mParentId != *static_cast<GameEntityId*>(payload["EntityId"]))
	{
		return;
	}

	// If there is a valid entity list, store it
	if(payload.find("EntityList") != payload.end())
	{
		std::list<GameEntityId> entityList = *static_cast<std::list<GameEntityId>* >(payload["EntityList"]);

		// Clear the current entity list
		mParent->mEntities.clear();

		// Go through each entity id and add it to the entity map
		for(std::list<GameEntityId>::iterator it = entityList.begin(); it != entityList.end(); ++it)
		{
			// Store a zero vector for each entity
			mParent->mEntities[*it] = Vector3::ZERO;
		}
	}
}
