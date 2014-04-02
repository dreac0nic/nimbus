#ifndef NIMBUS_AUDIOMANAGER_H
#define NIMBUS_AUDIOMANAGER_H

#include "Manager.h"
#include <cAudio.h>
#include <OgreVector3.h>
#include "EventListener.h"

namespace Nimbus
{
	typedef std::string AudioSourceGUID;
	typedef std::map<AudioSourceGUID, cAudio::IAudioSource*> AudioSourceMap;

	class AudioManager :
		public Manager
	{
	private:
		static cAudio::IAudioManager *mSoundManager;
		static AudioSourceMap mAsMap;

	protected:
        static void create(AudioSourceGUID, std::string);
		virtual void start(AudioSourceGUID, const bool& = false);
		virtual void start(AudioSourceGUID, Ogre::Vector3, const bool& = false);
		virtual void stop(AudioSourceGUID);
		virtual void loop(AudioSourceGUID, const bool& = true);
		virtual void volume(AudioSourceGUID, float);
		virtual void position(AudioSourceGUID, Ogre::Vector3*);

		class AudioListener :
			public EventListener
		{
		private:
			AudioManager *parent;
		protected:
		public:
			AudioListener(AudioManager *parent) { this->parent = parent; }
			~AudioListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload);
		} *mAudioListener;

	public:
		AudioManager(void);
		virtual ~AudioManager(void);
        virtual bool update(void);
	}
}

#endif
