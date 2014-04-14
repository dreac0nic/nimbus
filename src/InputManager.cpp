#include "InputManager.h"
#include <OgreRenderWindow.h>
#include <OgreLogManager.h>
#include "NimbusApplication.h"
#include "EventSystem.h"

using namespace Nimbus;

InputManager::InputManager(void)
{
	//////////
	// Initialize the OIS input system
	Ogre::LogManager::getSingleton().logMessage("*** Initializing OIS ***");
	OIS::ParamList parameters;
	size_t windowHandle = 0;
	std::ostringstream windowHandleString;

	// Get the Ogre::RenderWindow
	Ogre::RenderWindow* window = NimbusApplication::getRenderWindow();

	// Get the window handle
	window->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleString << windowHandle;
	parameters.insert(std::make_pair(std::string("WINDOW"), windowHandleString.str()));

	//TESTING FOR MOUSE CURSOR!
	#if defined OIS_WIN32_PLATFORM
		parameters.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		parameters.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		parameters.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		parameters.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    #elif defined OIS_LINUX_PLATFORM
		parameters.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		parameters.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		parameters.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
		parameters.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
		#endif

	// Create the input system using the window handle
	mInputManager = OIS::InputManager::createInputSystem(parameters);

	// Create the specific input listener objects (note: true because buffered)
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

	// Register the InputManager as a keyboard and mouse listener
	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);

	// Set the initial window size for the mouse input object
	windowResized(window);

	// Register as a Window listener for future resize and close events
	Ogre::WindowEventUtilities::addWindowEventListener(window, this);
}

InputManager::~InputManager(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(NimbusApplication::getRenderWindow(), this);
	windowClosed(NimbusApplication::getRenderWindow());
}

bool InputManager::update(void)
{
	// Capture input events
	mKeyboard->capture();
	mMouse->capture();

	return true;
}

bool InputManager::keyPressed(const OIS::KeyEvent& evt)
{
	std::stringstream logstring;

	logstring << "The " << evt.text << " key was pressed\n";

	std::cerr << logstring.str().c_str();

	// Send key pressed event!
	std::map<std::string, void*> keyEvent;

	bool tempReleased = false;

	keyEvent["KeyCode"] = (void*)&(evt.key);
	keyEvent["KeyReleased"] = &tempReleased;

	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::KEY_PRESS, keyEvent);

	// Hack to close the application when the user presses escape because we don't
	// have a proper event system yet
	if(evt.key == OIS::KC_ESCAPE)
	{
		//NimbusApplication::exitEvent();
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::SHUTDOWN);
	}

	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent& evt)
{
	// Send key released event!
	std::map<std::string, void*> keyEvent;

	bool tempReleased = true;

	keyEvent["KeyCode"] = (void*)&(evt.key);
	keyEvent["KeyReleased"] = &tempReleased;

	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::KEY_PRESS, keyEvent);

	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent& evt)
{
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	std::stringstream logstring;

	if(evt.state.buttonDown(OIS::MB_Left))
	{
		logstring << "The left mouse button was pressed at ("
			<< evt.state.X.abs << ", " << evt.state.Y.abs << ")"
			<< std::endl;
	}
	if(evt.state.buttonDown(OIS::MB_Right))
	{
		logstring << "The right mouse button was pressed at ("
			<< evt.state.X.abs << ", " << evt.state.Y.abs << ")"
			<< std::endl;
	}

	std::cerr << logstring.str().c_str();

	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}

void InputManager::windowResized(Ogre::RenderWindow* rw)
{
	//////////
	// Adjust the mouse input area to the new window size

	// Get the metrics of the new window size
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	// Modify the input manager to track the new size
	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow* rw)
{
	// If the relevant window is closing
	if(rw = NimbusApplication::getRenderWindow())
	{
		// And the input manager has been created
		if(mInputManager)
		{
			// Destroy the specific listeners
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);

			// Then destroy the entire input manager
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}