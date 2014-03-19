#include "AudioManager.h"
#include "NimbusApplication.h"

AudioManager::AudioManager(void) {
    this->mSoundManager = cAudio::createAudioManager(true);
}

AudioManager::~AudioManager(void) {
    this->mSoundManager->shutDown();
    cAudio::destroyAudioManager(this->mSoundManager);
}

AudioManager::create(AudioSourceGUID id, std::string path) {
    this-mAsMap[id] = this->mSoundManager->create("music", path, true);
}

AudioManager::start(AudioSourceGUID id) {
   // Stub!
}

AudioManager::stop(AudioSourceGUID id) {
    // Stub again!
}

AudioManager::loop(AudioSourceGUID id) {
    // Stubbiness
}

AudioManager::volume(AudioSourceGUID id) {
    // More stubbing!
}

AudioManager::position(AudioSourceGUID id, Ogre::Vector3 position) {
    // Officially stubbed.
}
