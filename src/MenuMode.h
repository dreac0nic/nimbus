#ifndef MENUMODE_H
#define MENUMODE_H

#include "RunMode.h"
#include "EventSystem/EventSystem.h"

namespace Nimbus
{
	class MenuMode :
		public RunMode
	{
	private:
		// Member variables
		Ogre::SceneManager* mSceneMgr;
		Ogre::Camera* mCamera;
		Ogre::Viewport* mViewport;
		RunMode* gameModePointer;
		

	protected:
		

		/** Listens for the key event
		*/
		class KeyListener : 
			public EventListener
		{
		public:
			KeyListener() {}
			virtual ~KeyListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
            virtual std::string str();
		} *keyListener;
	
		class MouseListener :
			public EventListener
		{

		public:
			Ogre::Viewport* viewport;

			MouseListener(Ogre::Viewport* mViewport) {viewport = mViewport; return;}
			virtual ~MouseListener() {}

			void setViewport(Ogre::Viewport* tempViewport) { viewport = tempViewport; }

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
            virtual std::string str();
		} *mouseListener;

	public:
		/** Creates a TestMode type run mode.
		 */
		MenuMode(void);
		virtual ~MenuMode(void);
		// From Nimbus::RunMode
		virtual void initialize();
		virtual void stop();
		virtual void pause();

		/** Runs the test mode.
		 */
		virtual RunMode* run(const Ogre::FrameEvent& evt);
	};
}

#endif
