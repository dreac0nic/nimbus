#include "AudioManager.h"
#include "NimbusApplication.h"

Nimbus::AudioManager::AudioManager(void) {
    this->mSoundManager = cAudio::createAudioManager(true);
}

Nimbus::AudioManager::~AudioManager(void) {
    this->mSoundManager->shutDown();
    cAudio::destroyAudioManager(this->mSoundManager);
}

void Nimbus::AudioManager::create(AudioSourceGUID id, std::string path) {
	AudioManager::mAsMap[id] = AudioManager::mSoundManager->create("music", path.c_str(), true);
}

void Nimbus::AudioManager::start(AudioSourceGUID id, const bool& loop) {
	this->mAsMap[id]->play2d(loop);
}

void Nimbus::AudioManager::start(AudioSourceGUID id, Ogre::Vector3 *position, const bool& loop) {
	this->mAsMap[id]->play3d(fromOgreVector(position), 1, loop);
}

void Nimbus::AudioManager::stop(AudioSourceGUID id) {
    this->mAsMap[id]->stop();
}

void Nimbus::AudioManager::loop(AudioSourceGUID id, const bool& loop) {
    this->mAsMap[id]->loop(loop);
}

void Nimbus::AudioManager::volume(AudioSourceGUID id, float volume) {
    this->mAsMap[id]->setVolume(volume);
}

void Nimbus::AudioManager::position(AudioSourceGUID id, Ogre::Vector3 *position) {
    this->mAsMap[id]->setPosition(fromOgreVector(position));
}

bool Nimbus::AudioManager::update(void) {
    // Run through behaviors and run sounds on behavior events
	// Run sounds on clicks of user interface elements
	return true;
}

/**
 * Handle events called by the nimbus application for firing off sounds
 * and play any sounds with a given sound name and, optionally, a
 * relative position vector.
 */
void Nimbus::AudioManager::AudioListener::handleEvent(payloadmap payload) {
	std::string soundBiteName = *(static_cast<std::string*>(payload["SoundName"]));
	Ogre::Vector3 *position;

	if (payload.find("PositionDelta") != payload.end()) {
		position = static_cast<Ogre::Vector3*>(payload["PositionDelta"]);
		this->parent->start(soundBiteName, position);
	} else {
		this->parent->start(soundBiteName);
	}
}

cAudio::cVector3 fromOgreVector(Ogre::Vector3 *ogreVector) {
	cAudio::cVector3 *newPosition = new cAudio::cVector3(ogreVector->x, ogreVector->y, ogreVector->z);

	return *newPosition;
}