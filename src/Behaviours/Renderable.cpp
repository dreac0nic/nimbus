#include "Renderable.h"
#include <OgreSceneManager.h>

using namespace Ogre;
using namespace Nimbus;

Nimbus::Renderable::Renderable(BehaviourType type, World* world, EventSystem* eventSystem):
	Behaviour(type, world, eventSystem)
{
	this->init(NULL, Vector3::ZERO, "");
}

Nimbus::Renderable::Renderable(BehaviourType type, World* world, ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem):
	Behaviour(type, world, eventSystem)
{
	std::string ogreName;
	Vector3 scale;
	Ogre::Entity* model;

	// The way this works now is a bit different... so consider changing this.
	if(initializingSettings->find("ogreName") != initializingSettings->end())
	{
		ogreName = initializingSettings->find("ogreName")->second;
	}

	// Grabbing the actual mesh name
	if(initializingSettings->find("model") != initializingSettings->end())
	{
		std::string modelPath = initializingSettings->find("model")->second;
		std::stringstream nameStream;
		nameStream << ogreName << this->mWorld->getCurrentId();
		model = this->mWorld->getSceneManager()->createEntity(nameStream.str(), modelPath);
	}

	// Grabbing the initial scale
	if(initializingSettings->find("scale") != initializingSettings->end())
	{
		std::string scales = initializingSettings->find("scale")->second;
		std::stringstream scaleStream;
		scaleStream << scales;
		Ogre::Real x, y, z;
		scaleStream >> x >> y >> z;
		scale = Ogre::Vector3(x, y, z);
	}
	else
	{
		scale = Vector3(1,1,1);
	}

	// Initialize the object
	this->init(model, scale, ogreName);
}

Nimbus::Renderable::Renderable(Renderable* other, World* world, int id, EventSystem* eventSystem):
	Behaviour(other, world, id, eventSystem)
{
	this->init(other->getModel(), other->getScale(), other->mBaseEntityName);
}

Nimbus::Renderable::~Renderable()
{
	// Empty destructor
}

void Nimbus::Renderable::init(Ogre::Entity* ogreEntity, Vector3 scale, std::string entityBaseName)
{
	if(ogreEntity != NULL)
	{
		std::stringstream nameStream;
		nameStream << entityBaseName << this->mWorld->getCurrentId();
		this->mOgreEntity = this->mWorld->getSceneManager()->createEntity(nameStream.str(), ogreEntity->getMesh());
	}
	else
	{
		this->mOgreEntity = NULL;
	}

	this->mBaseEntityName = entityBaseName;
	this->mScale = scale;

	this->mParentId;

	this->mPositionListener = new PositionListener(this);
}

void Nimbus::Renderable::startup(void)
{
	// Create and attach the scene node for the entity
	this->mNode = mWorld->getWorldNode()->createChildSceneNode();
	this->mNode->attachObject(this->mOgreEntity);

	// Setting initial properties
	this->mNode->setScale(mScale.x, mScale.y, mScale.z);

	// Register the position update listener
	this->mEntityEventSystem->registerListener(mPositionListener, EventSystem::EventType::ENTITY_TRANSLATED);
}

void Nimbus::Renderable::update(void)
{
	//this->mNode->yaw(Degree(0.5), Node::TS_WORLD);
}

void Nimbus::Renderable::shutdown(void)
{
	// SHUTDOWN FOR RENDERABLE
	
	// Detatch the scene node
	mWorld->getWorldNode()->removeChild(this->mNode->getName());
}

Ogre::Entity* Nimbus::Renderable::getModel()
{
	return this->mOgreEntity;
}

void Nimbus::Renderable::setModel(Ogre::Entity* model)
{
	this->mOgreEntity = model;
}

Behaviour* Nimbus::Renderable::clone(ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem)
{
	return new Nimbus::Renderable(this->mBehaviourType, this->mWorld, initializingSettings, eventSystem);
}

Behaviour* Nimbus::Renderable::clone(int id, EventSystem* eventSystem)
{
	return new Nimbus::Renderable(this, this->mWorld, id, eventSystem);
}

void Nimbus::Renderable::PositionListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Temporary vectors for storing direction and position
	Vector3 position, facing, rotation, scale;

	int temp = *(static_cast<int*>(payload["EntityId"]));
	
	// Immediately return if the event is not for us
	if(*(static_cast<int*>(payload["EntityId"])) != parent->mParentId)
	{
		return;
	}

	// Apply position if the position was found
	if(payload.find("PositionVector") != payload.end())
	{
		position = *static_cast<Ogre::Vector3*>(payload["PositionVector"]);
		parent->mNode->setPosition(position);
	}

	// Apply facing vector if facing vector was found
	if(payload.find("FacingVector") != payload.end())
	{
		position = parent->mNode->getPosition();
		facing = *static_cast<Ogre::Vector3*>(payload["FacingVector"]);
		parent->mNode->lookAt(facing+position, Node::TransformSpace::TS_WORLD);
	}

	// Apply rotation vector if rotation vector was found
	if(payload.find("RotationVector") != payload.end())
	{
		rotation = *static_cast<Ogre::Vector3*>(payload["RotationVector"]);
		parent->mNode->pitch(Degree(rotation.x));
		parent->mNode->yaw(Degree(rotation.y));
		parent->mNode->roll(Degree(rotation.z));
	}

	// Apply scale vector if scale vector was found
	if(payload.find("ScaleVector") != payload.end())
	{
		scale = *static_cast<Ogre::Vector3*>(payload["ScaleVector"]);
		parent->mNode->scale(scale);
	}
}