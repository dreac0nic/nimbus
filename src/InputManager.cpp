#include "InputManager.h"
#include <OgreRenderWindow.h>
#include <OgreLogManager.h>
#include "NimbusApplication.h"
#include "EventSystem/EventSystem.h"

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
    parameters.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    parameters.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    parameters.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    parameters.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));

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
	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent& evt)
{
	std::map<std::string, void*> mouseScreen;
    Ogre::Vector2 screenPosition = Ogre::Vector2(Ogre::Real(evt.state.X.abs), Ogre::Real(evt.state.Y.abs));
    mouseScreen["ScreenPosition"] = &screenPosition;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_UPDATE, mouseScreen);

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

	std::map<std::string, void*> mouseScreen;
    Ogre::Vector2 screenPosition = Ogre::Vector2(Ogre::Real(evt.state.X.abs), Ogre::Real(evt.state.Y.abs));
	mouseScreen["ButtonPressed"] = &id;
    mouseScreen["ScreenPosition"] = &screenPosition;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_DOWN, mouseScreen);

	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	std::map<std::string, void*> mouseScreen;
    Ogre::Vector2 screenPosition = Ogre::Vector2(Ogre::Real(evt.state.X.abs), Ogre::Real(evt.state.Y.abs));
	mouseScreen["ButtonPressed"] = &id;
    mouseScreen["ScreenPosition"] = &screenPosition;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_UP, mouseScreen);

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
