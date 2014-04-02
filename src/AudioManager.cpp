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
	AudioManager::mAsMap[id] = AudioManager::mSoundManager->create("music", path.c_str, true);
}

void Nimbus::AudioManager::start(AudioSourceGUID id, const bool& loop = false) {
   this->mAsMap[id]->play2d(loop);
}

void Nimbus::AudioManager::stop(AudioSourceGUID id) {
    this->mAsMap[id]->stop();
}

void Nimbus::AudioManager::loop(AudioSourceGUID id, const bool& loop = true) {
    this->mAsMap[id]->loop(loop);
}

void Nimbus::AudioManager::volume(AudioSourceGUID id, float volume) {
    this->mAsMap[id]->setVolume(volume);
}

void Nimbus::AudioManager::position(AudioSourceGUID id, Ogre::Vector3 *position) {
    this->mAsMap[id]->setPosition(cAudio::cVector3(position->x, position->y, position->z));
}

bool Nimbus::AudioManager::update(void) {
    // Run through behaviors and run sounds on behavior events
	// Run sounds on clicks of user interface elements
}

void Nimbus::AudioManager::AudioListener::handleEvent(payloadmap payload) {
	std::string soundBiteName = *(static_cast<std::string*>(payload["SoundName"]));
	Ogre::Vector3 position;

	if (payload.find("PositionDelta") != payload.end()) {
		position = *static_cast<Ogre::Vector3*>(payload["PositionDelta"]);
		this->parent->start(soundBiteName, position);
	} else {
		this->parent->start(soundBiteName);
	}
}