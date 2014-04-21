#ifndef MENUMODE_H
#define MENUMODE_H

#include "RunMode.h"
#include "EventSystem.h"

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

	protected:
		// From Nimbus::RunMode
		virtual bool initialize();

		/** Listens for the key event
		*/
		class KeyListener : 
			public EventListener
		{
		public:
			KeyListener() {}
			virtual ~KeyListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload);
		} *keyListener;

		/** Listens for the mouse event
		*/
		class MouseListener :
			public EventListener
		{
		public:
			Ogre::Viewport* viewport;
			void SetViewport(Ogre::Viewport* mViewport){viewport = mViewport; return;}

			MouseListener(Ogre::Viewport* mViewport) {viewport = mViewport; return;}
			virtual ~MouseListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload);
		} *mouseListener;

	public:
		/** Creates a TestMode type run mode.
		 */
		MenuMode(void);
		virtual ~MenuMode(void);

		/** Runs the test mode.
		 */
		virtual RunMode* run(const Ogre::FrameEvent& evt);
	};
}

#endif
