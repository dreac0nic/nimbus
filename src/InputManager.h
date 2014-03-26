#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Manager.h"
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>

namespace Nimbus
{
	/** Manager to bind input to events in the game.

	Events should be passed through the event callback system so that in game
	activities are not associated directly to keys, but rather to generic event
	calls created by key presses.

	The input manager uses buffered input because buffered input is better than
	polled input. This of course is not true, but for some reason, callbacks seem
	cleaner. So we'll use them if possible. If polling is necessary, we have an
	update function. At the moment, this only does generic input checks, and this
	is probably for the best.

	Currently, the input manager is a single monolithic class for handling game
	input. As a design decision, input managers could be made specific to each
	RunMode. That way input in a menu would be handled differently than in the
	game.

	The other possibility is to simply send off events regardless of the mode.
	Since no listeners would be registered, the only adverse effect would be
	a few (< 5?) extra function calls per input action caught.
	*/
	class InputManager :
		public Manager,
		public Ogre::WindowEventListener,	// Subscribe to window size changes	
		public OIS::KeyListener,			// It should be fairly obvious why
		public OIS::MouseListener			// this class inherits these
	{
	private:
		// Member variables

		OIS::InputManager* mInputManager;
		OIS::Mouse* mMouse;
		OIS::Keyboard* mKeyboard;

	protected:
		// From Ogre::WindowEventListener
		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw); 

		// From OIS::KeyListener
		virtual bool keyPressed(const OIS::KeyEvent& evt);
		virtual bool keyReleased(const OIS::KeyEvent& evt);

		// From OIS::MouseListener
		virtual bool mouseMoved(const OIS::MouseEvent& evt);
		virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	public:
		/** Constructs the input manager.
		*/
		InputManager(void);

		virtual ~InputManager(void);

		// From Nimbus::Manager
		// Run once per frame to update input listeners.
		virtual bool update(void);
	};
}

#endif
