#ifndef NIMBUS_AUDIOMANAGER_H
#define NIMBUS_AUDIOMANAGER_H

#include "Manager.h"
#include <cAudio.h>
#include <OgreVector3.h>

namespace Nimbus
{
	typedef std::string AudioSourceGUID;
	typedef std::map<AudioSourceGUID, cAudio::IAudioSource*> AudioSourceMap;

	class AudioManager :
		public Manager
	{
	private:
		cAudio::IAudioManager *soundManager;
		AudioSourceMap asMap;

	protected:
		virtual void start(AudioSourceGUID);
		virtual void stop(AudioSourceGUID);
		virtual void loop(AudioSourceGUID);
		virtual void volume(AudioSourceGUID, float);
		virtual void position(AudioSourceGUID, Ogre::Vector3);

	public:
		AudioManager(void);
		virtual ~AudioManager(void);
	}
}

#endif