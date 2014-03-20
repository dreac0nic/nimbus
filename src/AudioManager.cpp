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

AudioManager::start(AudioSourceGUID id, const bool& loop = false) {
   this->mAsMap[id]->play2d(loop);
}

AudioManager::stop(AudioSourceGUID id) {
    this->mAsMap[id]->stop();
}

AudioManager::loop(AudioSourceGUID id, const bool& loop) {
    this->mAsMap[id]->loop(loop);
}

AudioManager::volume(AudioSourceGUID id, float volume) {
    this->mAsMap[id]->setVolume(volume);
}

AudioManager::position(AudioSourceGUID id, Ogre::Vector3 position) {
    // Officially stubbed.
}
